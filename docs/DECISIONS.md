# VOLTRA Verdant: engineering decisions

These explain the **current inspectable design/documentation**, not why the owner originally bought components. Original selection history and personal debugging experiences have not been supplied.

| Decision | Current-file rationale | Alternative / trade-off | Constraint |
| --- | --- | --- | --- |
| Separate policy from GPIO | Test the portable controller without energizing a pump. | A monolithic sketch is initially simpler but couples logic to hardware. | Core checks cannot validate plumbing, driver or supply. |
| Disarmed boot and fault latch | Avoid silently requesting watering after invalid sensing or reboot. | Automatic resume is convenient but can restart a bad condition. | Manual recovery is required; software limits are not safety-rated. |
| Timer AND moisture default | Require both conditions rather than equating elapsed time with water demand. | Timer-only and moisture-only modes also exist. | Calibration and probe placement still determine percentage meaning. |

## Challenges: distinguish observed from suggested

The tests and configuration guards establish specific reference-software behavior, not proof that those failures occurred on the owner's prototype. The original matching sketch was not recovered.

No unattended travel trial, delivered water volume, battery runtime, charge efficiency or physical pump actuation was measured.

Troubleshooting suggestions are hypotheses, not incidents already solved. Add a case study only with the observed symptom, reproducible conditions, evidence for the cause, exact change, before/after test and remaining uncertainty. Do not turn design intentions into measured improvements.

[Architecture](ARCHITECTURE.md) · [Validation record](VALIDATION.md)
