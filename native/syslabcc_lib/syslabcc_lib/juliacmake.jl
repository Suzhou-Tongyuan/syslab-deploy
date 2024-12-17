using Pkg

function is_windows7()
    if Sys.iswindows()
        version_info = Sys.windows_version()
        return version_info.major == 6 && version_info.minor == 1
    end
    return false
end

function check_and_install_cmake(auto_confirm::Bool=false)
    installed = false
    version = v"0.0.0"
    for (uuid, pkg) in Pkg.dependencies()
        if pkg.name == "CMake_jll"
            installed = true
            version = VersionNumber(pkg.version)
            break
        end
    end

    if (is_windows7() && installed && version > v"3.29.0")
        println("CMake_jll 3.28.1 or earlier is required for Windows 7. Current version: $version")

        if auto_confirm
            response = "y"
        else
            println("Do you want to uninstall the existing CMake_jll and install version 3.28.1? [y/n]")
            response = readline()
        end

        if response != "y"
            println("Error: CMake_jll version is incompatible with Windows 7 and was not uninstalled.")
            return
        end
        println("Uninstalling existing CMake_jll version...")
        Pkg.rm("CMake_jll")
        installed = false
    end

    if !installed
        println("Installing CMake_jll 3.28.1...")
        Pkg.add(PackageSpec(name="CMake_jll", version="3.28.1"))
    end
end

# 需要手动安装CMake_jll或者使用Syslab环境
# check_and_install_cmake(true)

using CMake_jll

gen_vs = "--gen-vs" in ARGS

if Sys.iswindows()
    if gen_vs
        include("msvc_crlf.jl")
        run(`$(cmake()) -S . -B Builds -G 'Visual Studio 17 2022' -DUSE_MSVC=""`)
    else
        run(`$(cmake()) -S . -B build -G "Ninja" -DCMAKE_BUILD_TYPE=Release`)
    end
elseif Sys.islinux()
    run(`$(cmake()) -S . -B build -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release`)
elseif Sys.isapple()
    run(`$(cmake()) -S . -B build -G "Xcode" -DCMAKE_BUILD_TYPE=Release`)
else
    error("Unsupported platform: $(Sys.KERNEL)")
end
if !gen_vs
    # 只有在生成非vc项目的时候用cmake构建
    run(`$(cmake()) --build build -j8 `)
end