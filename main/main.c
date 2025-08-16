#include "library_imports.h"
#include "temp_h_data.h"

const char *TAG = "main";
const char *DS18B20_TAG = "ds18b20";
TimerHandle_t temp_timer;
int interval = 2000; // Timer interval in milliseconds
float tsens_out; 
esp_err_t set_temp_timer(void *pvParameters);

void app_main()
{
    esp_log_level_set("*", ESP_LOG_INFO);            // Set log level for all components to INFO
    esp_log_level_set(DS18B20_TAG, ESP_LOG_VERBOSE); // Set log level for this tag t
    config_temp_sensor(); 
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

