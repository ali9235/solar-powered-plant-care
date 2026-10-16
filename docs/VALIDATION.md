# VOLTRA Verdant: testing and validation

## Scope and actual results

[Machine-readable software-check record](../tests/evidence/2026-10-16-software-checks.json) · This record contains actual software checks and explicit exclusions, not fabricated sensor data.

45 historical C++ checks cover calibration, modes, hysteresis, faults and timing. Default and adapter-only Arduino Uno builds are compilation checks.

Default and private adapter-only sketches compiled successfully again on 16 October 2026. Fixture pins are compiler inputs, **not wiring recommendations**. Native C++ tests were not rerun in this documentation update; application-control restrictions were respected. Historical results remain in [BUILD_STATUS.md](../BUILD_STATUS.md).

## Reproduce software checks

```powershell
python tools/check_docs.py
arduino-cli compile --fqbn arduino:avr:uno firmware/plant_care
# With an approved native compiler/runtime:
g++ -std=c++17 -Wall -Wextra -Werror tests/test_core.cpp -o test_core.exe
.\test_core.exe
```

The documentation check needs only Python's standard library. Install dependencies from [FIRMWARE.md](../FIRMWARE.md). Compiling does not flash. If the OS blocks a native executable, record the blocker; do not rename, rebuild or move it to bypass policy.

## Error behavior to inspect

Invalid sensing disarms and latches a fault. Healthy input, explicit reset and re-arming are required before another request; that software behavior does not establish electrical shutdown.

## Hardware acceptance

No unattended travel trial, delivered water volume, battery runtime, charge efficiency or physical pump actuation was measured.

The [demo plan](DEMO_TEST_PLAN.md) defines proposed observations. [hardware-observations.csv](../tests/hardware-observations.csv) contains **not_run** rows, not measurements. Add raw values and genuine evidence only after testing.

## Continuous integration

The proposed workflow runs documentation checks and the portable C++ core on Ubuntu. It does not flash hardware or compile Arduino adapter branches, assess photo truth or establish safety. A workflow file is not a passing run; inspect Actions after an authorized push. No success badge is prefilled.

[Security](../SECURITY.md) · [Roadmap](ROADMAP.md)
