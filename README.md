# 📖 Readme

Branch : **main**

Supported Targets : ESP32-C3

## 🚧 项目声明 / Disclaimer

本项目目前处于起步和开发阶段，主要用于个人学习和练手。功能和结构都还在不断调整完善，暂未达到生产或实际使用标准。如果您恰好浏览到本仓库，请不要对项目的可用性和完成度抱有过高期望，以免辜负您的期待。欢迎提出建议，但请知悉本项目尚不成熟。

This project is at an early stage of development and is primarily intended for personal practice and learning. Features and structure are still under active adjustment and improvement, and the project is not ready for production or practical use. If you find this repository, please do not expect too much in terms of usability or completeness to avoid disappointment. Suggestions are welcome, but please be aware that the project is not yet mature.

## 💡 概述

基于 ESP32C3 和 Air780E 的 SMS 转发到 Email 的项目。正在开发中。

如果调整项目文件结构或切换分支后编译遇到问题请尝试 `rm -r build/`, 因为 build 目录不被 Git 追踪。

AIR780E 也可能需要更新固件，请参考[这篇文档](./assets/air780ex/readme.md)。

## ✅ TODO LIST

- [ ] 模组 Flash 大小为 4M, 当前分区表只用了 2M, 后续需要适时调整。
- [ ] 需要把当前使用的 WiFi 连接方式 (./components/tmp_station) 替换成使用统一配网服务。

## ⚙️ 资源分配

### GPIO

- GPIO8: Blink LED (蓝色)。

### Flash

分区表见文件 `./partitions.csv`

## 📅 更新记录

### 📝 2025/07/29

#### 添加了 STRIP_LED 的支持

1. 在 `main/my_config.h` 中配置宏来决定使用哪种 LED.
2. LED 的引脚和闪烁周期在 `components/blink_led/idf_component.yml` 中默认配置，可使用 `menuconfig` 修改。

#### 排除了对 dependencies.lock 的跟踪

### 📝 2025/07/18

#### 添加了WiFi 连接示例

添加了使用官方简化版方式连接 WiFi 的示例 (./components/tmp_station), 后续请替换成使用统一配网服务连接 WiFi.

#### 调整了工程结构

1. 调整了工程结构，创建了自定义组件目录并添加了说明。
2. 把 Blink LED 封装成了自定义组件。

### 📝 2025/07/17

#### 实现 Blink LED

实现 Blink LED, 并按照 ESP-IDF 官方 API 的风格修改了 LED 初始化函数。

#### 修改分区表

1. Flash size 大小修改为4M.
2. 使用自自定义分区表文件 `./partitions.csv`.

#### 🟡 创建项目

项目名在 `./CMakeLists.txt` 里设置。
