/* Include -------------------------------------------------------------------------------------------- */
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "sdkconfig.h"

/* Includes - User APIs ------------------------------------------------------------------------------- */
#include "blink_led.h"
#include "ntp_client.h"

/* Function declaration ------------------------------------------------------------------------------- */
void tmp_station_start(void);

/* Static value ---------------------------------------------------------------------------------------- */
static const char *TAG = "app_main";

/* Main function --------------------------------------------------------------------------------------- */

void app_main(void)
{
    ESP_ERROR_CHECK(blink_led_start());

    ESP_LOGW(TAG, "Heap: %u/%u", heap_caps_get_free_size(MALLOC_CAP_DEFAULT), heap_caps_get_total_size(MALLOC_CAP_DEFAULT));

    tmp_station_start();
    ntp_client_init(NULL);

    time_t now;
    if(ntp_client_get_timestamp(&now) == NTP_CLIENT_OK)
    {
        ESP_LOGI(TAG, "NTP time: %s", ctime(&now));
    }

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
