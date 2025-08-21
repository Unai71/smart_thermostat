#include "library_imports.h"
#include "temp_h_data.h"
#include "wifi_scan.h"

const char *TAG = "main";
const char *DS18B20_TAG = "ds18b20";

void app_main()
{
    esp_log_level_set("*", ESP_LOG_INFO);            // Set log level for all components to INFO
    esp_log_level_set(DS18B20_TAG, ESP_LOG_VERBOSE); // Set log level for this tag

    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // Commented out temperature sensor setup for now
    // config_temp_sensor(); 
    // set_temp_timer(NULL); // Set up the temperature timer
    // if (temp_timer != NULL)
    // {
    //     xTimerStart(temp_timer, 0); // Start the timer
    //     ESP_LOGI(TAG, "Temperature timer started");
    // }

    // Initialize Wi-Fi for scanning
    ret = wifi_scan_init();
    if (ret != ESP_OK) {
        ESP_LOGE("app_main", "Wi-Fi initialization failed: %s", esp_err_to_name(ret));
        return;
    }

    // Create a task for continuous Wi-Fi scanning
    xTaskCreate(wifi_scan_task, "wifi_scan_task", 4096, NULL, 5, NULL);

}

// Commented out the temperature timer setup function for now
// esp_err_t set_temp_timer(void *pvParameters)
// {
//     temp_timer = xTimerCreate("temp_timer",
//                  pdMS_TO_TICKS(interval), // Convert milliseconds to ticks
//                  pdTRUE,                  // Auto-reload timer
//                  NULL,                    // Timer ID (not used here)
//                  read_temp);              // Callback function to read temperature
//     if (temp_timer == NULL)
//     {
//         ESP_LOGE(TAG, "Failed to create temperature timer");
//         return ESP_FAIL;
//     }
//     else
//     {
//         ESP_LOGI(TAG, "Temperature timer created successfully");
//     }
//     return ESP_OK; // Placeholder for timer setup if needed
// }

