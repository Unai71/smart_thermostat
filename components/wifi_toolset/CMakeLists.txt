set(COMPONENT_ADD_INCLUDEDIRS include)
set(COMPONENT_SRCS "wifi_scan.c" "wifi_toolset.c")
set(COMPONENT_REQUIRES "esp_wifi" "nvs_flash")
register_component()