#include <stdio.h>
#include "wifi_stack.h"
static const char *wifi_tag ="WIFI-STACK";



static void wifi_event_handler(void *arg, esp_event_base_t event_base,
                          int32_t event_id, void *event_data)
{
    if (event_base == WIFI_PROV_EVENT) {
        switch (event_id) {
        case WIFI_PROV_START:
            ESP_LOGI(wifi_tag, "Provisioning started");
            break;
        case WIFI_PROV_CRED_RECV: {
            wifi_sta_config_t *wifi_sta_cfg = (wifi_sta_config_t *)event_data;
            ESP_LOGI(wifi_tag, "Received SSID:%s", (const char *)wifi_sta_cfg->ssid);
            break;
        }
        case WIFI_PROV_CRED_FAIL:
            ESP_LOGE(wifi_tag, "Provisioning failed!");
            break;
        case WIFI_PROV_CRED_SUCCESS:
            ESP_LOGI(wifi_tag, "Provisioning successful!");
            break;
        case WIFI_PROV_END:
            ESP_LOGI(wifi_tag, "Provisioning finished");
            wifi_prov_mgr_deinit();
            break;
        default:
            break;
        }
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ESP_LOGI(wifi_tag, "Connected to Wi-Fi!");
    }
}

void wifi_provisioning_start(void)
{

    // Init TCP/IP stack
    ESP_ERROR_CHECK(esp_netif_init());
    
    // Init event loop
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    // Register handlers early so provisioning events are received
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_PROV_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_event_handler, NULL));
    
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
        ESP_LOGE(wifi_tag, "wifi_prov_mgr_init failed: %s", esp_err_to_name(err));
        return; // or handle recovery
    }

    bool provisioned = false;
    
    ESP_ERROR_CHECK(wifi_prov_mgr_is_provisioned(&provisioned));


    if (!provisioned) {
        ESP_LOGI(wifi_tag, "Starting provisioning");

        // Configura el AP temporal
        wifi_prov_mgr_start_provisioning(WIFI_PROV_SECURITY_1, "abcd1234",
                                         "esp_provisioning_example", NULL);
        // password "abcd1234", SSID "PROV_ESP32"
    } else {
        ESP_LOGI(wifi_tag, "Already provisioned, connecting to Wi-Fi");
        wifi_prov_mgr_deinit();
        wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
        ESP_ERROR_CHECK(esp_wifi_init(&cfg));
        ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
        ESP_ERROR_CHECK(esp_wifi_start());
    }
}
