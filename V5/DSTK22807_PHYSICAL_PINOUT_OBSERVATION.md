# DSTK22807 Physical Pinout Observation

This document records a user-observed physical pin label reading for the DSTK22807 ESP32-H2 Super Mini development board.

This is a physical observation, not an official datasheet. It is not final electrical pinout documentation. Do not connect this footprint to schematic nets until the pinout is confirmed again by photo inspection and/or multimeter continuity testing.

## Footprint Reference

- Footprint: `DSTK22807_ESP32H2_SuperMini_Carrier_Measured_THT`
- Footprint pads remain numeric-only.
- Pad 1 is treated as the top-left / negative-X negative-Y pad.
- Pad 1 physically aligns with the observed `TX` label.
- Left row was read by the user from top to bottom.
- Right row was read by the user from bottom to top.

## User Physical Observation

Left row, top to bottom:

| Observed Order | Label |
| --- | --- |
| 1 | TX |
| 2 | RX |
| 3 | 0 |
| 4 | 1 |
| 5 | 2 |
| 6 | 3 |
| 7 | 4 |
| 8 | 5 |
| 9 | 8 |

Right row, bottom to top:

| Observed Order | Label |
| --- | --- |
| 1 | 9 |
| 2 | 10 |
| 3 | 11 |
| 4 | 12 |
| 5 | 13 |
| 6 | 14 |
| 7 | 3V3 |
| 8 | GND |
| 9 | 5V |

## Provisional Footprint Pad Mapping

| Footprint Pad | Observed Label | Confidence | Notes |
| --- | --- | --- | --- |
| 1 | TX | Observed | Anchor point; Pad 1 physically aligns with TX |
| 2 | RX | Observed | Left row, top-to-bottom |
| 3 | 0 | Observed | Left row, top-to-bottom |
| 4 | 1 | Observed | Left row, top-to-bottom |
| 5 | 2 | Observed | Left row, top-to-bottom |
| 6 | 3 | Observed | Left row, top-to-bottom |
| 7 | 4 | Observed | Left row, top-to-bottom |
| 8 | 5 | Observed | Left row, top-to-bottom |
| 9 | 8 | Observed | Left row, top-to-bottom |
| 10 | 5V | Provisional from right-row reversal | Right row was read bottom-to-top by user |
| 11 | GND | Provisional from right-row reversal | Right row was read bottom-to-top by user |
| 12 | 3V3 | Provisional from right-row reversal | Right row was read bottom-to-top by user |
| 13 | 14 | Provisional from right-row reversal | Right row was read bottom-to-top by user |
| 14 | 13 | Provisional from right-row reversal | Right row was read bottom-to-top by user |
| 15 | 12 | Provisional from right-row reversal | Right row was read bottom-to-top by user |
| 16 | 11 | Provisional from right-row reversal | Right row was read bottom-to-top by user |
| 17 | 10 | Provisional from right-row reversal | Right row was read bottom-to-top by user |
| 18 | 9 | Provisional from right-row reversal | Right row was read bottom-to-top by user |

## Safety And Integration Warnings

- This is a physical observation, not an official datasheet.
- Do not connect this footprint to schematic nets until pinout is confirmed again.
- Right row mapping depends on the user statement that the right side was read bottom-to-top.
- Confirm that top-right footprint pad 10 is `5V` and bottom-right footprint pad 18 is `9` before electrical integration.
- Confirm USB side and antenna side physically.
- Confirm which GPIOs are SPI-capable for MCP3208 before assigning SPI nets.
- Do not assume TX/RX are safe for SPI unless explicitly selected later.
- Do not power the system from 5V/USB while electrodes are connected to a human subject.

## Blockers Before Schematic / PCB Integration

- Pinout must be confirmed a second time by clear photo and/or continuity measurement.
- Right-row orientation must be confirmed: bottom-to-top reading is assumed here.
- Top-right pad 10 must be verified as `5V`.
- Bottom-right pad 18 must be verified as `9`.
- USB side and antenna side must be physically confirmed.
- GPIO capability for SPI must be checked before assigning MCP3208 SPI nets.
- Power-domain and electrode-safety behavior must be reviewed before any human-connected test.

## Decision

`PINOUT_OBSERVATION_READY_FOR_REVIEW`
