use cc::Build;
use std::path::{Path, PathBuf};
use std::sync::{LazyLock, RwLock};
use walkdir::WalkDir;

static DEPENDENCY_FILES: LazyLock<RwLock<Vec<String>>> = LazyLock::new(|| RwLock::new(Vec::new()));

fn add_dependency_files(files: &[String]) {
    let mut lock = DEPENDENCY_FILES.write().unwrap();
    lock.extend(files.iter().cloned());
}

fn get_dependency_files() -> Vec<String> {
    let lock = DEPENDENCY_FILES.read().unwrap();
    lock.clone()
}

fn is_platform(plat: &str) -> bool {
    let target_os = std::env::var("CARGO_CFG_TARGET_OS");
    target_os
        .as_ref()
        .map(|x| x.contains(plat))
        .unwrap_or(false)
}

fn get_files(path: &str, ext: &str, recursive: bool) -> Vec<String> {
    let mut result: Vec<String> = Vec::new();

    if recursive {
        let walker = WalkDir::new(path);
        for entry in walker {
            let entry = entry.unwrap();
            if let Some(found_ext) = entry.path().extension() {
                if found_ext == ext {
                    result.push(entry.path().to_str().unwrap().to_string());
                }
            }
        }
    } else {
        let files = std::fs::read_dir(path).unwrap();
        for file in files {
            let file = file.unwrap();
            if let Some(found_ext) = file.path().extension() {
                if found_ext == ext {
                    result.push(file.path().to_str().unwrap().to_string());
                }
            }
        }
    }

    if is_platform("windows") {
        // replace / with \\ for windows
        result = result.iter().map(|x| x.replace("/", "\\")).collect();
    }

    return result;
}

pub fn change_extension(path: &str, ext: &str) -> String {
    let path = Path::new(path);
    let path = path.with_extension(ext);
    path.display().to_string()
}

fn mark_intentional_no_warn(_build: &mut Build, _use_cpp: bool) {
    // build
    //     .flag("-Wno-unused")
    //     .flag("-Wno-implicit-fallthrough")
    //     .flag("-Wno-sign-compare")
    //     .flag("-Wno-parentheses")
    //     .flag("-Wno-type-limits");

    // if use_cpp {
    //     build.flag("-Wno-class-memaccess").flag("-Wno-reorder");
    // }
}

// pure c build
fn build_bdwgc() -> Vec<PathBuf> {
    let mut build = Build::new();

    let include_dir = &["syslabcc_lib/bdwgc/include", "syslabcc_lib/atomic_ops/src"];

    let mut define_bdwgc = vec![
        "GC_ATOMIC_UNCOLLECTABLE",
        "GC_GCJ_SUPPORT",
        "JAVA_FINALIZATION",
        "NO_EXECUTE_PERMISSION",
        "USE_MMAP",
        "USE_MUNMAP",
        "GC_PTHREADS",
        "GC_THREADS",
        "PARALLEL_MARK",
        "THREAD_LOCAL_ALLOC",
    ];

    if is_platform("windows") {
        build.define("_CRT_SECURE_NO_WARNINGS", None);
        if build.get_compiler().is_like_msvc() {
            define_bdwgc.push("WIN32_THREADS");
        }
    } else if is_platform("linux") {
        build.flag("-D_GNU_SOURCE");
    }

    build.file("syslabcc_lib/win32-implib/bdwgc.c");

    define_bdwgc.iter().for_each(|define| {
        build.define(define, None);
    });

    if is_platform("windows") {
        build.define("WIN32_LEAN_AND_MEAN", None);
    }

    let src_files = get_files("syslabcc_lib/bdwgc", "c", false);
    add_dependency_files(&src_files);
    for dir in include_dir {
        let header_files = get_files(&dir, ".h", true);
        add_dependency_files(&header_files);
    }

    build.files(src_files).includes(include_dir);

    build.compile_intermediates()
}

fn build_syslabcrt() -> Vec<PathBuf> {
    let mut build = Build::new();
    let mut files = get_files("syslabcc_lib/syslabcrt-io/src", "c", true);
    files.extend(get_files("syslabcc_lib/syslabcrt-dylib", "c", true));
    let include_dir = "syslabcc_lib/syslabcrt-io/include";

    add_dependency_files(&files);
    add_dependency_files(&get_files(&include_dir, ".h", true));

    build.files(files).include(include_dir);

    mark_intentional_no_warn(&mut build, false);

    if is_platform("windows") {
        build.define("WIN32_LEAN_AND_MEAN", None);
    }

    build.compile_intermediates()
}

fn build_lib() -> Vec<PathBuf> {
    let mut build = Build::new();
    let library_file = "syslabcc_lib/out.cpp";
    build.cpp(true).file(library_file);

    add_dependency_files(&[library_file.into()]);

    let mut include_dirs: Vec<String> = vec![
        "syslabcc_lib/syslabcrt-io/include".into(),
        "syslabcc_lib/syslabcrt-intrinsics".into(),
    ];

    if is_platform("windows") {
        if build.get_compiler().is_like_msvc() {
            build.flag("/std:c++17");
            include_dirs.push("win-io-patches".into());
        }
        // pass
    } else {
        include_dirs.push("syslabcc_lib/ghc-filesystem".into());
        build.flag("-fno-strict-aliasing");
    }

    mark_intentional_no_warn(&mut build, true);

    for dir in &include_dirs {
        add_dependency_files(&get_files(&dir, ".h", true));
        add_dependency_files(&get_files(&dir, ".hpp", true));
    }

    build.includes(&include_dirs);

    // Add system libraries needed for dynamic loading
    if is_platform("linux") {
        println!("cargo:rustc-link-lib=dylib=dl");
        println!("cargo:rustc-link-lib=dylib=pthread");
    }

    build.compile_intermediates()
}

pub fn build_into_one() {
    let mut build = Build::new();

    build
        .cpp(true)
        .objects(build_bdwgc())
        .objects(build_syslabcrt())
        .objects(build_lib())
        .compile("syslabcc_lib");

    if is_platform("windows") {
        println!("cargo:rustc-link-lib=user32");
    }
}

fn main() {
    // https://rinf.cunarist.com/frequently-asked-questions/#my-app-failed-to-load-dynamic-library
    // https://github.com/Losses/rune/pull/26
    if is_platform("android") {
        println!("cargo:rustc-link-lib=dylib=stdc++");
        println!("cargo:rustc-link-lib=c++_shared");
    }

    build_into_one();

    // write the rebuild token
    println!("cargo:rerun-if-changed=force-rebuild-token");
    println!("cargo:rerun-if-changed=build.rs");

    // write the dependency files
    for file in get_dependency_files() {
        println!("cargo:rerun-if-changed={}", file);
    }
}
