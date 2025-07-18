# components Readme

## 关于 components

参考文档：[构建系统（CMake 版） - ESP32 - — ESP-IDF 编程指南 v4.2.4 文档](https://docs.espressif.com/projects/esp-idf/zh_CN/v4.2.4/esp32/api-guides/build-system.html)

`components` 目录是官方推荐的存放**自定义组件**的路径，并且官方文档中也明确指出"**如果项目中源文件较多，建议将其归于组件中，而不是全部放在 “main” 中**"。

### 何为组件(components)

1. 组件 (Component) 的核心思想是模块化和代码复用。
2. 任何可复用的、功能独立的模块都可以被视为组件。
3. 自定义驱动(如屏幕、按键)和功能封装(如网络配置、蓝牙配置)无疑也属于组件。
4. `main` 目录本身也是一个特殊的组件，它的核心职责是作为"负责将其他组件整合到一起的主程序"。

### 组件(components)目录下应该有什么

1. 自定义组件：自己编写的、具有模块化和独立功能的代码。
2. 第三方组件：从其他地方获取的、非 ESP-IDF 官方提供的库或驱动。
3. 每个组件目录都包含一个 CMakeLists.txt 文件，里面会定义一些变量以控制该组件的构建过程，以及其与整个项目的集成。
4. 每个组件还可以包含一个 Kconfig 文件，它用于定义 menuconfig 时展示的 组件配置 选项。
5. 某些组件可能还会包含 Kconfig.projbuild 和 project_include.cmake 特殊文件，它们用于覆盖项目的部分设置。
