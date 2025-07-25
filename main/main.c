#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_system.h"
#include "esp_timer.h"
#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE
#include "esp_log.h"

#include "ds18b20.h"

const char *TAG = "main";
const char *DS18B20_TAG = "ds18b20";
TimerHandle_t temp_timer;
int interval = 2000; // Timer interval in milliseconds
esp_err_t set_temp_timer(void *pvParameters);
void read_temp(TimerHandle_t temp_timer);

void app_main()
{
    esp_log_level_set("*", ESP_LOG_INFO);            // Set log level for all components to INFO
    esp_log_level_set(DS18B20_TAG, ESP_LOG_VERBOSE); // Set log level for this tag t
    set_temp_timer(NULL); // Set up the temperature timer
    if (temp_timer != NULL)
    {
        xTimerStart(temp_timer, 0); // Start the timer
        ESP_LOGI(TAG, "Temperature timer started");
    }
}

esp_err_t set_temp_timer(void *pvParameters)
{
    temp_timer = xTimerCreate("temp_timer",
                 pdMS_TO_TICKS(interval), // Convert milliseconds to ticks
                 pdTRUE,                  // Auto-reload timer
                 NULL,                    // Timer ID (not used here)
                 read_temp);              // Callback function to read temperature
    if (temp_timer == NULL)
    {
        ESP_LOGE(TAG, "Failed to create temperature timer");
        return ESP_FAIL;
    }
    else
    {
        ESP_LOGI(TAG, "Temperature timer created successfully");
    }
    return ESP_OK; // Placeholder for timer setup if needed
}

void read_temp(TimerHandle_t temp_timer)
{

    ds18b20_init(GPIO_NUM_4);                        // Initialize DS18B20 on GPIO 4
    ESP_LOGD(TAG, "DS18B20 initialized on GPIO 4");
    ds18b20_reset();
    ESP_LOGI(DS18B20_TAG, "Temperature: %0.1f", ds18b20_get_temp());
}

