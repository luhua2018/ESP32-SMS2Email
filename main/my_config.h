/**
 * 包含一些个性化的私有配置，比如开发板型号、WiFi密码、Email信息等
 *
 * 本文件应该使用'--skip-worktree'标记，避免将私人信息提交到Git仓库
 * 如确需提交相关更改(比如修改配置格式)，请先取消'--skip-worktree'标记，修改并提交后可再次标记。
 */

#ifndef MY_CONFIG_H
#define MY_CONFIG_H

#define CONFIG_ESP_WIFI_SSID     "myssid"
#define CONFIG_ESP_WIFI_PASSWORD "mypassword"

/* 宏开关，选用哪种LED */
#define USE_GPIO_LED
// #define USE_STRIP_LED

#endif // MY_CONFIG_H