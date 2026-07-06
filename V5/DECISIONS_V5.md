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
- First schematic power-source strategy documented in `V5/FIRST_SCHEMATIC_POWER_SOURCE_STRATEGY.md`.
- First schematic power-source strategy is **SELECTABLE / JUMPER SOURCE OPTION**.
- Limited power/reference schematic block added in commit `1f759a6`.
- Root schematic `V5/EMG_v5.kicad_sch` now includes hierarchical sheet `POWER_REFERENCE_BLOCK`.
- Child schematic file added: `V5/POWER_REFERENCE_BLOCK.kicad_sch`.
- `POWER_REFERENCE_BLOCK` now receives `EMG1_RAW`, `EMG1_RECT`, and `EMG1_ENV` as hierarchical inputs from the Channel 1 analog sheet.
- Project metadata updated in `V5/EMG_v5.kicad_pro`.
- This is the first limited KiCad schematic edit for power/reference support.
- This schematic block is review/prototype planning support, not final production schematic.
- This is not full board schematic completion.
- This is not PCB layout.
- This is not analog EMG chain schematic.
- Candidate source A: `DSTK_3V3_CANDIDATE`.
- Candidate source B: `LDO_3V3_FALLBACK`.
- Selected analog/ADC rail: `3V3_ADC`.
- Power-source strategy uses selectable / jumper source option.
- `DSTK_3V3_CANDIDATE` and `LDO_3V3_FALLBACK` must be mutually exclusive.
- Do not populate/close both source paths at the same time.
- Do not directly tie the two source outputs together.
- Do not backfeed DSTK `3V3`.
- Do not power DSTK22807 from the external LDO in the first schematic block.
- Do not connect the DSTK22807 power path to the external LDO in this block.
- Do not use `5V` as analog/ADC supply.
- External analog/ADC 3.3 V regulator is now a conservative fallback candidate, not a mandatory preferred requirement.
- DSTK22807 onboard `3V3` rail is a new candidate source for analog/ADC power, pending current budget and load/noise test.
- External LDO remains a fallback if DSTK `3V3` current, noise, thermal margin, or ADC stability is unacceptable.
- DSTK22807 3V3 rail current budget review documented in `V5/DSTK22807_3V3_RAIL_CURRENT_BUDGET_REVIEW.md`.
- DSTK22807 onboard regulator / power path exact model remains UNKNOWN.
- DSTK22807 onboard 3V3 current capability remains unverified.
- DSTK22807 3V3 power direction remains unresolved.
- Do not assume DSTK22807 `3V3` pin can power the whole analog/ADC system until current and noise testing pass.
- Do not back-feed DSTK22807 until board behavior is verified.
- Do not back-feed DSTK22807 3V3 until power path is verified.
- For human-connected EMG testing, USB Type-C must be disconnected and the system must be battery-powered/isolated.
- MCP3208 `VDD` should be powered from selected `3V3_ADC`.
- MCP3208 `VREF` remains `ADC_REF`.
- `ADC_REF` and `analog VREF` are separate.
- MCP3208 powered at 3.3 V is preferred.
- If MCP3208 is powered at 5 V later, `DOUT` level shifting is required.
- If DSTK `3V3` is used for MCP3208 `VDD` / `ADC_REF`, local decoupling/filtering and ADC_REF quality review remain required.
- `analog VREF` candidate remains 47k/47k divider plus buffer.
- `VREF_MON` and `BAT_MON` are represented in the child schematic.
- ADC CH0-CH5 placeholders remain only placeholders.
- SPI labels are present but are not connected to DSTK22807 in this task.
- Safety notes are present in the child schematic.
- `BAT_MON` is represented, but final divider/scaling remains review item.
- ADC input protection is unresolved.
- Analog headroom/gain at 3.3 V is unresolved.
- ADC input range protection must ensure CH0-CH7 never exceed VSS to VREF in single-ended MCP3208 use.
- Proper MCP3208 `VDD` decoupling, `VREF` decoupling, and AGND/DGND handling are required in the later schematic/layout.
- Power/reference value-level design review documented in `V5/POWER_REFERENCE_VALUE_LEVEL_DESIGN_REVIEW.md`.
- Value-level candidates exist for regulator, analog VREF divider/buffer, ADC_REF decoupling, ADC input protection, BAT_MON divider, unused ADC bias, and test points.
- These candidates are not final schematic implementation and not final BOM lock.
- Small power/reference schematic block is **ADDED / NEEDS REVIEW**.
- Further changes to this block require review.
- Full board schematic remains **NO**.
- Analog EMG chain schematic remains **NO**.
- Power/reference schematic block proposal documented in `V5/POWER_REFERENCE_SCHEMATIC_BLOCK_PROPOSAL.md`.
- Proposal defines maximum scope for a future small schematic-only power/reference block.
- Proposal has now been followed by the limited implementation in commit `1f759a6`.
- Proposal does not approve full board schematic, analog EMG chain schematic, PCB layout, or DSTK22807 external 3.3 V powering.
- Small power/reference schematic block proposal was **READY_FOR_REVIEW**.
- Limited power/reference schematic block implementation is **ADDED FOR REVIEW**.


## 8. Channel 1 Analog Status

- Checkpoint `8428e5e add channel 1 analog schematic candidate` adds `V5/EMG_CHANNEL_1_ANALOG.kicad_sch`.
- Checkpoint `adea50a fix channel 1 analog polarity and block rectifier placeholder` fixed Channel 1 analog polarity/topology issues and blocked the unfinished RECT topology.
- Checkpoint `3ecdacb implement channel 1 rectifier candidate` implements the reviewable Channel 1 VREF-centered absolute-value rectifier candidate.
- Checkpoint `3825181 cleanup channel 1 rectifier schematic readability` applied Channel 1 RECT schematic readability cleanup.
- Root schematic `V5/EMG_v5.kicad_sch` includes hierarchical sheet `EMG_CHANNEL_1_ANALOG`.
- Channel 1 analog schematic candidate exists and uses real KiCad symbols, not placeholder boxes.
- `EMG1_RAW`, `EMG1_RECT`, and `EMG1_ENV` are connected to MCP3208 CH0, CH1, and CH2 through `POWER_REFERENCE_BLOCK`.
- `ADC_REF` is not used inside the analog child sheet.
- `analog VREF` remains the 1.65 V midbias reference.
- `ADC_REF` and `analog VREF` remain separate.
- `EMG1_REF_ELECTRODE` / sleeve is not tied to GND.
- J301 TRS mapping is fixed: Tip/T = `EMG1_IN_P`, Ring/R = `EMG1_IN_N`, Sleeve/S = `EMG1_REF_ELECTRODE`.
- U301 INA polarity is fixed: IN+ = `EMG1_IN_P_PROT`, IN- = `EMG1_IN_N_PROT`.
- U302A RAW gain topology is corrected to non-inverting feedback.
- U302B and U302C were repurposed from reserved followers into the two-op-amp RECT candidate.
- U302D remains the ENV buffer.
- RECT topology is no longer only a blocked placeholder in the KiCad schematic.
- Channel 1 RECT KiCad candidate: **IMPLEMENTED / REVIEWED / CLEANED READABILITY / NOT FINAL HARDWARE APPROVAL**.
- Cleanup target was `V5/EMG_CHANNEL_1_ANALOG.kicad_sch` only.
- RECT block readability was improved without intentional topology/function change.
- R332 / the old RECT placeholder marker was fully removed.
- `EMG1_RECT_PLACEHOLDER_REMOVED` is no longer present.
- Stale/forbidden strings were checked and absent from the cleaned child sheet: `ADC_REF`, `No ERC`, `R_RECT_PLACEHOLDER`, `EMG1_RECT_PLACEHOLDER_REMOVED`, `RECT BLOCKED PLACEHOLDER`, `ENV placeholder`, `not functional until RECT`, and `TP_analog_VREF`.
- `EMG1_RECT_DRV` is no longer passively biased to `analog VREF` through the old 1M placeholder as the active implementation.
- D331/D332 Schottky candidate diodes were added.
- R333-R338 were added for the 10k/20k rectifier resistor network.
- New internal nets include `EMG1_RECT_SUM1`, `EMG1_RECT_A1_OUT`, `EMG1_RECT_U302B_DRV`, and `EMG1_RECT_SUM2`.
- Output remains `EMG1_RECT_DRV -> R331 470R -> EMG1_RECT`, with `C331 1nF` to GND.
- ENV path follows `EMG1_RECT_DRV` through `R341 33k`, `C341 1uF` to `analog VREF`, U302D buffer, and `EMG1_ENV_DRV`.
- `EMG1_RECT` and `EMG1_ENV` are no longer merely placeholder-only in KiCad, but still need review/validation before Channel 2 or PCB.
- No No ERC markers were added.
- No 5V analog/ADC use was introduced.
- KiCad GUI ERC after diode pin fixes returned to 6 expected placeholder errors only from U201 CH3/CH4/CH5/CLK/Din/`~CS/SHDN`.
- The temporary D331/D332 pin-not-connected errors were fixed.
- Real new ERC errors after RECT implementation: 0.
- Warnings remain, including symbol/library mismatch and other existing planning placeholders; these do not approve hardware and can be reviewed separately.
- Channel 2 remains **NOT STARTED**.
- PCB remains **NOT STARTED**.
- DSTK SPI/power remains **NOT CONNECTED**.
- Exact MCP600x vendor model and exact diode model limitations still remain.
- Channel 1 RECT KiCad implementation received post-review verdict: **POST_REVIEW_PASS_WITH_CLEANUP_RECOMMENDED**.
- The implemented topology matches the planned VREF-centered rectifier candidate at schematic-connectivity level.
- This remains reviewable and is not final hardware approval.
- U302B role was confirmed: `+` input -> `analog VREF`, `-` input -> `EMG1_RECT_SUM1`, output -> `EMG1_RECT_U302B_DRV`.
- U302C role was confirmed: `+` input -> `analog VREF`, `-` input -> `EMG1_RECT_SUM2`, output -> `EMG1_RECT_DRV`.
- D331/D332 orientation matches the implementation plan using the local KiCad `Device:D_Schottky` pin convention.
- R333-R338 match the planned 10k/20k network.
- R332 / old RECT placeholder marker has now been fully removed and no longer biases `EMG1_RECT_DRV`.
- `EMG1_RECT_DRV` is driven by U302C output.
- `EMG1_RECT_DRV -> R331 470R -> EMG1_RECT` remains intact.
- `C331 1nF` to GND remains intact.
- ENV path remains `EMG1_RECT_DRV -> R341 33k -> ENV_LPF_NODE`, `C341 1uF` to `analog VREF`, U302D buffer -> `EMG1_ENV_DRV`.
- `ADC_REF` remains absent from the analog child sheet.
- `EMG1_REF_ELECTRODE` remains isolated from GND.
- No No ERC markers were added.
- No new 5V analog/ADC use was introduced.
- GUI ERC after diode fix showed 6 errors only: the expected U201 placeholder input errors CH3, CH4, CH5, CLK, Din, and `~CS/SHDN`.
- D331/D332 pin-not-connected errors were resolved.
- New real RECT ERC errors: 0.
- RECT block readability cleanup was applied at checkpoint `3825181`.
- D331/D332 and MCP6004 symbol/library mismatch warnings should be reviewed.
- D331/D332 diode MPN/footprint direction is now recorded for first-prototype planning, but schematic footprint fields are not assigned yet.
- KiCad `Device:D_Schottky` convention used by D331/D332: pin 1 = K / cathode and pin 2 = A / anode.
- Current D331/D332 orientation matches the previous Channel 1 RECT review:
  - D331 K -> `EMG1_RECT_U302B_DRV`, A -> `EMG1_RECT_A1_OUT`.
  - D332 K -> `EMG1_RECT_SUM1`, A -> `EMG1_RECT_U302B_DRV`.
- Do not use `BAS70FILM` with a plain 2-pin `Device:D_Schottky` symbol plus generic 3-pad SOT-23 footprint for the first prototype.
- `BAS70FILM` is deferred because the ST SOT-23 single diode uses two active package pins plus a third unused/NC-style pad and does not map cleanly without explicit custom mapping or a project-local 3-pin symbol.
- Recommended D331/D332 first-prototype direction:
  - MPN: `BAS70ZFILM`
  - Package: SOD-123
  - KiCad symbol: `Device:D_Schottky`
  - KiCad footprint: `Diode_SMD:D_SOD-123`
- `BAS70ZFILM` is preferred because the two-terminal SOD-123 package avoids SOT-23 third-pad / NC ambiguity and maps cleanly to the 2-pin diode symbol.
- Avoid `BAS70-04`, `BAS70-05`, `BAS70-06`, and BAT54A/C/S dual/common variants for this rectifier pass.
- Before PCB/layout, visually verify D331/D332 cathode-band orientation against the KiCad footprint and rectifier schematic.
- U302 MCP6004 MPN/package/footprint verification verdict: **MCP6004_PDIP14_SOCKETED_FIRST_PROTO_READY**.
- U302 uses KiCad symbol `Amplifier_Operational:MCP6004`.
- U302 units are present as one package: U302A, U302B, U302C, U302D, plus power unit.
- U302 pin mapping matches MCP6004/LM324-style quad op-amp pinout: U302A 1=OUT, 2=-, 3=+; U302B 7=OUT, 6=-, 5=+; U302C 8=OUT, 9=-, 10=+; U302D 14=OUT, 13=-, 12=+; power 4=V+, 11=V-.
- U302 power unit is present with V+ -> `3V3_ADC` and V- -> GND.
- C360 100nF local decoupling is present.
- MCP6004 caveat: the schematic still has an MCP6004 symbol/library mismatch or stale metadata warning, but functional pin numbering matches the expected MCP6004 pinout.
- Recommended U302 first-prototype direction: MPN `MCP6004-I/P`, package PDIP-14, KiCad symbol `Amplifier_Operational:MCP6004`, KiCad footprint `Package_DIP:DIP-14_W7.62mm`.
- Use socketed PDIP for first prototype bring-up/debug.
- Recommended compact U302 alternative: MPN `MCP6004-I/SL`, package SOIC-14, KiCad footprint `Package_SO:SOIC-14_3.9x8.7mm_P1.27mm`.
- MCP6004-I/ST / TSSOP-14 is only a later compact option with KiCad footprint `Package_SO:TSSOP-14_4.4x5mm_P0.65mm`; it is not recommended for the first prototype because hand bring-up/debug and inspection are worse than PDIP/SOIC.
- Before PCB, resolve/review the MCP6004 symbol/library mismatch warning before final footprint assignment.
- Before PCB, confirm MCP6004 input common-mode range and output swing at `3V3_ADC = 3.3 V`.
- Before PCB, confirm rectifier crossover behavior and output stability with ADC RC caps and ENV/storage load.
- Exact MCP6004 vendor LTspice model is still not locally locked.
- Final MCP6004 purchase-page/datasheet package check is still required before ordering.
- Channel 1 RECT resistor matching verification verdict: **RECT_RESISTOR_MATCHING_DECISION_READY**.
- Confirmed RECT resistor roles: `R333 = 10k` RIN1, `R334 = 10k` RF1, `R335 = 1M` A1 bias / off-state return toward `analog VREF`, `R336 = 20k` RRAW2, `R337 = 10k` RA12, and `R338 = 20k` RF2.
- Ratio-critical resistors: R333/R334 as the 10k/10k first-stage pair, and R336/R337/R338 for the weighted summer.
- Key weighted-summer ratios: `R336 = R338 = 20k` and `R338/R337 = 2`.
- `R335 = 1M` is not ratio-critical; it is mainly bias/leakage/off-state related.
- First-prototype resistor decision: use 0.1% thin-film, 0805 for R333, R334, R336, R337, and R338.
- Use the same manufacturer/series/package/tempco for the ratio-critical resistors where possible.
- Reason: the first prototype should separate rectifier topology error from resistor mismatch.
- `R335 = 1M` may be 1% thin-film 0805; 0.1% is optional only for BOM simplicity or later sensitivity findings.
- Compact / cost-down alternatives: 0603 thin-film can be used later if layout density matters, and 1% thin-film may be considered after tolerance sensitivity or bench validation.
- 1% thick-film is probably functional but is not preferred for first validation of the rectifier candidate.
- Remaining risk: run tolerance sensitivity or bench check before relaxing ratio-critical parts to 1%.
- Exact resistor MPN/series/tempco is still not locked.
- MCP6004 offset/swing/crossover and BAS70 leakage may dominate small-signal error.
- Layout should keep the ratio network close and thermally similar.
- ENV/output cap notes were clarified to distinguish ENV LPF storage cap `C341 = 1uF to analog VREF`, ENV ADC/output cap `C351 = 4.7nF to GND`, and RAW/RECT ADC/output caps `C321/C331 = 1nF to GND`.
- Final Channel 1 RECT status: **CHANNEL_1_RECT_IMPLEMENTED_REVIEWED_CLEANED_READABILITY_NOT_FINAL_HW_APPROVAL**.
- D331/D332 diode MPN/footprint decision status: **CHANNEL_1_RECT_DIODE_MPN_FOOTPRINT_DECISION_READY_NOT_FINAL_HW_APPROVAL**.
- U302 MCP6004 MPN/footprint decision status: **CHANNEL_1_RECT_MCP6004_MPN_FOOTPRINT_DECISION_READY_NOT_FINAL_HW_APPROVAL**.
- Channel 1 RECT resistor matching decision status: **CHANNEL_1_RECT_RESISTOR_MATCHING_DECISION_READY_NOT_FINAL_HW_APPROVAL**.
- Do not duplicate to Channel 2 until cleaned Channel 1 RECT review is accepted.
- Do not start PCB.

Channel 1 RECT LTspice simulation status:

- Simulation planning/prototype files now exist:
  - `V5/sim/rectifier/RECTIFIER_LTSPICE_PLAN.md`
  - `V5/sim/rectifier/emg_vref_abs_rectifier_candidate.cir`
  - `V5/sim/rectifier/emg_vref_abs_rectifier_real_topology_candidate.cir`
  - `V5/sim/rectifier/RECTIFIER_MODEL_CONFIRMATION.md`
  - `V5/sim/rectifier/emg_vref_abs_rectifier_conservative_rrio_candidate.cir`
- Behavioral target/load simulation was created first.
- Behavioral simulation validates the intended behavior: `RECT_OUT = VREF + abs(RAW_IN - VREF)`.
- Behavioral `RECT_OUT` tracks `IDEAL_RECT` and `ERR` is near 0.
- `EMG1_RECT_ADC` follows `RECT_OUT` through the 470R/1nF load path.
- `EMG1_ENV_DRV` gives plausible smoothing.
- This behavioral result is target/load validation only, not a real op-amp/diode implementation.
- A real-component LTspice candidate was added.
- The real topology candidate uses explicit op-amp stages, explicit Schottky diode approximations, and explicit resistor networks.
- `abs()` is used only for `IDEAL_RECT` comparison, not inside the rectifier core.
- Initial LTspice review showed the nominal 0.10 V amplitude case tracks `IDEAL_RECT` closely.
- Initial LTspice review showed the 0.50 V amplitude case remains close with only small crossover/spike error.
- Initial LTspice review showed the 1.00 V amplitude stress case reaches about 2.65 V and does not clip at 3.3 V.
- `ERR` is generally small, with mV-level crossover spikes in stress cases.
- The real topology candidate is **REVIEWABLE**, not automatically KiCad-approved.
- Exact MCP6004/MCP6002 LTspice model was not used.
- Exact BAS70/BAT54 LTspice model was not used.
- Generic RRIO op-amp and generic Schottky approximations were used.
- Because of these model limitations, KiCad implementation still requires manual schematic/topology review.
- KiCad schematic has now been updated with the Channel 1 RECT candidate at checkpoint `3ecdacb`.
- KiCad schematic readability cleanup was applied at checkpoint `3825181`.
- `EMG1_RECT_DRV` in KiCad is no longer placeholder-biased to `analog VREF` through the old active `1M R_RECT_PLACEHOLDER`.
- `EMG1_RECT` and `EMG1_ENV` are no longer merely placeholder-only in KiCad, but still need review/validation before Channel 2 or PCB.
- Model confirmation pass added `V5/sim/rectifier/RECTIFIER_MODEL_CONFIRMATION.md`.
- A conservative RRIO simulation candidate was added: `V5/sim/rectifier/emg_vref_abs_rectifier_conservative_rrio_candidate.cir`.
- Exact MCP6001/MCP6002/MCP6004/MCP600x local LTspice vendor models were not found.
- Exact BAS70 model was not found.
- Local LTspice Toshiba TBAT54 encrypted models were found under LTspice Contrib, but were not copied into the repo.
- Conservative candidate uses finite gain, finite bandwidth, limited output swing generic RRIO behavior and a generic Schottky approximation.
- Initial conservative RRIO candidate failed.
- Cause was found: op-amp macromodel transconductance source polarity was reversed.
- Broken form: `GIN INT 0 ...`.
- Corrected form: `GIN 0 INT ...`.
- This was a macromodel polarity bug, not a topology change.
- Rectifier core, resistor network, diode orientation, VREF references, RAW input, `IDEAL_RECT`, `ERR`, and load path were preserved.
- After the fix, the conservative nominal case tracks `IDEAL_RECT`.
- The conservative 1.00 V stress case reaches about 2.65 V and does not clip at 3.3 V.
- `ERR` is no longer volt-level and remains near 0 visually, with remaining exact error still model-dependent.
- `EMG1_RECT_ADC` follows `RECT_OUT`.
- `EMG1_ENV_DRV` gives plausible smoothing.
- Conservative RRIO simulation is now **PASS / REVIEWABLE**.
- Exact MCP600x vendor model is still not available locally.
- Exact BAS70/BAT54 vendor model is still not included in the repo.
- Final rectifier simulation/model status: **RECTIFIER_SIMULATION_MODEL_CONFIRMATION_REVIEWABLE_NOT_KICAD_APPROVED**.
- KiCad implementation now exists and still requires visual/electrical review before Channel 2 or PCB.
- Any next schematic prompt should be limited to reviewed Channel 1 RECT fixes only.
- Do not start Channel 2.
- Do not start PCB.
- Do not treat LTspice candidates as final hardware approval.


## 9. Open Blockers Before Further Schematic/PCB Work

The following blockers must be closed before further schematic or PCB work:

- Physical DSTK22807 pinout second confirmation.
- Right-row reversal confirmation.
- USB side and antenna side confirmation.
- DSTK22807 `3V3` / `5V` power behavior verification.
- DSTK22807 3V3 external input safety.
- DSTK22807 3V3 current capability.
- USB and external 3.3 V coexistence / backfeed behavior.
- Identify DSTK22807 onboard regulator / power path.
- Estimate or measure ESP32-H2 board current under expected firmware/BLE activity.
- Estimate minimal analog/ADC current.
- Estimate later full 2-channel EMG analog/ADC current.
- Perform DSTK `3V3` dummy-load test.
- Check `3V3` rail voltage under load.
- Check regulator temperature under load.
- Check ESP reset/stability behavior under load.
- Check ADC stability/noise under BLE/radio activity if wireless sampling is used.
- Review and accept `V5/FIRST_SCHEMATIC_POWER_SOURCE_STRATEGY.md`.
- Review `V5/EMG_CHANNEL_1_ANALOG.kicad_sch` visually and electrically.
- Review and accept the cleaned Channel 1 RECT schematic before Channel 2 or PCB.
- Complete cleaned Channel 1 analog/RECT review acceptance.
- Verify source selection is mutually exclusive in schematic.
- Confirm source selection mutual exclusion.
- Ensure `DSTK_3V3_CANDIDATE` is only a candidate input, not final approval.
- Ensure `LDO_3V3_FALLBACK` does not backfeed DSTK `3V3`.
- Confirm no backfeed path from LDO to DSTK `3V3`.
- Decide default population state for source jumpers/solder bridges.
- Mark source population as TBD until DSTK 3V3 load/noise testing.
- Battery regulator architecture.
- Battery chemistry and maximum voltage.
- 3.3 V regulator part/current/noise selection, if separate LDO fallback is selected.
- Decide whether the first schematic block should use the selectable/jumper source option with `DSTK_3V3_CANDIDATE`, `LDO_3V3_FALLBACK`, and selected `3V3_ADC`.
- INA333/op-amp headroom at 3.3 V.
- `analog VREF` buffer topology/part.
- MCP3208 ADC input protection and source impedance/acquisition requirements.
- `BAT_MON` divider/scaling.
- `AGND` / `DGND` grounding strategy.
- Battery choice narrowed enough for regulator and BAT_MON.
- Exact regulator candidate/suffix/package chosen if separate LDO fallback is selected.
- Current budget for analog/ADC estimated.
- Decision whether DSTK22807 is excluded from first power block or included later.
- DSTK22807 external 3.3 V powering test result if Option C is considered.
- analog VREF buffer candidate verified at 3.3 V.
- ADC_REF decoupling/filter values accepted.
- Confirm `ADC_REF` and analog `VREF` are separate.
- ADC input source impedance/acquisition calculation accepted.
- BAT_MON divider values accepted.
- Unused ADC bias values accepted.
- Human-test safety procedure accepted.
- Review and accept `V5/POWER_REFERENCE_SCHEMATIC_BLOCK_PROPOSAL.md`.
- Confirm MCP3208 channel labels match locked CH0-CH7 mapping.
- Confirm DSTK pinout before connecting SPI/power to DSTK symbol.
- Keep DSTK SPI disconnected until pinout and power behavior are verified.
- Confirm DSTK 3V3 current/load/noise tests before choosing population path.
- Verify exact KiCad symbols/footprints for regulator/op-amp/connector/test points.
- Verify analog VREF buffer stability at 3.3 V.
- Accept ADC_REF decoupling strategy.
- Accept BAT_MON divider candidate.
- Accept ADC input R/C/source impedance strategy.
- Decide unused ADC bias approach.
- Keep DSTK22807 power path unresolved unless separately tested.
- Keep DSTK 3V3 current/load/noise test blocker.
- Keep human-test safety procedure blocker.
- Keep ADC_REF/analog VREF separation blocker.
- Human-test safety procedure remains required.


## 10. Should schematic implementation proceed?

Full board schematic: **NO**.

Full two-channel analog EMG chain schematic: **NO / NOT COMPLETE**.

Channel 1 RECT KiCad candidate: **IMPLEMENTED / REVIEWED / CLEANED READABILITY / NOT FINAL HARDWARE APPROVAL**.

Channel 2 analog schematic: **NO / NOT STARTED**.

PCB layout: **NO**.

Small power/reference schematic block proposal: **READY_FOR_REVIEW**.

Small power/reference schematic block: **ADDED FOR REVIEW**.

Further changes to this block require review.

Small power/reference schematic block implementation: **ADDED FOR REVIEW** with selectable/jumper power-source strategy.

First schematic power-source strategy: **SELECTABLE / JUMPER SOURCE OPTION**.

DSTK onboard `3V3` as analog/ADC source: **CANDIDATE ONLY**, pending current budget and load/noise test.

External analog/ADC LDO: **CONSERVATIVE FALLBACK CANDIDATE**.

`3V3_ADC` source selection: **MUTUALLY EXCLUSIVE**.

DSTK22807 external 3.3 V powering: **NOT APPROVED YET**.

USB-powered human EMG testing: **FORBIDDEN**.

5V pin as analog/ADC supply: **NOT APPROVED**.

## 11. Recommended Next Action

- Review and accept the cleaned Channel 1 RECT schematic.
- Review and assign D331/D332 `BAS70ZFILM` / `Diode_SMD:D_SOD-123` only after cathode-band orientation is visually checked against the schematic.
- Review and assign U302 `MCP6004-I/P` / `Package_DIP:DIP-14_W7.62mm` only after the MCP6004 symbol/library mismatch warning is reviewed.
- Assign R333/R334/R336/R337/R338 as 0.1% thin-film 0805 from the same series where possible; keep R335 as 1% thin-film 0805 unless BOM simplification or sensitivity results justify 0.1%.
- Confirm ENV behavior after RECT implementation.
- Do not duplicate to Channel 2 until cleaned Channel 1 RECT review is accepted.
- Do not start PCB.
- Do not treat the KiCad/LTspice RECT candidates as final hardware approval.
- Do not connect DSTK22807 power/SPI nets until pinout and power behavior are verified.
- Do not begin full board schematic.
- Do not use 5V as analog/ADC supply.

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
- `V5/POWER_REFERENCE_VALUE_LEVEL_DESIGN_REVIEW.md`
- `V5/POWER_REFERENCE_SCHEMATIC_BLOCK_PROPOSAL.md`
- `V5/DSTK22807_3V3_RAIL_CURRENT_BUDGET_REVIEW.md`
- `V5/FIRST_SCHEMATIC_POWER_SOURCE_STRATEGY.md`
- `V5/POWER_REFERENCE_BLOCK.kicad_sch`
- `V5/sim/rectifier/RECTIFIER_LTSPICE_PLAN.md`
- `V5/sim/rectifier/emg_vref_abs_rectifier_candidate.cir`
- `V5/sim/rectifier/emg_vref_abs_rectifier_real_topology_candidate.cir`
- `V5/sim/rectifier/RECTIFIER_MODEL_CONFIRMATION.md`
- `V5/sim/rectifier/emg_vref_abs_rectifier_conservative_rrio_candidate.cir`
- `V5/EMG_CHANNEL_1_ANALOG.kicad_sch`
- `V5/EMG_v5.kicad_sch`
- `V5/EMG_v5.kicad_pro`

Final decision: **CHANNEL_1_RECT_IMPLEMENTED_REVIEWED_CLEANED_READABILITY_NOT_FINAL_HW_APPROVAL**
