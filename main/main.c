#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "esp_log.h"

#include "wifi_provisioning/manager.h"
#include "wifi_provisioning/scheme_softap.h"   // o scheme_ble si prefieres BLE

static const char *TAG = "provisioning_example";

static void event_handler(void *arg, esp_event_base_t event_base,
                          int32_t event_id, void *event_data)
{
    if (event_base == WIFI_PROV_EVENT) {
        switch (event_id) {
        case WIFI_PROV_START:
            ESP_LOGI(TAG, "Provisioning started");
            break;
        case WIFI_PROV_CRED_RECV: {
            wifi_sta_config_t *wifi_sta_cfg = (wifi_sta_config_t *)event_data;
            ESP_LOGI(TAG, "Received SSID:%s", (const char *)wifi_sta_cfg->ssid);
            break;
        }
        case WIFI_PROV_CRED_FAIL:
            ESP_LOGE(TAG, "Provisioning failed!");
            break;
        case WIFI_PROV_CRED_SUCCESS:
            ESP_LOGI(TAG, "Provisioning successful!");
            break;
        case WIFI_PROV_END:
            ESP_LOGI(TAG, "Provisioning finished");
            wifi_prov_mgr_deinit();
            break;
        default:
            break;
        }
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ESP_LOGI(TAG, "Connected to Wi-Fi!");
    }
}

void app_main(void)
{
    // Init NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ESP_ERROR_CHECK(nvs_flash_init());
    }
    ESP_LOGI(TAG, "NVS initialized");
    
    // Init TCP/IP stack
    ESP_ERROR_CHECK(esp_netif_init());
    
    // Init event loop
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    // Register handlers early so provisioning events are received
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_PROV_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL));
    
    esp_netif_create_default_wifi_sta();
    esp_netif_create_default_wifi_ap();

    // Ensure Wi-Fi driver is initialized before provisioning manager
    wifi_init_config_t wifi_cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&wifi_cfg));

    // Inicializar el provisioning manager
    wifi_prov_mgr_config_t prov_cfg = {
        .scheme = wifi_prov_scheme_softap,
        .scheme_event_handler = WIFI_PROV_EVENT_HANDLER_NONE
    };

    // don't hide the error: capture it and log a readable message
    esp_err_t err = wifi_prov_mgr_init(prov_cfg);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "wifi_prov_mgr_init failed: %s", esp_err_to_name(err));
        return; // or handle recovery
    }

    // OPTIONAL for debugging: clear previously stored provisioning (uncomment to force fresh state)
    esp_err_t r = wifi_prov_mgr_reset_provisioning();
    ESP_LOGI(TAG, "wifi_prov_mgr_reset_provisioning: %s", esp_err_to_name(r));

    bool provisioned = false;
    
    ESP_ERROR_CHECK(wifi_prov_mgr_is_provisioned(&provisioned));


    if (!provisioned) {
        ESP_LOGI(TAG, "Starting provisioning");

        // Configura el AP temporal
        wifi_prov_mgr_start_provisioning(WIFI_PROV_SECURITY_1, "abcd1234",
                                         "esp_provisioning_example", NULL);
        // password "abcd1234", SSID "PROV_ESP32"
    } else {
        ESP_LOGI(TAG, "Already provisioned, connecting to Wi-Fi");
        wifi_prov_mgr_deinit();
        wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
        ESP_ERROR_CHECK(esp_wifi_init(&cfg));
        ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
        ESP_ERROR_CHECK(esp_wifi_start());
    }
}
