# 📖 Readme

Branch : **main**

Supported Targets : ESP32-C3

## 💡 概述

基于 ESP32C3 和 Air780E 的 SMS 转发到 Email 的项目。正在开发中。

如果调整项目文件结构或切换分支后编译遇到问题请尝试 `rm -r build/`, 因为 build 目录不被 Git 追踪。

## ✅ TODO LIST

- [ ] 模组 Flash 大小为 4M, 当前分区表只用了 2M, 后续需要适时调整。

## ⚙️ 资源分配

### GPIO

- GPIO8: Blink LED (蓝色)。

### Flash

分区表见文件 `./partitions.csv`

## 📅 更新记录

### 📝 2025/07/17

#### 实现 Blink LED

#### 修改分区表

1. Flash size 大小修改为4M.
2. 使用自自定义分区表文件 `./partitions.csv`.

#### 🟡 创建项目

项目名在 `./CMakeLists.txt` 里设置。
