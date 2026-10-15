# Reproduction checklist — original design needed

The owner confirms moisture-based watering, timed watering, solar-supported charging, and the photograph-to-project association. These confirmations describe the design; they do not establish a pin map or an observed test result.

| Connection / parameter | Current status | Needed before assembly instructions |
| --- | --- | --- |
| Controller | Arduino / Uno-format board partially visible | Exact board and MCU |
| Moisture probe → controller | Probe/wires visible | Actual signal pin, interface module, power rail, dry/wet calibration |
| Timing | Owner confirms timed operation | Software interval, RTC, or external timer; settings and restart behavior |
| Controller → water actuator | Not visible | Pump/valve model, driver, actual signal pin and supply |
| Solar panel → charging circuit | Owner-reported solar charging | Panel ratings, charger model and actual terminals |
| Charging / energy storage | Battery holder visible | Cell chemistry/count, protection, charger compatibility and wiring |
| Grounds and power rails | Not traceable from photo | Complete connection table or clear schematic |
| Timer / moisture interaction | Not established | Mode selection or precise combined rule |

No original GPIO map, charging circuit, pump-driver circuit, or voltage specifications are guessed. The owner has now requested [new reference firmware](FIRMWARE.md); its disabled/unset configuration is not an identification of the original circuit. Actual electrical design and calibration remain required.

## Useful proof of operation

## Detailed commissioning sequence

1. **Inventory before power:** read the controller marking, probe/interface part number, pump rating, solar-panel rating, battery chemistry/count, and charger/protection model. The photograph does not establish these specifications.
2. **Draw the power paths:** separately identify controller supply, pump supply, and panel/charger/storage connections. Do not connect a panel directly to unknown cells or treat this program as a charge regulator. Use the identified charger manufacturer's wiring and battery-compatibility instructions.
3. **Soil input:** trace the probe's actual interface to its signal output and ground. Confirm whether it is analog before using this reference driver. Connect its output only through an electrically compatible interface to the chosen analog input; enter that real input in `SOIL_PIN`. Do not assume a raw two-electrode probe can be connected as a complete powered sensor module.
4. **Calibrate:** with the actual sensor supply and ADC range established, record dry and wet reference ADC values. Enter them in `DRY_ADC` and `WET_ADC`; this firmware expects a 10-bit reference ADC. Rail readings are rejected. Inspect repeated samples before enabling control.
5. **Pump control:** use the actual rated switching driver, not a pump directly on a GPIO. Establish load supply, control signal, protection required by the load/driver, grounding or isolation, and active polarity. Keep the pump disconnected while testing inactive/active logic. Enter the verified output in `PUMP_PIN` and its polarity in `PUMP_ACTIVE_HIGH`.
6. **Timing policy:** choose `Mode::Moisture`, `Mode::Timer`, or `Mode::TimerAndMoisture` in the new `Config`. Configure thresholds, interval, soak and maximum run time for a supervised test. They are example settings—not a plant-specific watering recommendation. This is a relative software timer, not a clock/calendar schedule.
7. **Enable in stages:** first retain the all-disabled default and run serial simulation/tests. Then configure and enable `ENABLE_SOIL_INPUT` only. Verify the calibrated reading before enabling `ENABLE_PUMP` and `PUMP_DRIVER_VERIFIED`; these flags are acknowledgements of work you must actually do, not automatic electrical checks.
8. **First physical run:** use a controlled test area, short supervised cycle, accessible power disconnect and a container for water. Verify boot-off, manual stop, wet cutoff, elapsed-time cutoff, invalid readings and restart behavior. Inspect for leaks and overheating. Do not leave it unattended based on a compile pass.

| New-code configuration | Actual connection or observation required |
| --- | --- |
| `SOIL_PIN` | Confirmed analog input, not an assumed photograph pin |
| `DRY_ADC` / `WET_ADC` | Measured calibration values with the actual analog interface |
| `PUMP_PIN` | Confirmed driver-control GPIO, not the pump power lead |
| `PUMP_ACTIVE_HIGH` | Measured inactive/active driver polarity |
| Solar charger terminals | Hardware-specific manufacturer's schematic; not controlled by this firmware |

No physical pin-to-pin circuit is asserted until the actual components and wiring are identified. The steps above explain how to complete the connection record safely; compiler fixture pins are never a substitute for it.

## Demonstration record

Record a moisture-triggered watering event and a timer-triggered event separately. Document the actual power source and show the charging arrangement. If measuring energy or delivered water, retain raw readings, instruments, duration, and conditions. Test missing-sensor behavior and pump run limits before any unattended operation. These are proposed tests—not claimed features or completed results.
