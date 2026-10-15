# New reference firmware

**Newly authored for this portfolio—not recovered original firmware.** No pump, solar charger, battery, or moisture probe was actuated/tested by this implementation task. The original photograph remains evidence of the owner's physical prototype, not evidence that this new code ran on it.

## Included code

- [plant_care.ino](firmware/plant_care/plant_care.ino): serial controls, analog adapter, optional pump output, periodic status.
- [irrigation.h](firmware/plant_care/irrigation.h): portable, hardware-independent control logic.
- [config.h](firmware/plant_care/config.h): deliberately disabled inputs/output and unset pins/calibration.
- [Core tests](tests/test_core.cpp): executable tests of the actual C++ control logic.

## Implemented software behavior

Boots disarmed. Supports moisture-only, timer-only, and timer-AND-moisture modes. The **new default** is timer AND dry soil; this is not a claim about how the original prototype combined its modes. Every mode requires a numerically valid moisture input and stops at the wet threshold. Watering is bounded by a maximum run time and a soak period. Invalid input latches a fault, disarms, and requires valid input, a fault reset, and manual re-arming. Timer arithmetic is tested across 32-bit millisecond rollover.

Example settings in `irrigation.h`: start 35%, stop 65%, six-hour interval, one-minute soak, five-second maximum run. These are **reference policy values, not plant-care recommendations or measured watering amounts**. Percentages are a linear calibration indicator, not laboratory volumetric water content. ADC rail readings are rejected; a disconnected/faulty sensor can still produce plausible values, so this is not a proven disconnection detector.

No real-time clock, date scheduler, nonvolatile state, tank-level sensor, current feedback, dry-run pump detection, or solar charging regulation is implemented. A reboot disarms and restarts the relative timer. Serial processing is capped at 16 characters per loop to prevent an unbounded input loop. Run-time enforcement is cooperative software, not an independent hardware cutoff or watchdog. Do not deploy it unattended based on software tests alone.

## Build and dry-run

Reference compilation target: Arduino Uno, **not confirmed as the exact photographed board**. With Arduino AVR core 1.8.7 installed:

```powershell
arduino-cli compile --fqbn arduino:avr:uno firmware/plant_care
```

Default configuration needs no external Arduino library and configures no pump/sensor GPIO; UART serial is still used. If you later choose to flash a verified board, serial controls at 115200 baud are `d` simulated dry, `w` simulated wet, `m` middle, `f` invalid, `a` arm, `x` stop/disarm, `r` reset fault. After a fault: send a valid simulated sample, allow the loop to process it, reset, then arm. The six-hour default interval still applies; automated core tests use shorter **test-only** intervals.

To run standalone core tests with a host C++ compiler:

```powershell
g++ -std=c++17 -Wall -Wextra -Werror tests/test_core.cpp -o test_core.exe
.\test_core.exe
```

MSVC is also supported through a Developer Command Prompt. A successful compile is not a hardware test.

## Hardware enablement gate

Keep `ENABLE_PUMP=0` until the actual board, analog input limits, dry/wet calibration, driver, active polarity, pump voltage/current, switching protection, grounds, and supply are established. Fill the **new design's** pin map only after checking the actual build; no photograph-derived GPIOs are claimed. Compile-time checks reject some incomplete/conflicting settings but do not validate electrical safety.

Solar charging belongs to a compatible hardware charging/protection circuit selected for the actual battery chemistry and panel. This sketch must not be treated as a battery charger. See [the electrical checklist](WIRING_AND_SOURCE.md).
