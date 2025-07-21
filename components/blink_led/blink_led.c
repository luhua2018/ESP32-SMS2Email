#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "blink_led.h"
#include "my_config.h"
#include "esp_log.h"
#include "driver/gpio.h"
#if BLINK_LED_TYPE == USE_WS2812_LED
#include "led_strip.h"
static led_strip_handle_t led_strip = NULL;
#endif

#define BLINK_GPIO    MY_BLINK_GPIO
#define BLINK_PERIOD  MY_BLINK_PERIOD

static const char *TAG = "blink_led";

esp_err_t configure_led(void)
{
#if BLINK_LED_TYPE == USE_GPIO_LED
    gpio_config_t io_conf = {
        .pin_bit_mask = 1ULL << BLINK_GPIO,
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = 0,
        .pull_down_en = 0,
        .intr_type = GPIO_INTR_DISABLE,
    };
    gpio_config(&io_conf);
    gpio_set_level(BLINK_GPIO, 0);
    ESP_LOGI(TAG, "Configured GPIO LED");
#elif BLINK_LED_TYPE == USE_WS2812_LED
    led_strip_config_t strip_config = {
        .strip_gpio_num = BLINK_GPIO,
        .max_leds = MY_WS2812_NUM,
    };
    led_strip_rmt_config_t rmt_config = {
        .resolution_hz = 10 * 1000 * 1000,
        .flags.with_dma = false,
    };
    ESP_ERROR_CHECK(led_strip_new_rmt_device(&strip_config, &rmt_config, &led_strip));
    led_strip_clear(led_strip);
    ESP_LOGI(TAG, "Configured WS2812 LED strip");
#else
    #error "Unsupported LED type"
#endif
    return ESP_OK;
}

void blink_led_task(void *pvParameter)
{
    while (1) {
#if BLINK_LED_TYPE == USE_GPIO_LED
        gpio_set_level(BLINK_GPIO, 1);
        vTaskDelay(BLINK_PERIOD / portTICK_PERIOD_MS / 2);
        gpio_set_level(BLINK_GPIO, 0);
        vTaskDelay(BLINK_PERIOD / portTICK_PERIOD_MS / 2);
#elif BLINK_LED_TYPE == USE_WS2812_LED
        led_strip_set_pixel(led_strip, 0, 0, 0, 16);
        led_strip_refresh(led_strip);
        vTaskDelay(BLINK_PERIOD / portTICK_PERIOD_MS / 2);

        led_strip_set_pixel(led_strip, 0, 0, 0, 0);
        led_strip_refresh(led_strip);
        vTaskDelay(BLINK_PERIOD / portTICK_PERIOD_MS / 2);
#endif
    }
}
void blink_led_start(void)
{
    configure_led();
    xTaskCreate(blink_led_task, "blink_led_task", 2048, NULL, 5, NULL);
}