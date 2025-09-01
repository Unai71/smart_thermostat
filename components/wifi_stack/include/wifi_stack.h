#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "esp_log.h"

#include "wifi_provisioning/manager.h"
#include "wifi_provisioning/scheme_softap.h"   // o scheme_ble si prefieres BLE

void wifi_provisioning_start(void);
