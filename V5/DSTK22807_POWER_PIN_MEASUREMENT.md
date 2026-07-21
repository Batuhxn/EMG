# DSTK22807 Power-Pin Measurement Record

This document preserves the reported physical voltage observations and states how the current project uses them. It does not add new measurements or approve external powering/backfeeding.

## Measurement setup recorded by the project

| Measurement | USB Type-C | External power | Reference |
| --- | --- | --- | --- |
| Powered observation | Connected | Not connected | DSTK22807 GND header pin |
| Post-power-removal decay | Disconnected | Disconnected | DSTK22807 GND header pin |

## Recorded observations

| Condition | Pin / rail | Reported voltage |
| --- | --- | --- |
| USB connected, no external battery/regulator | 5V header pin | 5.126 V |
| Powered carrier observation | 3V3 header pin | Approximately 3.291–3.31 V |
| USB and external power disconnected | 5V header pin | 0 V |
| Power removed for 10 s | 3V3 header pin | 0.590 V |
| Power removed for 30 s | 3V3 header pin | 0.400 V |
| Power removed for 60 s | 3V3 header pin | 0.250 V |

The decay samples are consistent with residual charge and leakage paths; they are not evidence of an actively powered rail.

## Accepted project mapping

- U501 pad 10 = `5V_VBUS`.
- U501 pad 11 = GND.
- U501 pad 12 = carrier `3V3`, named `CARRIER_3V3` at the SPI isolation boundary.

This mapping is authoritative project evidence and is no longer provisional or an SPI-isolation implementation blocker.

## What the evidence establishes

- The carrier exposes its 5V rail on pad 10 during USB-powered operation.
- The carrier exposes approximately 3.3 V on pad 12 during powered operation.
- GND is available on pad 11 and was used as the measurement reference.
- Residual voltage remains on carrier 3V3 after power removal and decays over time.

## What remains unproven

- Safe external 3V3 injection into the carrier.
- Carrier 3V3 current capability.
- USB and external-power coexistence.
- Reverse-current and backfeed behavior.
- Exact carrier regulator topology.

The present battery architecture therefore powers U501 through pad 10 / `5V_VBUS`. Pad 12 is monitored as `CARRIER_3V3`; it does not power `3V3_ADC`.

## Required future bench work

- Carrier startup, peak current, and RF-burst rail behavior.
- Carrier-on/ADC-off and ADC-on/carrier-off SPI-interface tests.
- Asymmetric collapse and residual-charge tests.
- Inactive-rail rise, pin-relative voltage, aggregate leakage, and phantom-power checks.

## Safety boundary

Human-connected acquisition requires battery-only operation with USB physically absent and no mains-referenced instrumentation. The TXU0304 interface is not a patient-protection barrier.
