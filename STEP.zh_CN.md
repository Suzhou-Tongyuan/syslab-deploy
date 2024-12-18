# 项目构建步骤

## 安装 Rust 工具链

- 目的：跨平台 C++ 源码构建和 Flutter 移动应用集成
- 前置条件：配置 Rust 镜像
    - 配置包镜像： https://mirrors.ustc.edu.cn/help/crates.io-index.html
    - 配置工具链镜像： https://mirrors.ustc.edu.cn/help/rust-static.html

- 步骤指南： https://learn.microsoft.com/zh-cn/windows/dev-environment/rust/setup
- 安装内容包括：
    - 安装 MVSC 工具链
    - 安装 Rust 工具链 (rustup, cargo)
    - 将 Rust 工具链设置为 msvc (命令： `rustup default stable-msvc`)


## 安装 Flutter 工具链

- 要求：SDK 版本  >= 3.27
- 安装指南：https://docs.flutter.cn/get-started/install

## 安装 Flutter C++ 集成框架

命令：`cargo install rinf`

## 运行/构建应用程序


### 本机调试及构建

- 本机调试运行：`flutter run`
- 为 Windows 构建（要求本机为 Windows）：`flutter build windows --release`
- 为 Linux 构建（要求本机为 Linux）：`flutter build linux --release`

### 安卓调试及构建

1. 根据以下链接内容，下载 Android Studio，并指定 SDK 下载位置：

    > https://docs.flutter.cn/get-started/install/windows/mobile#configure-android-development

    注意：首次安装时，Android Studio 会自动提示安装所需组件，部分网络环境可能需要设置代理。


2. 设置 Flutter 的 Android SDK 路径，并接受许可。

    ```bash
    # flutter config --android-sdk <Android SDK> 安装路径
    flutter config --android-sdk D:\Android


    # 执行以下命令，全部选 y 接受许可
    flutter doctor --android-licenses
    ```

3. （可选）配置下载仓库：

    Android 构建需要下载资源，在 `<syslab-deploy根目录>/android/build.gradle` 中修改下载仓库：

    ```gradle
    //        google()
    //        jcenter()
    maven { url 'https://maven.aliyun.com/repository/google' }
    maven { url 'https://maven.aliyun.com/repository/jcenter' }
    maven { url 'http://maven.aliyun.com/nexus/content/groups/public' }
    ```

    此问题也可在 `.gradle/gradle.properties` 中设置代理解决。

4. 使用适配的数据线将 PC 与 Android 手机相连，并打开 Android 手机的 USB 调试，具体参考：https://docs.flutter.cn/get-started/install/windows/mobile#configure-android-development


5. 使用安装手机调试程序：`flutter devices` 列出设备，找到安卓设备（编号格式类似是 `17111222C`），然后使用命令 `flutter run -d 17111222C` 运行安装程序。

    注意：手机可能会请求安装应用，此时需要在手机上点击同意。

6. 发布 Android 应用：`flutter build apk --split-per-abi` 生成 apk 文件，可以将发布应用传递给其他 Android 手机用户进行安装，用户可在 Android 应用中体验 Julia 编写的程序功能。


## 修改 Julia 代码并重新编译

1. 修改 `<syslab-deploy根目录>/julia/build.jl`，修改其中的 julia 代码（可使用 `include`函数或调用 `Pkg` 包来集成外部库）。

2. 打开 Syslab 进入 `<syslab-deploy根目录>julia` 目录，运行 `compile.sh` 脚本，或直接运行命令 `scc build.jl --mode shared -c -d ../native/syslabcc_lib/syslabcc_lib/ --no-blas`。

3. 重新运行 `flutter run` 调试修改后的程序，或 `flutter build ...` 构建修改后的应用。