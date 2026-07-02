# V5 First Schematic Power Source Strategy

## 1. Current repo state

| Item | State |
| --- | --- |
| Branch | `v5/measured-dstk22807-footprint` |
| HEAD | `9707dd5 update decisions with DSTK 3V3 current budget review` |
| Git status | Clean before writing this document |
| Remote tracking | `github-emg/v5/measured-dstk22807-footprint` |
| Scope | Documentation-only power-source strategy |
| KiCad implementation | No KiCad implementation in this task |

## 2. Purpose

This document selects the power-source strategy for the first small V5 power/reference schematic block.

This document is not a schematic. This document is not a PCB layout. This document does not approve DSTK22807 onboard `3V3` use as a final analog/ADC source. This document does not make the external analog/ADC LDO mandatory.

The goal is to preserve flexibility while preventing accidental rail tying, backfeed, or premature power-architecture lock-in.

## 3. Background

Earlier planning treated a separate external analog/ADC 3.3 V LDO as the conservative path.

After `V5/DSTK22807_3V3_RAIL_CURRENT_BUDGET_REVIEW.md`, that external LDO is no longer a mandatory preferred requirement. It is now a conservative fallback candidate.

DSTK22807 onboard `3V3` became a candidate source for the analog/ADC rail because the measured analog/ADC load appears small compared with expected ESP32-H2/RF current peaks. However:

- DSTK22807 onboard regulator / power-path exact model remains UNKNOWN.
- DSTK22807 onboard `3V3` current capability remains unverified.
- ESP/RF peaks and rail noise still matter for ADC accuracy.
- DSTK onboard `3V3` is not automatically approved.
- External LDO is not automatically mandatory.

Therefore, the safest flexible approach for the first schematic block is a selectable / jumper source option.

## 4. Compared strategies

| Strategy | Description | Pros | Risks | Decision |
| --- | --- | --- | --- | --- |
| DSTK `3V3` only | Use DSTK22807 onboard `3V3` as the only analog/ADC rail source | Simplest BOM and routing if it passes tests | Current/noise/thermal margin unverified; can hide regulator overload or ADC noise issues | NOT SELECTED as sole strategy yet |
| Separate LDO only | Use a dedicated analog/ADC LDO as the only source | Conservative and isolated from DSTK rail uncertainty | Adds parts and may be unnecessary if DSTK `3V3` passes load/noise tests | NOT SELECTED as mandatory strategy |
| Selectable / jumper source option | Show both candidate sources, but allow only one populated/closed source path to `3V3_ADC` | Flexible, test-friendly, avoids premature lock-in, supports fallback | Requires clear notes and strict no-dual-population policy | RECOMMENDED FOR FIRST SCHEMATIC BLOCK |

## 5. Recommended strategy

First schematic block should include a selectable source concept for the analog/ADC rail.

Candidate source A: `DSTK_3V3_CANDIDATE`.

Candidate source B: `LDO_3V3_FALLBACK`.

Selected output rail: `3V3_ADC`.

Rules:

- Only one source may be populated or closed at a time.
- Use jumper, solder bridge, or DNP resistor option.
- Do not tie source A and source B directly together.
- Do not backfeed DSTK `3V3`.
- Do not power DSTK22807 from the external LDO in this first block.
- Do not use `5V` as analog/ADC supply.
- Keep MCP3208 `VDD` on the selected `3V3_ADC` rail.
- Derive `ADC_REF` from the selected 3.3 V rail with proper local decoupling/filtering.
- Keep `ADC_REF` separate from analog `VREF`.

## 6. Proposed net names

| Net | Meaning | Source / destination | Status |
| --- | --- | --- | --- |
| `DSTK_3V3_CANDIDATE` | Candidate input from DSTK22807 onboard `3V3` | Input to mutually exclusive source selection | Candidate only, not final approval |
| `LDO_3V3_FALLBACK` | Separate analog/ADC LDO output candidate | Input to mutually exclusive source selection | Conservative fallback candidate |
| `3V3_ADC` | Selected analog/ADC rail after source selection | MCP3208 `VDD`, ADC/reference support, analog support branch | Selected rail name for first schematic block |
| `ADC_REF` | MCP3208 full-scale reference node | Derived from selected 3.3 V rail with local reference decoupling/filtering | Separate from analog VREF |
| `analog VREF` | Buffered 1.65 V analog midscale bias | Divider/buffer output for analog bias and `VREF_MON` | Not MCP3208 `VREF` |
| `GND` | Common return | Power/reference/ADC common ground | No blind split |
| `BAT+` | Battery/input placeholder | Optional input to LDO fallback and BAT_MON divider | Battery choice still not locked |
| `VREF_MON` | ADC monitor of analog VREF | MCP3208 CH6 | Must not load analog VREF |
| `BAT_MON` | Scaled battery monitor | MCP3208 CH7 through divider/protection | Divider/acquisition still open |

`DSTK_3V3_CANDIDATE` is a candidate input to analog/ADC source selection, not final approval.

`LDO_3V3_FALLBACK` is a separate LDO output candidate for analog/ADC only.

`3V3_ADC` is the selected analog/ADC rail after jumper/source selection.

`ADC_REF` derives from the selected 3.3 V rail but remains distinct from analog VREF.

`analog VREF` remains buffered 1.65 V midscale, not MCP3208 VREF.

## 7. Schematic implementation intent for later

This is not a KiCad task, but the later KiCad edit should use this intent:

- Add hierarchical sheet `POWER_REFERENCE_BLOCK` later.
- In that sheet, include source selection placeholders:
  - `DSTK_3V3_CANDIDATE` input stub.
  - `LDO_3V3_FALLBACK` block placeholder.
  - Mutually exclusive jumper/solder bridge/DNP links to `3V3_ADC`.
- Do not connect `DSTK_3V3_CANDIDATE` to an actual DSTK22807 symbol yet unless a later root schematic strategy explicitly allows it.
- Do not add DSTK22807 external 3.3 V powering.
- Do not connect the external LDO output back into DSTK `3V3`.
- Keep MCP3208 powered from `3V3_ADC`.
- Keep MCP3208 `VREF` = `ADC_REF`.
- Keep `ADC_REF` and analog VREF separate.

## 8. Test requirements before choosing DSTK 3V3 population

Before populating/closing the DSTK source path:

- Identify onboard regulator/power path if possible.
- Dummy-load test `3V3` at 5 mA, 10 mA, 20 mA, 30 mA, and optionally 50 mA.
- Measure voltage droop.
- Check ESP reset/stability.
- Check regulator temperature.
- Check ADC readings/noise under BLE/radio activity if wireless sampling will run.
- Verify no USB human testing.
- Verify no 5V analog/ADC use.

## 9. Population policy

The prototype schematic may show both source options as candidates.

BOM/population must choose only one source path.

Default population should remain TBD until test.

- If DSTK `3V3` passes load/noise test, populate the `DSTK_3V3_CANDIDATE` path.
- If DSTK `3V3` fails or remains unknown, populate the `LDO_3V3_FALLBACK` path.
- Never populate both source links simultaneously.
- Mark the mutual-exclusion rule clearly in schematic notes and assembly notes.

## 10. Human safety notes

- USB-powered human EMG testing: FORBIDDEN.
- 5V pin as analog/ADC supply: NOT APPROVED.
- DSTK external 3.3 V powering: NOT APPROVED YET.
- Battery-powered isolated operation required for human-contact testing.
- This is not a certified medical device.
- Do not use earth-referenced equipment while electrodes are attached to a human.

## 11. Decision

- First schematic power-source strategy: SELECTABLE / JUMPER SOURCE OPTION.
- DSTK onboard 3V3 as analog/ADC source: CANDIDATE ONLY, pending current budget and load/noise test.
- External analog/ADC LDO: CONSERVATIVE FALLBACK CANDIDATE.
- 3V3_ADC source selection: MUTUALLY EXCLUSIVE; do not populate both source links.
- DSTK22807 external 3.3 V powering: NOT APPROVED YET.
- USB-powered human EMG testing: FORBIDDEN.
- 5V as analog/ADC supply: NOT APPROVED.
- KiCad implementation: NOT IN THIS TASK.
- PCB layout: NO.
- Full board schematic: NO.
- Analog EMG chain schematic: NO.

## 12. Recommended next action

- Review and commit this strategy document.
- Update `DECISIONS_V5.md` in a separate commit to reference this power-source strategy.
- Then revise the KiCad schematic edit prompt:
  - include selectable source option,
  - keep DSTK source as candidate input only,
  - keep separate LDO as fallback,
  - prevent backfeed and dual-source connection,
  - keep implementation restricted to `POWER_REFERENCE_BLOCK` sheet.
- Do not begin KiCad edit before decisions update.

## 13. Source documents used

- `V5/DECISIONS_V5.md`
- `V5/DSTK22807_3V3_RAIL_CURRENT_BUDGET_REVIEW.md`
- `V5/POWER_REFERENCE_SCHEMATIC_BLOCK_PROPOSAL.md`
- `V5/POWER_REFERENCE_VALUE_LEVEL_DESIGN_REVIEW.md`
- `V5/BATTERY_REGULATOR_POWER_PATH_PLAN.md`
- `V5/POWER_REFERENCE_COMPONENT_CANDIDATES.md`
- `V5/DSTK22807_POWER_PIN_MEASUREMENT.md`
- `V5/MCP3208_MPN_SYMBOL_FOOTPRINT_REVIEW.md`

Final decision: **FIRST_SCHEMATIC_POWER_SOURCE_STRATEGY_READY_FOR_REVIEW**
