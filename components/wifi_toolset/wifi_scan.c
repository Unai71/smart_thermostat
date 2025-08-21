#include "wifi_scan.h"

const char *scan_tag = "wifi_scan"; // Define the scan tag for logging

/* Initialize Wi-Fi for scanning */
esp_err_t wifi_scan_init(void)
{
    esp_err_t ret;

    // Initialize the TCP/IP stack
    ret = esp_netif_init();
    if (ret != ESP_OK) {
        ESP_LOGE(scan_tag, "Failed to initialize network interface: %s", esp_err_to_name(ret));
        return ret;
    }

    // Create the default event loop
    ret = esp_event_loop_create_default();
    if (ret != ESP_OK) {
        ESP_LOGE(scan_tag, "Failed to create default event loop: %s", esp_err_to_name(ret));
        return ret;
    }

    // Initialize Wi-Fi
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ret = esp_wifi_init(&cfg);
    if (ret != ESP_OK) {
        ESP_LOGE(scan_tag, "Failed to initialize Wi-Fi: %s", esp_err_to_name(ret));
        return ret;
    }

    // Set Wi-Fi mode to station
    ret = esp_wifi_set_mode(WIFI_MODE_STA);
    if (ret != ESP_OK) {
        ESP_LOGE(scan_tag, "Failed to set Wi-Fi mode: %s", esp_err_to_name(ret));
        return ret;
    }

    // Start Wi-Fi
    ret = esp_wifi_start();
    if (ret != ESP_OK) {
        ESP_LOGE(scan_tag, "Failed to start Wi-Fi: %s", esp_err_to_name(ret));
        return ret;
    }

    ESP_LOGI(scan_tag, "Wi-Fi initialized successfully for scanning.");
    return ESP_OK;
}

/* Perform a Wi-Fi scan and log the results */
esp_err_t wifi_scan_list_aps(void)
{
    esp_err_t ret;

    // Configure the scan
    wifi_scan_config_t scan_config = {
        .ssid = NULL,          // Scan all SSIDs
        .bssid = NULL,         // Scan all BSSIDs
        .channel = 0,          // Scan all channels
        .show_hidden = true,   // Include hidden networks
        .scan_type = WIFI_SCAN_TYPE_ACTIVE,
        .scan_time = {
            .active = {
                .min = 100,    // Minimum active scan time per channel (ms)
                .max = 300,    // Maximum active scan time per channel (ms)
            },
        },
    };

    // Start the scan
    ESP_LOGI(scan_tag, "Starting Wi-Fi scan...");
    ret = esp_wifi_scan_start(&scan_config, true);
    if (ret != ESP_OK) {
        ESP_LOGE(scan_tag, "Failed to start Wi-Fi scan: %s", esp_err_to_name(ret));
        return ret;
    }

    // Get the number of access points found
    uint16_t ap_count = 0;
    ret = esp_wifi_scan_get_ap_num(&ap_count);
    if (ret != ESP_OK) {
        ESP_LOGE(scan_tag, "Failed to get AP count: %s", esp_err_to_name(ret));
        return ret;
    }

    if (ap_count == 0) {
        ESP_LOGI(scan_tag, "No access points found.");
        return ESP_OK;
    }

    // Retrieve the AP records
    wifi_ap_record_t ap_records[ap_count];
    ret = esp_wifi_scan_get_ap_records(&ap_count, ap_records);
    if (ret != ESP_OK) {
        ESP_LOGE(scan_tag, "Failed to get AP records: %s", esp_err_to_name(ret));
        return ret;
    }

    // Log the SSID, RSSI, and other details of each AP
    ESP_LOGI(scan_tag, "Found %d access points:", ap_count);
    for (int i = 0; i < ap_count; i++) {
        ESP_LOGI(scan_tag, "SSID: %s, RSSI: %d, Channel: %d, Authmode: %d",
                 ap_records[i].ssid,
                 ap_records[i].rssi,
                 ap_records[i].primary,
                 ap_records[i].authmode);
    }

    return ESP_OK;
}
