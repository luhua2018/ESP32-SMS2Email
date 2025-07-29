/* Includes - User config ----------------------------------------------------------------------------- */
#include "my_config.h"
/* Includes - ESP APIs -------------------------------------------------------------------------------- */
#include "esp_log.h"
#include "esp_check.h"
#include "sdkconfig.h"
/* Includes - FreeRTOS APIs --------------------------------------------------------------------------- */
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
/* Includes - Driver APIs ----------------------------------------------------------------------------- */
#include "driver/gpio.h"
/* Includes - Components APIs ------------------------------------------------------------------------- */
#if defined(USE_STRIP_LED)
#include "led_strip.h"
#include "led_strip_types.h"
#endif
/* Includes - User APIs ------------------------------------------------------------------------------- */
#include "blink_led.h"

/* Define Blink_LED config ---------------------------------------------------------------------------- */
#define BLINK_GPIO CONFIG_BLINK_GPIO

/* Static value ---------------------------------------------------------------------------------------- */
static const char *TAG = "blink_led";
#if defined(USE_STRIP_LED)
static led_strip_handle_t led_strip;
#endif

/* Static function ------------------------------------------------------------------------------------- */

/**
 * @brief 初始化Blink_LED
 */
static esp_err_t blink_led_init(void)
{
    ESP_RETURN_ON_ERROR(gpio_reset_pin(BLINK_GPIO), TAG, "GPIO reset failed.");

#if defined(USE_GPIO_LED)
    gpio_config_t io_conf = {
        .pin_bit_mask = 1ULL << BLINK_GPIO,
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLUP_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    ESP_RETURN_ON_ERROR(gpio_config(&io_conf), TAG, "GPIO config failed.");
    gpio_set_level(BLINK_GPIO, 0);
    ESP_LOGI(TAG, "Configured GPIO_LED succeed.");
#elif defined(USE_STRIP_LED)
    led_strip_config_t strip_config = {
        .strip_gpio_num = BLINK_GPIO,
        .max_leds = 1, // at least one LED on board
    };
    led_strip_rmt_config_t rmt_config = {
        .resolution_hz = 10 * 1000 * 1000, // 10MHz
        .flags.with_dma = false,
    };
    ESP_RETURN_ON_ERROR(led_strip_new_rmt_device(&strip_config, &rmt_config, &led_strip), TAG, "Strip_LED config failed.");
    led_strip_clear(led_strip);
    ESP_LOGI(TAG, "Configured Strip_LED succeed.");
#endif

    return ESP_OK;
}

/**
 * @brief Blink_LED的任务函数
 */
void blink_led_task(void *pvParameter)
{
    static uint8_t s_led_state = 0;

    while (1)
    {
#if defined(USE_GPIO_LED)
        gpio_set_level(BLINK_GPIO, s_led_state);
#elif defined(USE_STRIP_LED)
        if (s_led_state)
        {
            /* Set the LED pixel using RGB from 0 (0%) to 255 (100%) for each color */
            led_strip_set_pixel(led_strip, 0, 16, 16, 16);
            /* Refresh the strip to send data */
            led_strip_refresh(led_strip);
        }
        else
        {
            /* Set all LED off to clear all pixels */
            led_strip_clear(led_strip);
        }
#endif
        s_led_state = !s_led_state;
        vTaskDelay(pdMS_TO_TICKS(CONFIG_BLINK_PERIOD));
    }
}

/* Public function ------------------------------------------------------------------------------------- */

/**
 * @brief 启动Blink_LED
 */
esp_err_t blink_led_start(void)
{
    esp_err_t ret = ESP_OK;

    /* 初始化引脚 */
    ESP_RETURN_ON_ERROR(blink_led_init(), TAG, "Blink_LED init failed.");

    /* 创建闪烁任务 */
    if (xTaskCreate(blink_led_task, "blink_led_task", 1024, NULL, 16, NULL) != pdPASS)
    {
        ESP_LOGE("TASK", "Failed to create task");
        ret = ESP_FAIL;
    }
    else
    {
        ESP_LOGI(TAG, "Blink LED startup success!");
    }

    return ret;
}
