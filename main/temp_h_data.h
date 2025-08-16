#ifndef TEMP_H_DATA_H
#define TEMP_H_DATA_H

#include "library_imports.h"
#include "driver/temp_sensor.h"
extern float tsens_out; // Declaración de la variable global

esp_err_t config_temp_sensor();
void read_temp(TimerHandle_t temp_timer);

#endif // TEMP_H_DATA_H