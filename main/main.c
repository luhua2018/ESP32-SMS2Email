/* include -------------------------------------------------------------------------------------------- */
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "sdkconfig.h"

/* Includes - User APIs ------------------------------------------------------------------------------- */
#include "blink_led.h"

/* static value ---------------------------------------------------------------------------------------- */
static const char *TAG = "app_main";

/* main function --------------------------------------------------------------------------------------- */

void app_main(void)
{
    ESP_ERROR_CHECK(blink_led_start());

    ESP_LOGW(TAG, "Heap: %u/%u", heap_caps_get_free_size(MALLOC_CAP_DEFAULT), heap_caps_get_total_size(MALLOC_CAP_DEFAULT));

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
