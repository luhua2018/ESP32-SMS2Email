/**
 * 该模块初始化一个GPIO作为BlinkLED的驱动引脚，并创建一个FreeRTOS任务来实现LED的闪烁。
 */

#ifndef BLINK_LED_H
#define BLINK_LED_H

/* Public Function Declarations ----------------------------------------------------------------------- */
esp_err_t blink_led_start(void);

#endif // BLINK_LED_H
