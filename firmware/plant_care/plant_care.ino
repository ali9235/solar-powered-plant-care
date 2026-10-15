#include "config.h"
#include "irrigation.h"

plant::Config settings;
plant::Controller watering(settings);
int simulatedMoisture = 50;
uint32_t lastReport = 0;

void writePump(bool requested) {
#if ENABLE_PUMP
  digitalWrite(PUMP_PIN, requested == bool(PUMP_ACTIVE_HIGH) ? HIGH : LOW);
#else
  (void)requested;  // A dry-run request never touches a GPIO.
#endif
}

void setup() {
  Serial.begin(115200);
#if ENABLE_PUMP
  digitalWrite(PUMP_PIN, PUMP_ACTIVE_HIGH ? LOW : HIGH);
  pinMode(PUMP_PIN, OUTPUT);
#endif
  watering.begin(millis());
  Serial.println(F("NEW REFERENCE FIRMWARE. Disarmed on boot. Not field validated."));
#if !ENABLE_SOIL_INPUT
  Serial.println(F("SIMULATED soil: d=dry, w=wet, m=middle, f=invalid."));
#endif
  Serial.println(F("a=arm, x=stop/disarm, r=reset fault. Combined timer AND dry-soil mode."));
}

void loop() {
  const uint32_t now = millis();
  uint8_t commands = 0;
  while (Serial.available() && commands++ < 16) {
    const char c = char(Serial.read());
#if !ENABLE_SOIL_INPUT
    if (c == 'd') simulatedMoisture = 20;
    if (c == 'w') simulatedMoisture = 80;
    if (c == 'm') simulatedMoisture = 50;
    if (c == 'f') simulatedMoisture = -1;
#endif
    if (c == 'x') { watering.disarm(now); writePump(false); }
    if (c == 'r') watering.resetFault();
    if (c == 'a' && !watering.arm()) Serial.println(F("ARM rejected: valid sensing and fault reset required."));
  }
#if ENABLE_SOIL_INPUT
  const int percent = plant::moisturePercent(analogRead(SOIL_PIN), DRY_ADC, WET_ADC);
#else
  const int percent = simulatedMoisture;
#endif
  watering.tick(now, percent);
  writePump(watering.pumpRequested());
  if (uint32_t(now - lastReport) >= 1000UL) {
    lastReport = now;
    Serial.print(F("moisture=")); Serial.print(percent);
    Serial.print(F(" armed=")); Serial.print(watering.armed());
    Serial.print(F(" fault=")); Serial.print(watering.fault());
    Serial.print(F(" pump_request=")); Serial.print(watering.pumpRequested());
    Serial.print(F(" physical_output_enabled=")); Serial.println(ENABLE_PUMP);
  }
  delay(10);
}
