/**
 * 该模块初始化一个GPIO作为BlinkLED的驱动引脚，并创建一个FreeRTOS任务来实现LED的闪烁。
 */

#ifndef BLINK_LED_H
#define BLINK_LED_H

/* Includes - ESP APIs -------------------------------------------------------------------------------- */
#include "esp_log.h"
#include "sdkconfig.h"

/* Includes - Driver APIs ----------------------------------------------------------------------------- */
#include "driver/gpio.h"

/* Includes - FreeRTOS APIs --------------------------------------------------------------------------- */
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "freertos/timers.h"

/* Public Function Declarations ----------------------------------------------------------------------- */
esp_err_t blink_led_start(void);

#endif // BLINK_LED_H
