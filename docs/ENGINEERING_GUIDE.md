# Verdant: understand the design

This guide explains the **new reference implementation**, not recovered firmware from the photograph. Hardware commissioning is still pending. Use the [real demonstration plan](DEMO_TEST_PLAN.md) to turn a prototype into evidence.

## Explain the system in your own words

The objective is to request watering when the chosen policy permits it, stop that request predictably, and refuse to arm after invalid sensing. The owner reports a solar-supported power arrangement; the panel, charger and pump are not visible in the supplied photograph. **The sketch does not charge batteries.**

| Element | Intended role | What still needs confirmation |
| --- | --- | --- |
| Exposed-electrode probe | Supply a soil-dependent electrical input | Interface, supply and dry/wet calibration |
| Uno-format board | Run the control policy | Actual MCU and board identity |
| Cell holder and wiring | Form part of the power arrangement | Chemistry, ratings, protection and connections |
| Pump/driver | Deliver water when commanded | Owner-reported; model and driver not identified |
| Panel/charger | Support the energy arrangement | Owner-reported; compatibility and charge behavior unverified |

Do not invent a historical reason for selecting a part. The reference compiles for an Uno because it provides the Arduino interfaces used by the sketch; that is **not** identification of the photographed board or a claim that it is the best choice.

## Read the code in this order

| Source | Follow these names | Meaning |
| --- | --- | --- |
| [config.h](../firmware/plant_care/config.h) | `ENABLE_SOIL_INPUT`, `ENABLE_PUMP` | Both physical paths start disabled; unknown GPIOs remain unset. |
| [irrigation.h](../firmware/plant_care/irrigation.h) | `moisturePercent`, `Config::valid` | Calibration mapping and policy validation. A mapped percentage is not laboratory water content. |
| [irrigation.h](../firmware/plant_care/irrigation.h) | `Controller::tick`, `arm`, `resetFault` | State transitions, arming, timing and recovery. |
| [plant_care.ino](../firmware/plant_care/plant_care.ino) | `setup`, `loop`, `writePump` | Serial input, optional adapters and status output. |
| [test_core.cpp](../tests/test_core.cpp) | `shortConfig`, `CHECK` | Actual portable logic tests with explicitly shortened test timing. |

## Understand the policy before changing it

1. Boot is **disarmed**. A healthy sample is required before `arm()` can succeed.
2. The default `TimerAndMoisture` mode requires **both** a due interval and a dry reading. Dry soil alone does not bypass the interval.
3. The reference uses start/stop hysteresis, a soak delay and a maximum run time. These prevent rapid policy toggling and bound a request; they do not prove safe water delivery.
4. Every mode, including `Timer`, requires numerically valid sensing and observes the wet stop threshold.
5. An invalid sample latches a fault and disarms. Recovery requires a valid sample, fault reset, then manual arming.
6. A reboot restarts the relative timer. There is no real-time calendar or preserved state.

Reference defaults are 35/65%, a six-hour interval, one-minute soak and five-second run limit. They are **software examples, not recommendations for a plant or pump**. The run limit is cooperative software, not an independent electrical cutoff.

## Build without operating hardware

From the repository root, with AVR core 1.8.7 installed:

```powershell
arduino-cli compile --fqbn arduino:avr:uno firmware/plant_care
```

Compilation does not flash a board. The default configuration leaves sensing/output disabled. See [FIRMWARE.md](../FIRMWARE.md) for the dependency and commissioning boundaries. Default and private adapter-only compilation both passed again on 16 October 2026; no device was flashed or exercised.

## Three useful learning exercises

- Trace the test that starts at time 100, then stops at 105 with a shortened configuration. Explain why it says nothing about millilitres of water.
- In a separate **software-only** experiment, compare `Moisture`, `Timer` and `TimerAndMoisture`. Predict their behavior before running the existing tests. Keep hardware adapters disabled.
- Add a logic test for a plausible but faulty sensor value. Explain why rejecting ADC rails cannot detect every disconnected probe.

Never weaken OS execution policy to run a test. Record a blocked execution separately from a compilation failure.

## Troubleshooting to demonstrate understanding

| Observation | Check first |
| --- | --- |
| Dry sample but no request | Armed state, combined-mode interval and soak delay |
| Arm rejected after a fault | Healthy sample, then reset, then arm |
| Dry/wet behavior reversed | Measured calibration orientation, not jumper colors |
| Request reported but no water | Software request is not driver current or pump flow; identify the actual electrical path before testing |
| Solar claim cannot be reproduced | Panel, charger, storage and load evidence are separate from this sketch |

Before presenting the project, explain which hardware you assembled, which code is new reference work, and what you have actually measured. Existing results remain in [BUILD_STATUS.md](../BUILD_STATUS.md); do not describe a planned demonstration as completed.
