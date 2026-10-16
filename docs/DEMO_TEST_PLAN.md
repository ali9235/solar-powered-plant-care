# Verdant: record a real demonstration

**Status: planned, not performed.** This is a recording and measurement procedure, not a demo video or proof of unattended reliability.

## Safety and identification gate

Complete [WIRING_AND_SOURCE.md](../WIRING_AND_SOURCE.md) first. Identify the actual board, probe interface, pump, driver, supply, battery chemistry and charging/protection hardware. Leave pump output disabled until verified. Do not connect a solar panel directly to unidentified cells, modify live wiring, or leave this prototype unattended. Keep water away from exposed electronics.

Compiler fixture pins are not a wiring diagram. Start with the pump disconnected and a software-only review. Hardware enablement, flashing and powering require a separate deliberate commissioning decision.

## A useful 60–90 second recording

1. Show the real assembly, component markings and the exact code revision. State whether this is the original program or new reference software.
2. Show the selected policy and calibration record. Clearly label any serial-driven sample **SIMULATION**.
3. After safe commissioning, show the actual trigger condition, control request and real water output in one continuous shot. Keep the driver and pump distinguishable from a status message.
4. Show stopping behavior and explain a limitation. If only simulation is possible, say so and do not film it as automatic physical watering.

Do not shorten the six-hour interval invisibly. Any changed test interval must appear in the recording and log. Replayed, accelerated or edited footage must be disclosed.

## Bench cases to record

| ID | Test | Evidence needed | Acceptance basis |
| --- | --- | --- | --- |
| V1 | Calibration review | Raw dry/wet ADC observations and actual interface details | Endpoints distinct and valid for the verified input; no claim of absolute soil-water percentage |
| V2 | Policy / start-stop | Mode, timing, arming and timestamped request transitions | Matches the displayed test configuration; wet input stops a request |
| V3 | Fault and recovery | Explicit invalid-input simulation, disarm, reset and re-arm sequence | No request during fault; no automatic recovery presented as manual recovery |
| V4 | Water delivery | Supervised duration and collected volume, with method stated | Compare to a tolerance chosen before testing; no invented flow figure |
| V5 | Solar power path | Identified ratings, compatible charger and appropriate measured input/output evidence | Evaluate the actual charger specification; not a firmware test or battery-life estimate |

Use [hardware-observations.csv](../tests/hardware-observations.csv). Every row starts `not_run`. Preserve raw readings, time/configuration, reference method and evidence filename. Publish only completed observations; leave unperformed cases visibly unperformed.

## Explain the result, even if it fails

Answer: why is the pump a separate power load, why is hysteresis useful, why does a restart affect the schedule, and what does the software fail to observe? A convincing result includes the failure or limit—not a promise that plants will survive a trip.
