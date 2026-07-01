# V5 Decisions Log

## 1. Purpose

This file is the central V5 decision log for the EMG project.

It reconstructs the missing `V5/DECISIONS_V5.md` file from the current V5 review documents. It records what is locked, what is provisional, and what must be confirmed before schematic or PCB implementation.

This file is not schematic implementation. This file is not PCB implementation. It does not approve footprint, firmware, gerber, or production output changes by itself.

## 2. Locked Decisions

The following decisions are locked for the current V5 planning direction:

- V4 will be preserved.
- V5 will continue as a separate development line.
- DSTK22807 will be used as a ready-made development board.
- A bare ESP32-H2-MINI module footprint will not be used for the first V5 prototype.
- DSTK22807 will use the measured THT carrier/socket footprint.
- The first prototype will mount the module in a removable header/socket style.
- MCP3208 ADC will be used.
- MCP3208 first prototype package: `MCP3208-CI/P` PDIP-16.
- KiCad MCP3208 symbol: `Analog_ADC:MCP3208`.
- KiCad MCP3208 footprint: `Package_DIP:DIP-16_W7.62mm`.
- MCP3208 TSSOP will not be used unless an exact official MPN and datasheet-backed package/pinout proof are found later.
- `RAW`, `RECT`, and `ENV` analog nodes will be physically produced.
- ADC channel mapping is locked as the V5 planning map:

| MCP3208 Channel | V5 Net |
| --- | --- |
| CH0 | `EMG1_RAW` |
| CH1 | `EMG1_RECT` |
| CH2 | `EMG1_ENV` |
| CH3 | `EMG2_RAW` |
| CH4 | `EMG2_RECT` |
| CH5 | `EMG2_ENV` |
| CH6 | `VREF_MON` |
| CH7 | `BAT_MON` |

- Human electrode safety rule is locked: when electrodes are connected to a human, do not use USB power, 5 V power, bench supply power, mains-connected equipment, or oscilloscope earth connection.

## 3. Provisional Decisions

The following decisions are provisional and must be confirmed before full schematic implementation:

- DSTK22807 Pad 1 = `TX` was observed.
- DSTK22807 observed pinout mapping is recorded as physical observation, not as an official datasheet.
- Current SPI candidate set from `V5/DSTK22807_SPI_PIN_REVIEW.md`:

| SPI Net | ESP32-H2 Candidate | Footprint Pad | Status |
| --- | --- | --- | --- |
| `ADC_CS` | GPIO14 | Pad 13 | Provisional candidate |
| `ADC_SCLK` | GPIO13 | Pad 14 | Provisional candidate |
| `ADC_MOSI` | GPIO12 | Pad 15 | Provisional candidate |
| `ADC_MISO` | GPIO11 | Pad 16 | Provisional candidate |

- MCP3208 `VDD` should preferably be 3.3 V.
- `ADC_REF` is a clean/filtered 3.3 V candidate.
- `analog VREF` is a buffered 1.65 V midscale candidate.
- `analog VCC` is a 3.3 V candidate, but INA/op-amp headroom must be verified.
- `BAT_MON` is planned, but divider/scaling is unresolved.
- `AGND` and `DGND` should be handled with controlled return strategy on a common ground system, not with a blind split.

Note on older photo-derived SPI notes: `V5/DSTK22807_PHOTO_PIN_NOTES.md` contains an earlier photo-derived SPI suggestion using labels `4`, `5`, `10`, and `11`. The newer SPI review uses official ESP32-H2 GPIO capability information and currently supersedes that suggestion for planning. The physical board pinout still needs a second confirmation before any schematic connection.

## 4. Explicit Net Naming Distinctions

These names must stay distinct in schematic review and implementation:

| Net / Name | Meaning | Do not confuse with |
| --- | --- | --- |
| `ADC_REF` | MCP3208 `VREF` pin / ADC full-scale reference. Candidate is clean/filtered 3.3 V. | `analog VREF` |
| `analog VREF` | 1.65 V midscale bias/reference for the analog front-end. Should be buffered. | `ADC_REF` |
| `VREF_MON` | MCP3208 ADC channel that measures `analog VREF`. Planned on CH6. | MCP3208 `VREF` pin |
| `3V3` | Main regulated 3.3 V system rail, source ownership still TBD. | DSTK22807 module-only 3.3 V assumption |
| `3V3_ADC` | Quiet/local ADC supply branch if filtering is used. | A separate isolated ground domain |
| `analog VCC` | Analog front-end supply candidate, currently 3.3 V pending headroom validation. | `analog VREF` |
| `AGND` | Analog return naming/placement intent on the common ground system. | A blindly split ground island |
| `DGND` | Digital return naming/placement intent on the common ground system. | A floating separate ground |

## 5. DSTK22807 Status

- Measured footprint was created.
- The footprint was paper-tested for mechanical planning.
- Silkscreen cleanup was completed.
- Footprint is geometry-approved for mechanical planning.
- Electrical pinout remains provisional.
- USB side still needs physical confirmation.
- Antenna side still needs physical confirmation.
- Right-row reversal still needs confirmation:
  - Top-right footprint pad 10 must be `5V`.
  - Bottom-right footprint pad 18 must be `GPIO9`.
- Footprint pads remain numeric-only; signal labels should not be embedded into the geometry-only footprint.
- Do not connect the DSTK22807 footprint to schematic nets until physical pinout is confirmed again.
- DSTK22807 power pin measurement is documented in `V5/DSTK22807_POWER_PIN_MEASUREMENT.md`.
- USB powered measurement observed 5V = 5.126 V and 3V3 = 3.291 V.
- Unpowered measurement observed 5V = 0 V and 3V3 rail discharge over time: 0.590 V after 10 s, 0.400 V after 30 s, and 0.250 V after 60 s.
- The 3V3 decay is consistent with residual capacitor discharge / leakage path.
- 3V3 external powering remains **NOT APPROVED YET**.
- USB plus external 3.3 V simultaneous connection remains **NOT APPROVED**.
- 5V pin as analog/ADC supply remains **NOT APPROVED**.

Current observed pad mapping from `V5/DSTK22807_PHYSICAL_PINOUT_OBSERVATION.md`:

| Footprint Pad | Observed Label | Status |
| --- | --- | --- |
| 1 | TX | Observed |
| 2 | RX | Observed |
| 3 | GPIO0 | Observed |
| 4 | GPIO1 | Observed |
| 5 | GPIO2 | Observed |
| 6 | GPIO3 | Observed |
| 7 | GPIO4 | Observed |
| 8 | GPIO5 | Observed |
| 9 | GPIO8 | Observed |
| 10 | 5V | Provisional from right-row reversal |
| 11 | GND | Provisional from right-row reversal |
| 12 | 3V3 | Provisional from right-row reversal |
| 13 | GPIO14 | Provisional from right-row reversal |
| 14 | GPIO13 | Provisional from right-row reversal |
| 15 | GPIO12 | Provisional from right-row reversal |
| 16 | GPIO11 | Provisional from right-row reversal |
| 17 | GPIO10 | Provisional from right-row reversal |
| 18 | GPIO9 | Provisional from right-row reversal |

## 6. MCP3208 Status

- `MCP3208-CI/P` PDIP-16 is recommended for the socketed first prototype.
- `MCP3208-CI/SL` SOIC-16 is a valid compact alternative after DIP bring-up.
- KiCad symbol `Analog_ADC:MCP3208` was reviewed against the official MCP3208 pinout.
- KiCad DIP footprint `Package_DIP:DIP-16_W7.62mm` was reviewed for the PDIP first prototype.
- MCP3208 should preferably be powered at 3.3 V for direct ESP32-H2 SPI compatibility.
- If MCP3208 is powered at 5 V, `DOUT` level shifting is required before ESP32-H2.
- MCP3208 `VREF` is the ADC full-scale reference and must not be merged with the 1.65 V analog midscale bias without an explicit architecture decision.
- MCP3208-CI/P is the C-grade accuracy version. It is acceptable for first EMG prototype bring-up, but B-grade alternatives such as `MCP3208-BI/P` or `MCP3208-BI/SL` can be considered if accuracy margin becomes critical.

## 7. Power / Reference Status

- Battery-powered first prototype is preferred.
- External low-noise 3.3 V regulator is preferred.
- External 3.3 V regulator remains preferred for analog/ADC.
- Do not assume DSTK22807 `3V3` pin can power the whole analog/ADC system.
- DSTK22807 3V3 power direction remains unresolved.
- Do not back-feed DSTK22807 until board behavior is verified.
- Do not back-feed DSTK22807 3V3 until power path is verified.
- For human-connected EMG testing, USB Type-C must be disconnected and the system must be battery-powered/isolated.
- `ADC_REF` and `analog VREF` are separate.
- `analog VREF` buffer is required or strongly preferred.
- `BAT_MON` is unresolved.
- ADC input protection is unresolved.
- Analog headroom/gain at 3.3 V is unresolved.
- ADC input range protection must ensure CH0-CH7 never exceed VSS to VREF in single-ended MCP3208 use.
- Proper MCP3208 `VDD` decoupling, `VREF` decoupling, and AGND/DGND handling are required in the later schematic/layout.

## 8. Open Blockers Before Schematic Implementation

The following blockers must be closed before full schematic implementation:

- Physical DSTK22807 pinout second confirmation.
- Right-row reversal confirmation.
- USB side and antenna side confirmation.
- DSTK22807 `3V3` / `5V` power behavior verification.
- DSTK22807 3V3 external input safety.
- DSTK22807 3V3 current capability.
- USB and external 3.3 V coexistence / backfeed behavior.
- Battery regulator architecture.
- Battery chemistry and maximum voltage.
- 3.3 V regulator part/current/noise selection.
- INA333/op-amp headroom at 3.3 V.
- `analog VREF` buffer topology/part.
- MCP3208 ADC input protection and source impedance/acquisition requirements.
- `BAT_MON` divider/scaling.
- `AGND` / `DGND` grounding strategy.
- Human-test safety procedure.

## 9. Should schematic implementation proceed?

Full schematic implementation: **NO**.

Documentation/planning: **YES**.

3V3 external powering of DSTK22807: **NOT APPROVED YET**.

USB-powered human EMG testing: **FORBIDDEN**.

5V pin as analog/ADC supply: **NOT APPROVED**.

The next small step should be battery/regulator architecture planning and DSTK22807 power-path verification, not schematic capture.

## 10. Recommended Next Action

The component-level power/reference candidate review has already been documented in `V5/POWER_REFERENCE_COMPONENT_CANDIDATES.md`.

Next recommended action:

- Plan the battery/regulator architecture and DSTK22807 power-path verification.
- Keep external 3.3 V powering of DSTK22807 as **NOT APPROVED YET**.
- Do not connect USB Type-C and external 3.3 V at the same time unless power-path safety is proven.
- Do not start schematic power implementation yet.
- Continue with documentation/planning only until power-path, regulator, analog headroom, ADC protection, BAT_MON, grounding, and human-safety blockers are closed.

## Source Documents Used

- `V5/DSTK22807_PHYSICAL_PINOUT_OBSERVATION.md`
- `V5/DSTK22807_SPI_PIN_REVIEW.md`
- `V5/MCP3208_MPN_SYMBOL_FOOTPRINT_REVIEW.md`
- `V5/POWER_REFERENCE_ARCHITECTURE_REVIEW.md`
- `V5/BOM_V5_DRAFT.md`
- `V5/STATUS_V5.md`
- `V5/DSTK22807_MEASUREMENT_CHECKLIST.md`
- `V5/DSTK22807_PHOTO_PIN_NOTES.md`
- `V5/DSTK22807_POWER_PIN_MEASUREMENT.md`
- `V5/POWER_REFERENCE_COMPONENT_CANDIDATES.md`

Final decision: **DECISIONS_POWER_MEASUREMENT_UPDATE_READY_FOR_REVIEW**





