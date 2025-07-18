#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "sdkconfig.h"

static const char *TAG = "app_main";

#define BLINK_GPIO CONFIG_BLINK_GPIO

static uint8_t s_led_state = 0;

static void blink_led(void)
{
    /* Set the GPIO level according to the state (LOW or HIGH)*/
    gpio_set_level(BLINK_GPIO, s_led_state);
    s_led_state = !s_led_state;
}

static esp_err_t configure_led(void)
{
    esp_err_t ret = gpio_reset_pin(BLINK_GPIO);
    if (ret == ESP_OK)
    {
        ret = gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
    }
    if (ret == ESP_OK)
    {
        ESP_LOGI(TAG, "Blink LED init done!");
    }
    return ret;
}

void app_main(void)
{
    ESP_ERROR_CHECK(configure_led());

    while (1)
    {
        ESP_LOGI(TAG, "Turning the LED %s!", s_led_state == true ? "ON" : "OFF");
        blink_led();
        vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
    }
}
