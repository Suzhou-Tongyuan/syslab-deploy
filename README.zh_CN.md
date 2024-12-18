[![构建说明](https://img.shields.io/badge/中文-构建说明-blue.svg)](https://github.com/Suzhou-Tongyuan/syslab-deploy/blob/main/STEP.zh_CN.md)


# Syslab Deploy

Syslab Deploy 是一个模板项目，可以将 Syslab/Julia 编写的程序集成到跨平台的 GUI
应用中。

功能：

- 开箱即用支持 Windows/macOS/Linux/Android (iOS 尚未测试)
- 一行命令将 Julia 代码编译为对应平台的动态库

## 使用

- 构建 Windows 应用：`flutter build windows`
- 构建 Android 应用: `flutter build apk --split-per-abi`

## 架构

- `julia/`: `julia/build.jl` 包含生成静态库的 Julia 代码入口，使用
  `SyslabCC.static_compile` 导出 C 函数符号。运行 `julia/compile.sh` 将待集成的
  C++ 项目，生成位置是 `natice/syslabcc_lib/syslabcc_lib`。

- `native/syslabcc_lib`: 使用 Rust 的 cargokit/rinf 工具链集成生成的 C++
  项目。如果生成的 Julia 代码导出新的函数符号， 需修改 `lib.rs` 文件。

  ```rust
  #[link(name = "syslabcc_lib", kind = "static")]
  extern "C" {
    // you modify this according to the symbols you export from Julia
    #[allow(non_snake_case)]
    pub fn simulate(
        U: *mut f64,   // Nt x Ny x Nx array
        u: *const f64, // Ny x Nx matrix
        n_x: i64,      // number of rows
        n_y: i64,      // number of columns
        n_t: i64,      // number of time steps
        alpha: f64,    // thermal diffusivity
        dx: f64,       // spatial step size (x)
        dy: f64,       // spatial step size (y)
        dt: f64,       // time step size
    ) -> u8;
  }
  ```

- `native/hub`:

  此项目使用标准的 `rinf` 结构，实现了 Flutter 与 Julia 导出函数的操作交互。

- `lib/`: 实际的 Flutter 项目，调用 Julia 库编写应用。
