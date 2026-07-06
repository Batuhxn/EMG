# EMG V5 Status

Date: 2026-07-06

## Current State

The V5 folder now contains a clean separate KiCad working base copied from the verified V4 project, plus V5 documentation and ESP32-H2 firmware scaffold.

Current schematic checkpoint:

- `080eef1 remove legacy root emg schematic block`
- `60d5881 implement channel 2 analog schematic`
- Root schematic: `V5/EMG_v5.kicad_sch`
- Power/reference child sheet: `V5/POWER_REFERENCE_BLOCK.kicad_sch`
- Channel 1 analog child sheet: `V5/EMG_CHANNEL_1_ANALOG.kicad_sch`
- Channel 2 analog child sheet: `V5/EMG_CHANNEL_2_ANALOG.kicad_sch`
- Root schematic now keeps the hierarchical `EMG_CHANNEL_1_ANALOG`, `EMG_CHANNEL_2_ANALOG`, and `POWER_REFERENCE_BLOCK` structure.
- `POWER_REFERENCE_BLOCK` now receives `EMG1_RAW`, `EMG1_RECT`, `EMG1_ENV`, `EMG2_RAW`, `EMG2_RECT`, and `EMG2_ENV` as hierarchical inputs.

Current schematic status:

- Power/reference schematic block: **ADDED / REVIEWED ENOUGH FOR CURRENT PLANNING**
- Channel 1 RECT KiCad candidate: **IMPLEMENTED / REVIEWED / NOT FINAL HARDWARE APPROVAL**
- Channel 2 analog schematic: **IMPLEMENTED / NOT FINAL HARDWARE APPROVAL**
- PCB layout: **NOT STARTED / NO**
- Full two-channel analog EMG chain schematic: **CANDIDATES IMPLEMENTED / NOT FINAL HARDWARE APPROVAL**
- DSTK external 3.3 V powering: **NOT APPROVED YET**
- USB plus external 3.3 V simultaneous connection: **NOT APPROVED**
- Human USB testing: **FORBIDDEN**
- 5V analog/ADC: **NOT APPROVED**

Channel 2 and legacy root cleanup status: **CHANNEL_2_IMPLEMENTED_LEGACY_ROOT_REMOVED_NOT_FINAL_HW_APPROVAL**.

- Commit `60d5881 implement channel 2 analog schematic` created `V5/EMG_CHANNEL_2_ANALOG.kicad_sch`.
- Commit `60d5881` also updated `V5/EMG_v5.kicad_sch` and `V5/POWER_REFERENCE_BLOCK.kicad_sch`.
- Channel 2 was created as a controlled copy/adaptation of Channel 1.
- Channel 2 uses +100 refdes mapping: J401, U401, U402, R4xx, C4xx, and D431/D432.
- Channel 2 uses `EMG2_*` nets.
- Shared rails remain `3V3_ADC`, `analog VREF`, and GND.
- `ADC_REF` was not introduced into the Channel 2 analog sheet.
- `EMG2_REF_ELECTRODE` remains isolated from GND/chassis/USB.
- No No ERC markers were added.
- PCB was not started.
- DSTK SPI/power was not connected.
- EMG2 outputs are integrated with MCP3208 U201: `EMG2_RAW -> CH3`, `EMG2_RECT -> CH4`, and `EMG2_ENV -> CH5`.
- Previous U201 CH3/CH4/CH5 placeholder input errors disappeared.
- Channel 2 inherited the Channel 1 BOM/footprint strategy: D431/D432 use `BAS70ZFILM`, `Device:D_Schottky`, and `Diode_SMD:D_SOD-123`; U402 uses MCP6004 and `Package_DIP:DIP-14_W7.62mm`; Channel 2 RECT resistors inherit the 0805 resistor footprint strategy; relevant capacitors inherit the 0805 capacitor footprint strategy.
- Commit `080eef1 remove legacy root emg schematic block` removed the old standalone/legacy root EMG schematic block from `V5/EMG_v5.kicad_sch`.
- The removed legacy flat block included the old J1/U1/U2/J2-style EMG circuit.
- Root now keeps the hierarchical Channel 1, Channel 2, and `POWER_REFERENCE_BLOCK` structure.
- Root GND PWR_FLAG/source declaration was restored cleanly after legacy block deletion.
- The old legacy block was not restored.
- GUI/ERC target after Channel 2 implementation and legacy cleanup reached 3 remaining errors.
- Remaining errors are expected SPI placeholders only: U201 CLK input not driven, U201 Din input not driven, and U201 `~CS/SHDN` input not driven.
- U201 CH3/CH4/CH5 errors are gone.
- U201 DGND error was fixed.
- GND label-not-connected error was fixed.
- #FLG power-output conflict was fixed.
- `3V3_ADC` / `analog VREF` label-not-connected errors were fixed.
- No D431/D432 pin-not-connected errors appeared.
- No new real Channel 1/Channel 2 errors remain.
- Remaining caveats: this is not final hardware approval; D431/D432 cathode-band orientation must be visually reviewed before PCB; U302/U402 MCP6004 symbol/library mismatch warnings remain; D331/D332/D431/D432 D_Schottky symbol/library mismatch warnings remain; exact passive ordering MPNs remain unlocked; DSTK SPI/power remains disconnected; PCB remains not started.

Channel 1 analog / RECT implementation status at checkpoint `8f2bf9e`:

- Channel 1 analog schematic remains polarity fixed.
- `V5/EMG_CHANNEL_1_ANALOG.kicad_sch` now includes a reviewable Channel 1 VREF-centered absolute-value rectifier candidate.
- RECT is no longer only a blocked placeholder in the KiCad schematic.
- Channel 1 RECT KiCad candidate: **IMPLEMENTED / REVIEWED / CLEANED READABILITY / FOOTPRINT FIELDS ASSIGNED / DIODE VALUES CLARIFIED / NOT FINAL HARDWARE APPROVAL**.
- Checkpoint `3825181 cleanup channel 1 rectifier schematic readability` applied the Channel 1 RECT schematic readability cleanup.
- Checkpoint `eca2cc3 assign channel 1 rectifier footprints` assigned Channel 1 RECT/ENV-related schematic footprint fields.
- Checkpoint `8f2bf9e clarify channel 1 rectifier diode values` clarified the visible D331/D332 value text.
- Cleanup target was `V5/EMG_CHANNEL_1_ANALOG.kicad_sch` only.
- Footprint assignment edit target was also `V5/EMG_CHANNEL_1_ANALOG.kicad_sch` only.
- Diode value-text clarification target was also `V5/EMG_CHANNEL_1_ANALOG.kicad_sch` only.
- RECT block readability was improved without intentional topology/function change.
- Footprint assignment diff was limited to footprint property fields; no value, net, or topology changes were intended.
- Diode value-text clarification changed D331/D332 visible value properties only; no net, topology, symbol, footprint, or orientation change was made.
- R332 / the old RECT placeholder marker was fully removed.
- `EMG1_RECT_PLACEHOLDER_REMOVED` is no longer present.
- Stale/forbidden strings were checked and absent from the cleaned child sheet: `ADC_REF`, `No ERC`, `R_RECT_PLACEHOLDER`, `EMG1_RECT_PLACEHOLDER_REMOVED`, `RECT BLOCKED PLACEHOLDER`, `ENV placeholder`, `not functional until RECT`, and `TP_analog_VREF`.
- `EMG1_RECT_DRV` is no longer passively biased to `analog VREF` through the old 1M placeholder as the active implementation.
- U302B and U302C were repurposed from reserved followers into the two-op-amp RECT candidate.
- U302D remains the ENV buffer.
- D331/D332 Schottky candidate diodes were added.
- R333-R338 were added for the 10k/20k rectifier resistor network.
- New internal nets include `EMG1_RECT_SUM1`, `EMG1_RECT_A1_OUT`, `EMG1_RECT_U302B_DRV`, and `EMG1_RECT_SUM2`.
- Output remains `EMG1_RECT_DRV -> R331 470R -> EMG1_RECT`, with `C331 1nF` to GND.
- ENV path follows `EMG1_RECT_DRV` through `R341 33k`, `C341 1uF` to `analog VREF`, U302D buffer, and `EMG1_ENV_DRV`.
- `ADC_REF` remains absent from the analog child sheet.
- `EMG1_REF_ELECTRODE` remains isolated from GND.
- No No ERC markers were added.
- No 5V analog/ADC use was introduced.
- `EMG1_RECT` and `EMG1_ENV` are no longer merely placeholder-only in KiCad, but still need PCB/hardware review before layout.
- Channel 2 was later implemented as a controlled schematic duplicate/adaptation of Channel 1 at checkpoint `60d5881`.
- PCB remains **NOT STARTED**.
- DSTK SPI/power remains **NOT CONNECTED**.
- Exact MCP600x vendor model and exact diode model limitations still remain.
Channel 1 RECT post-implementation review result:

- Post-review verdict: **POST_REVIEW_PASS_WITH_CLEANUP_RECOMMENDED**.
- The implemented topology matches the planned VREF-centered rectifier candidate at schematic-connectivity level.
- This remains reviewable and is not final hardware approval.
- U302B role was confirmed: `+` input -> `analog VREF`, `-` input -> `EMG1_RECT_SUM1`, output -> `EMG1_RECT_U302B_DRV`.
- U302C role was confirmed: `+` input -> `analog VREF`, `-` input -> `EMG1_RECT_SUM2`, output -> `EMG1_RECT_DRV`.
- D331/D332 orientation matches the implementation plan using the local KiCad `Device:D_Schottky` pin convention.
- R333-R338 match the planned 10k/20k rectifier resistor network.
- R332 / old RECT placeholder marker has now been fully removed and no longer biases `EMG1_RECT_DRV`.
- `EMG1_RECT_DRV` is driven by U302C output.
- `EMG1_RECT_DRV -> R331 470R -> EMG1_RECT` remains intact.
- `C331 1nF` to GND remains intact.
- ENV path remains `EMG1_RECT_DRV -> R341 33k -> ENV_LPF_NODE`, `C341 1uF` to `analog VREF`, U302D buffer -> `EMG1_ENV_DRV`.
- `ADC_REF` remains absent from the analog child sheet.
- `EMG1_REF_ELECTRODE` remains isolated from GND.
- No No ERC markers were added.
- No new 5V analog/ADC use was introduced.

Cleanup/checks before Channel 2 or PCB:

- RECT block readability cleanup was applied at checkpoint `3825181`.
- Channel 1 RECT remains reviewable, not final hardware approval.
- Final pre-duplication review verdict: **CHANNEL_1_READY_FOR_CHANNEL_2_DUPLICATION**.
- Channel 1 ready status: **CHANNEL_1_READY_FOR_CHANNEL_2_DUPLICATION_NOT_FINAL_HW_APPROVAL**.
- Issues requiring fix before Channel 2: none found in the reviewed gates.
- Channel 2 may start as a controlled schematic duplicate/adaptation of Channel 1.
- This is still not PCB approval and not final hardware approval.
- Confirmed pass item: RAW path still reaches `EMG1_RAW_DRV`, then `R321 -> EMG1_RAW` with `C321` to GND.
- Confirmed pass item: RECT path is present around U302B/U302C, D331/D332, and R333-R338.
- Confirmed pass item: D331/D332 values are `BAS70ZFILM`, symbol `Device:D_Schottky`, footprint `Diode_SMD:D_SOD-123`.
- Confirmed pass item: ENV path remains `EMG1_RECT_DRV -> R341 -> ENV_LPF_NODE`, `C341` to `analog VREF`, U302D -> `EMG1_ENV_DRV`.
- Confirmed pass item: output paths remain `EMG1_RECT_DRV -> R331 -> EMG1_RECT` and `EMG1_ENV_DRV -> R351 -> EMG1_ENV`.
- Confirmed pass item: `ADC_REF` is absent from the child sheet.
- Confirmed pass item: no No ERC markers were added.
- Confirmed pass item: old RECT placeholders and `BAT54/BAS70` text are absent.
- Confirmed pass item: `EMG1_REF_ELECTRODE` remains isolated from GND.
- Confirmed pass item: docs match schematic fields for D331/D332, U302, R333-R338, and C321/C331/C351/C341/C360.
- D331/D332 and MCP6004 symbol/library mismatch warnings should be reviewed.
- D331/D332 diode MPN/footprint direction is recorded for first-prototype planning, and schematic footprint fields were assigned at checkpoint `eca2cc3`.
- D331/D332 visible schematic value text was clarified at checkpoint `8f2bf9e`.
- D331 value is now `BAS70ZFILM`.
- D332 value is now `BAS70ZFILM`.
- The previous visible `BAT54/BAS70` ambiguity is removed from the Channel 1 analog schematic.
- D331/D332 KiCad symbol convention: `Device:D_Schottky` uses pin 1 = K / cathode and pin 2 = A / anode.
- D331/D332 still use KiCad symbol `Device:D_Schottky` and footprint `Diode_SMD:D_SOD-123`.
- Current D331/D332 orientation matches the previous Channel 1 RECT review: D331 K -> `EMG1_RECT_U302B_DRV`, D331 A -> `EMG1_RECT_A1_OUT`; D332 K -> `EMG1_RECT_SUM1`, D332 A -> `EMG1_RECT_U302B_DRV`.
- Do not use `BAS70FILM` with a plain 2-pin `Device:D_Schottky` symbol plus generic 3-pad SOT-23 footprint for the first prototype.
- `BAS70FILM` is deferred because the ST SOT-23 single diode uses two active package pins plus a third unused/NC-style pad and needs explicit custom mapping or a project-local 3-pin symbol/footprint review.
- Recommended D331/D332 first-prototype direction: MPN `BAS70ZFILM`, package SOD-123, KiCad symbol `Device:D_Schottky`, KiCad footprint `Diode_SMD:D_SOD-123`.
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
- Channel 1 RAW/RECT/ENV capacitor dielectric/package verification verdict: **RECT_ENV_CAPACITOR_DECISION_READY**.
- Confirmed capacitor roles: `C321 = 1nF` RAW ADC/output cap to GND, `C331 = 1nF` RECT ADC/output cap to GND, `C351 = 4.7nF` ENV ADC/output cap to GND, `C341 = 1uF` ENV LPF storage cap from `ENV_LPF_NODE` to `analog VREF`, and `C360 = 100nF` U302 local decoupling cap.
- First-prototype capacitor decision for C321/C331: 1nF C0G/NP0, 0805, 5% or better preferred.
- First-prototype capacitor decision for C351: 4.7nF C0G/NP0, 0805 preferred; X7R is an acceptable fallback if C0G/NP0 availability becomes difficult.
- First-prototype capacitor decision for C341: 1uF X7R, 0805, 10% acceptable, with 16V or 25V rating to reduce DC-bias derating.
- First-prototype capacitor decision for C360: 100nF X7R, 0805, 16V or 25V local decoupling.
- Avoid Y5V/Z5U for these analog/output/decoupling capacitors.
- Compact alternative: 0603 can be considered later if layout density matters; keep C321/C331/C351 as C0G/NP0 where practical and keep C341/C360 as X7R.
- Remaining capacitor risk: C341 tolerance and DC-bias derating shift the ENV cutoff.
- Remaining capacitor risk: C341 returns to `analog VREF`, so VREF buffer stability/loading should be reviewed.
- Remaining capacitor risk: MCP6004 output stability with ADC/output caps should remain protected by 470R series resistors, but still needs review.
- Exact capacitor MPN/voltage/tolerance is not locked yet.
- Channel 1 RECT/ENV footprint field assignment status: **CHANNEL_1_RECT_FOOTPRINT_FIELDS_ASSIGNED_NOT_FINAL_HW_APPROVAL**.
- Assigned schematic footprint fields at checkpoint `eca2cc3`:
  - D331 = `Diode_SMD:D_SOD-123`
  - D332 = `Diode_SMD:D_SOD-123`
  - U302 = `Package_DIP:DIP-14_W7.62mm`
  - R333 = `Resistor_SMD:R_0805_2012Metric`
  - R334 = `Resistor_SMD:R_0805_2012Metric`
  - R335 = `Resistor_SMD:R_0805_2012Metric`
  - R336 = `Resistor_SMD:R_0805_2012Metric`
  - R337 = `Resistor_SMD:R_0805_2012Metric`
  - R338 = `Resistor_SMD:R_0805_2012Metric`
  - C321 = `Capacitor_SMD:C_0805_2012Metric`
  - C331 = `Capacitor_SMD:C_0805_2012Metric`
  - C351 = `Capacitor_SMD:C_0805_2012Metric`
  - C341 = `Capacitor_SMD:C_0805_2012Metric`
  - C360 = `Capacitor_SMD:C_0805_2012Metric`
- Root GUI ERC after footprint assignment remained 6 errors and 21 warnings.
- The 6 errors are still the expected U201 placeholder input errors: CH3, CH4, CH5, CLK, Din, and `~CS/SHDN`.
- No new D331/D332 pin-not-connected errors appeared after footprint assignment.
- No new real RECT/ENV ERC errors appeared after footprint assignment.
- D331/D332 schematic value text now says `BAS70ZFILM`; the previous visible `BAT54/BAS70` ambiguity is removed.
- D331/D332 still use symbol `Device:D_Schottky` and footprint `Diode_SMD:D_SOD-123`.
- No net, topology, footprint, symbol, or diode orientation change was made by the value-text clarification.
- Exact ordering MPNs for passives are not locked yet.
- D331/D332 cathode-band footprint orientation must be visually checked before PCB.
- MCP6004 symbol/library mismatch warning still needs review.
- Channel 2 may start as a controlled schematic duplicate/adaptation of Channel 1.
- Do not start PCB.

Final Channel 1 RECT status: **CHANNEL_1_READY_FOR_CHANNEL_2_DUPLICATION_NOT_FINAL_HW_APPROVAL**.

D331/D332 diode MPN/footprint decision status: **CHANNEL_1_RECT_DIODE_MPN_FOOTPRINT_DECISION_READY_NOT_FINAL_HW_APPROVAL**.

U302 MCP6004 MPN/footprint decision status: **CHANNEL_1_RECT_MCP6004_MPN_FOOTPRINT_DECISION_READY_NOT_FINAL_HW_APPROVAL**.

Channel 1 RECT resistor matching decision status: **CHANNEL_1_RECT_RESISTOR_MATCHING_DECISION_READY_NOT_FINAL_HW_APPROVAL**.

Channel 1 RAW/RECT/ENV capacitor decision status: **CHANNEL_1_RECT_ENV_CAPACITOR_DECISION_READY_NOT_FINAL_HW_APPROVAL**.

Channel 1 RECT/ENV footprint field assignment status: **CHANNEL_1_RECT_FOOTPRINT_FIELDS_ASSIGNED_NOT_FINAL_HW_APPROVAL**.

Channel 1 RECT diode value-text status: **CHANNEL_1_RECT_DIODE_VALUES_CLARIFIED_NOT_FINAL_HW_APPROVAL**.

Channel 1 pre-duplication review status: **CHANNEL_1_READY_FOR_CHANNEL_2_DUPLICATION_NOT_FINAL_HW_APPROVAL**.

Channel 2 implementation and legacy root cleanup status: **CHANNEL_2_IMPLEMENTED_LEGACY_ROOT_REMOVED_NOT_FINAL_HW_APPROVAL**.

- Checkpoint `60d5881 implement channel 2 analog schematic` created `V5/EMG_CHANNEL_2_ANALOG.kicad_sch`.
- Root sheet `V5/EMG_v5.kicad_sch` was updated.
- `V5/POWER_REFERENCE_BLOCK.kicad_sch` was updated.
- Channel 2 was created as a controlled copy/adaptation of Channel 1.
- Channel 2 uses +100 refdes mapping: J401, U401, U402, R4xx, C4xx, and D431/D432.
- Channel 2 uses `EMG2_*` nets and shared rails `3V3_ADC`, `analog VREF`, and GND.
- `ADC_REF` was not introduced.
- `EMG2_REF_ELECTRODE` remains isolated from GND/chassis/USB.
- No No ERC markers were added.
- PCB was not started.
- DSTK SPI/power was not connected.
- D431/D432 inherited the Channel 1 diode decision: `BAS70ZFILM`, `Device:D_Schottky`, `Diode_SMD:D_SOD-123`.
- U402 inherited the Channel 1 op-amp package direction: MCP6004, `Package_DIP:DIP-14_W7.62mm`.
- Channel 2 RECT resistors inherit the 0805 resistor footprint strategy.
- Channel 2 relevant capacitors inherit the 0805 capacitor footprint strategy.
- `EMG2_RAW` is connected to U201 CH3.
- `EMG2_RECT` is connected to U201 CH4.
- `EMG2_ENV` is connected to U201 CH5.
- Previous U201 CH3/CH4/CH5 placeholder errors disappeared.
- Checkpoint `080eef1 remove legacy root emg schematic block` removed the old standalone/legacy root EMG schematic block from `V5/EMG_v5.kicad_sch`.
- The removed legacy flat block used the old J1/U1/U2/J2-style EMG circuit.
- Root now keeps only the hierarchical `EMG_CHANNEL_1_ANALOG`, `EMG_CHANNEL_2_ANALOG`, and `POWER_REFERENCE_BLOCK` structure.
- Root GND PWR_FLAG/source declaration was restored cleanly after legacy block deletion.
- The old legacy block was not restored.

Current root ERC status after Channel 2 implementation and legacy root cleanup:

- GUI/ERC target reached: 3 errors remain.
- Remaining errors are expected SPI placeholders only:
  - U201 CLK input not driven
  - U201 Din input not driven
  - U201 `~CS/SHDN` input not driven
- U201 CH3/CH4/CH5 errors are gone.
- U201 DGND error was fixed.
- GND label-not-connected error was fixed.
- #FLG power-output conflict was fixed.
- `3V3_ADC` / `analog VREF` label-not-connected errors were fixed.
- No D431/D432 pin-not-connected errors appeared.
- No new real Channel 1/Channel 2 errors remain.
- Remaining ERC errors are expected only until DSTK SPI is connected.

## Rectifier LTspice Simulation Status

Simulation planning/prototype files now exist for Channel 1 RECT review:

- `V5/sim/rectifier/RECTIFIER_LTSPICE_PLAN.md`
- `V5/sim/rectifier/emg_vref_abs_rectifier_candidate.cir`
- `V5/sim/rectifier/emg_vref_abs_rectifier_real_topology_candidate.cir`
- `V5/sim/rectifier/RECTIFIER_MODEL_CONFIRMATION.md`
- `V5/sim/rectifier/emg_vref_abs_rectifier_conservative_rrio_candidate.cir`

Behavioral target/load simulation status:

- Behavioral target/load simulation was created first.
- It validates the intended behavior: `RECT_OUT = VREF + abs(RAW_IN - VREF)`.
- Behavioral `RECT_OUT` tracks `IDEAL_RECT`.
- `ERR` is near 0.
- `EMG1_RECT_ADC` follows `RECT_OUT` through the 470R/1nF load path.
- `EMG1_ENV_DRV` gives plausible smoothing.
- This is behavioral target/load validation only, not a real op-amp/diode implementation.

Real topology simulation status:

- A real-component LTspice candidate was added.
- It uses explicit op-amp stages, explicit Schottky diode approximations, and explicit resistor networks.
- `abs()` is used only for `IDEAL_RECT` comparison, not inside the rectifier core.
- Initial LTspice review showed the nominal 0.10 V amplitude case tracks `IDEAL_RECT` closely.
- Initial LTspice review showed the 0.50 V amplitude case remains close with only small crossover/spike error.
- Initial LTspice review showed the 1.00 V amplitude stress case reaches about 2.65 V and does not clip at 3.3 V.
- `ERR` is generally small, with mV-level crossover spikes in stress cases.
- This makes the topology **REVIEWABLE**, not automatically KiCad-approved.

Model limitations:

- Exact MCP6004/MCP6002 LTspice model was not used.
- Exact BAS70/BAT54 LTspice model was not used.
- Generic RRIO op-amp and generic Schottky approximations were used.
- Because of these limitations, KiCad implementation still requires manual schematic/topology review.

Model confirmation status:

- `V5/sim/rectifier/RECTIFIER_MODEL_CONFIRMATION.md` was added.
- A conservative RRIO simulation candidate was added: `V5/sim/rectifier/emg_vref_abs_rectifier_conservative_rrio_candidate.cir`.
- Exact MCP6001/MCP6002/MCP6004/MCP600x local LTspice vendor models were not found.
- Exact BAS70 model was not found.
- Local LTspice Toshiba TBAT54 encrypted models were found under LTspice Contrib, but were not copied into the repo.
- The conservative candidate uses finite gain, finite bandwidth, limited output swing generic RRIO behavior and a generic Schottky approximation.
- Initial conservative RRIO candidate failed because the op-amp macromodel transconductance source polarity was reversed.
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
- Therefore this remains **REVIEWABLE**, not final hardware approval or KiCad approval.

Final rectifier simulation/model status: **RECTIFIER_SIMULATION_MODEL_CONFIRMATION_REVIEWABLE_NOT_KICAD_APPROVED**.

KiCad RECT implementation status:

- Channel 1 RECT KiCad candidate: **READY FOR CHANNEL 2 DUPLICATION / NOT FINAL HARDWARE APPROVAL**.
- KiCad schematic has been updated with the Channel 1 RECT candidate in `V5/EMG_CHANNEL_1_ANALOG.kicad_sch`.
- Checkpoint `3825181` applied readability cleanup only to `V5/EMG_CHANNEL_1_ANALOG.kicad_sch`.
- `EMG1_RECT_DRV` is no longer placeholder-biased to `analog VREF` through the old active 1M placeholder.
- `EMG1_RECT` and `EMG1_ENV` are no longer merely placeholder-only in KiCad, but still need PCB/hardware review before layout.
- Channel 2 has since been implemented as a controlled schematic duplicate/adaptation of Channel 1 at checkpoint `60d5881`.
- PCB remains **NOT STARTED**.
- DSTK SPI/power remains **NOT CONNECTED**.
- This is not final hardware approval and not production ready.

## Important

This is not a finished V5 fabrication layout yet.

The V5 schematic and PCB still need these planned follow-up items:

- DSTK22807 carrier footprint
- power/regulator/BAT_MON details
- V5 test points and silkscreen safety warnings

PCB layout must not start for the DSTK22807 carrier until the physical board is measured and pin labels are confirmed.

## Next Actions

- Perform Channel 2 post-implementation review.
- Review D331/D332 `BAS70ZFILM` / `Diode_SMD:D_SOD-123` cathode-band orientation visually against the schematic before PCB.
- Review D431/D432 `BAS70ZFILM` / `Diode_SMD:D_SOD-123` cathode-band orientation visually against the schematic before PCB.
- Review the U302 `MCP6004-I/P` / `Package_DIP:DIP-14_W7.62mm` assignment alongside the remaining MCP6004 symbol/library mismatch warning.
- Review the U402 `MCP6004` / `Package_DIP:DIP-14_W7.62mm` assignment alongside the remaining MCP6004 symbol/library mismatch warning.
- Lock exact ordering MPNs for R333/R334/R336/R337/R338 as 0.1% thin-film 0805 from the same series where possible; keep R335 as 1% thin-film 0805 unless BOM simplification or sensitivity results justify 0.1%.
- Lock exact ordering MPNs for C321/C331 as 1nF C0G/NP0 0805, C351 as 4.7nF C0G/NP0 0805 preferred, and C341/C360 as X7R 0805 with suitable voltage ratings before Channel 2 or PCB.
- Confirm ENV behavior and analog VREF buffer loading after RECT/capacitor decisions.
- Plan DSTK SPI/power connection after Channel 2 post-implementation review.
- Current root ERC target is 3 expected SPI placeholder errors only; do not add No ERC markers for them yet.
- Do not start PCB layout yet.
- Do not connect DSTK22807 power/SPI nets until pinout and power behavior are verified.
- Do not treat the KiCad/LTspice RECT candidates as final hardware approval or production readiness.

## Next Required Input

Measure or photograph the DSTK22807 board clearly enough to confirm:

- board length and width
- header/castellated pitch
- exact pin labels
- antenna location and keepout
- USB connector overhang
- usable SPI-capable pins
