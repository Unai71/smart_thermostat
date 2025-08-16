#include "temp_h_data.h"
extern float tsens_out; 
const char *component_tag = "ESP_TEMP"; 

esp_err_t config_temp_sensor()
{
    // Initialize touch pad peripheral, it will start a timer to run a filter
    ESP_LOGI(component_tag, "Initializing Temperature sensor");
    temp_sensor_config_t temp_sensor = TSENS_CONFIG_DEFAULT();
    temp_sensor_get_config(&temp_sensor);
    ESP_LOGI(component_tag, "default dac %d, clk_div %d", temp_sensor.dac_offset, temp_sensor.clk_div);
    temp_sensor.dac_offset = TSENS_DAC_DEFAULT; // DEFAULT: range:-10℃ ~  80℃, error < 1℃.
    temp_sensor_set_config(temp_sensor);
    temp_sensor_start();
    return ESP_OK; 
}
void read_temp(TimerHandle_t temp_timer)
{
    temp_sensor_read_celsius(&tsens_out);
    ESP_LOGI(component_tag, "Temperature out celsius %f°C", tsens_out);
    
}
