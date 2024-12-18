
[![中文](https://img.shields.io/badge/中文-README-orange.svg)](https://github.com/Suzhou-Tongyuan/syslab-deploy/blob/main/README.zh_CN.md) [![构建说明](https://img.shields.io/badge/中文-构建说明-blue.svg)](https://github.com/Suzhou-Tongyuan/syslab-deploy/blob/main/STEP.zh_CN.md)

# Syslab Deploy

Syslab Deploy is a template project for integrating Syslab/Julia programs into
cross-platform GUI apps.

Features:

- Out-of-the-box support for Windows, macOS, Linux and Android (iOS not tested)
- One line build command to compile Julia code into a static library for each
  platform

## Usage

- Build for Windows: `flutter build windows`
- Build for Android: `flutter build apk --split-per-abi`

## Architecture

- `julia/`: `julia/build.jl` contains the entrypoint for building Julia code
  into a static library. Running `julia/compile.sh` will build the library into a C++ project located in `native/syslabcc_lib/syslabcc_lib/`.

- `native/syslabcc_lib/`: Integrating the generated C++ project using Rust's
  cargokit/rinf toolchain. If you export different symbols from Julia, you might
  modify the `lib.rs` file:

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

  This uses standard `rinf` structure, which implements the interops between
  Flutter and Julia exported functions.

- `lib/`: This is the actual Flutter project which calls functions exported from
  your Julia library.
