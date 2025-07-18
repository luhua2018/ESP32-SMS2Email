/* include -------------------------------------------------------------------------------------------- */
#include "blink_led.h"

/* define relay config --------------------------------------------------------------------------------- */
#define BLINK_GPIO CONFIG_BLINK_GPIO

/* static value ---------------------------------------------------------------------------------------- */
static const char *TAG = "blink_led";

/* static function ------------------------------------------------------------------------------------- */

/**
 * @brief BlinkLED的任务函数
 */
static void blink_led_task(void *pvParameter)
{
    static uint8_t s_led_state = 0;
    while (1)
    {
        gpio_set_level(BLINK_GPIO, s_led_state);
        s_led_state = !s_led_state;
        vTaskDelay(pdMS_TO_TICKS(CONFIG_BLINK_PERIOD));
    }
}

/* public function ------------------------------------------------------------------------------------- */

/**
 * @brief 启动BlinkLED
 */
esp_err_t blink_led_start(void)
{
    /* 初始化引脚 */
    esp_err_t ret = gpio_reset_pin(BLINK_GPIO);
    if (ret == ESP_OK)
    {
        ret = gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
    }
    /* 创建闪烁任务 */
    if (ret == ESP_OK)
    {
        if (xTaskCreate(blink_led_task, "blink_led_task", 1024, NULL, 16, NULL) != pdPASS)
        {
            ESP_LOGE("TASK", "Failed to create task");
            ret = ESP_FAIL;
        }
        else
        {
            ESP_LOGI(TAG, "Blink LED startup success!");
        }
    }
    return ret;
}
