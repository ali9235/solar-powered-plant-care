#pragma once
// New reference design. These switches do not describe the photographed wiring.
#define ENABLE_SOIL_INPUT 0
#define SOIL_PIN -1
#define DRY_ADC -1
#define WET_ADC -1
#define ENABLE_PUMP 0
#define PUMP_PIN -1
#define PUMP_DRIVER_VERIFIED 0
#define PUMP_ACTIVE_HIGH 1
#if ENABLE_SOIL_INPUT && (SOIL_PIN < 0 || DRY_ADC < 0 || WET_ADC < 0 || DRY_ADC == WET_ADC)
#error "Configure the actual analog pin and measured dry/wet ADC calibration first"
#endif
#if ENABLE_PUMP && (!ENABLE_SOIL_INPUT || PUMP_PIN < 0 || !PUMP_DRIVER_VERIFIED)
#error "Pump disabled until sensing, driver, pin, polarity and supply are verified"
#endif
#if ENABLE_PUMP && PUMP_PIN == SOIL_PIN
#error "Pump output and soil input cannot share a pin"
#endif
