# EMG V5 Status

Date: 2026-07-04

## Current State

The V5 folder now contains a clean separate KiCad working base copied from the verified V4 project, plus V5 documentation and ESP32-H2 firmware scaffold.

Current schematic checkpoint:

- `76745f7 sync docs with channel 1 pre duplication review`
- `8f2bf9e clarify channel 1 rectifier diode values`
- Root schematic: `V5/EMG_v5.kicad_sch`
- Power/reference child sheet: `V5/POWER_REFERENCE_BLOCK.kicad_sch`
- Channel 1 analog child sheet: `V5/EMG_CHANNEL_1_ANALOG.kicad_sch`
- Root schematic now includes the `EMG_CHANNEL_1_ANALOG` hierarchical sheet.
- `POWER_REFERENCE_BLOCK` now receives `EMG1_RAW`, `EMG1_RECT`, and `EMG1_ENV` as hierarchical inputs.

Current schematic status:

- Power/reference schematic block: **ADDED / REVIEWED ENOUGH FOR CURRENT PLANNING**
- Channel 1 RECT KiCad candidate: **READY FOR CHANNEL 2 DUPLICATION / NOT FINAL HARDWARE APPROVAL**
- Channel 2 analog schematic: **DUPLICATION PLAN READY / NOT IMPLEMENTED**
- PCB layout: **NOT STARTED / NO**
- Full two-channel analog EMG chain: **NOT COMPLETE**
- DSTK external 3.3 V powering: **NOT APPROVED YET**
- USB plus external 3.3 V simultaneous connection: **NOT APPROVED**
- Human USB testing: **FORBIDDEN**
- 5V analog/ADC: **NOT APPROVED**

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
- Channel 2 may start as a controlled schematic duplicate/adaptation of Channel 1.
- Channel 2 remains **NOT STARTED** in the repository at this checkpoint.
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

Channel 2 duplication/adaptation plan status: **CHANNEL_2_DUPLICATION_PLAN_READY_NOT_IMPLEMENTED**.

Channel 2 implementation plan:

- Planning verdict: **CHANNEL_2_PLAN_READY**.
- Future file/sheet strategy: create separate child sheet `V5/EMG_CHANNEL_2_ANALOG.kicad_sch`.
- Use controlled copy/adaptation from `V5/EMG_CHANNEL_1_ANALOG.kicad_sch`.
- Channel 1 remains unchanged unless absolutely required.
- Required future implementation edit scope is expected to touch only `V5/EMG_CHANNEL_2_ANALOG.kicad_sch`, `V5/EMG_v5.kicad_sch`, and `V5/POWER_REFERENCE_BLOCK.kicad_sch`.
- No docs, PCB, firmware, LTspice, project/library changes are expected in that implementation edit unless explicitly approved.
- Reference strategy: use +100 mapping from Channel 1: `J301 -> J401`, `U301 -> U401`, `U302 -> U402`, `R3xx -> R4xx`, `C3xx -> C4xx`, and `D331/D332 -> D431/D432`.
- `U402` must be a separate MCP6004 package because Channel 1 already uses U302 units A/B/C/D plus power.
- Net strategy: map EMG1 nets to EMG2 equivalents, including `EMG1_IN_P -> EMG2_IN_P`, `EMG1_IN_N -> EMG2_IN_N`, `EMG1_REF_ELECTRODE -> EMG2_REF_ELECTRODE`, `EMG1_RAW_DRV -> EMG2_RAW_DRV`, `EMG1_RAW -> EMG2_RAW`, `EMG1_RECT_DRV -> EMG2_RECT_DRV`, `EMG1_RECT -> EMG2_RECT`, `EMG1_ENV_DRV -> EMG2_ENV_DRV`, and `EMG1_ENV -> EMG2_ENV`.
- RECT internal nets should also become EMG2 equivalents, including `EMG1_RECT_A1_OUT -> EMG2_RECT_A1_OUT`, `EMG1_RECT_U302B_DRV -> EMG2_RECT_U402B_DRV`, `EMG1_RECT_SUM1 -> EMG2_RECT_SUM1`, and `EMG1_RECT_SUM2 -> EMG2_RECT_SUM2`.
- Shared rails remain shared: `3V3_ADC`, `analog VREF`, and GND.
- Do not introduce `ADC_REF` in the Channel 2 analog sheet.
- Root/U201 plan: add root sheet instance for `EMG_CHANNEL_2_ANALOG`, add/connect `EMG2_RAW`, `EMG2_RECT`, and `EMG2_ENV`, then connect `EMG2_RAW -> U201 CH3`, `EMG2_RECT -> U201 CH4`, and `EMG2_ENV -> U201 CH5` through `POWER_REFERENCE_BLOCK`.
- Preserve SPI placeholders: CLK, Din, and `~CS/SHDN` remain expected until DSTK SPI is connected.
- Channel 2 inherits Channel 1 BOM/footprint decisions: `BAS70ZFILM` / `Device:D_Schottky` / `Diode_SMD:D_SOD-123` for D431/D432, MCP6004 / `Package_DIP:DIP-14_W7.62mm` for U402, `R_0805_2012Metric` for RECT ratio resistors, and `C_0805_2012Metric` for relevant capacitors.
- Expected ERC after implementation: U201 CH3/CH4/CH5 input-not-driven errors should disappear; U201 CLK/Din/`~CS/SHDN` placeholder errors may remain.
- No D431/D432 pin-not-connected errors, no `ADC_REF`, no No ERC markers, and no new real Channel 1/Channel 2 errors should be introduced.
- Risks before edit: copied UUIDs must be regenerated or safely unique, `EMG2_REF_ELECTRODE` must remain isolated from GND/chassis/USB, generic internal labels such as `RAW_HPF_NODE`, `RAW_GAIN_FB`, and `ENV_LPF_NODE` should become EMG2-prefixed for readability, D431/D432 orientation must be rechecked after copy, and Channel 1 must not be functionally changed.

Current root ERC status after the Channel 1 RECT/ENV footprint field assignment:

- Remaining ERC errors: 6 expected placeholder errors.
- Remaining ERC warnings: 21.
- Real new ERC errors after RECT implementation: 0.
- `U201` CH3 input not driven.
- `U201` CH4 input not driven.
- `U201` CH5 input not driven.
- `U201` CLK input not driven.
- `U201` Din input not driven.
- `U201` `~CS/SHDN` input not driven.
- These are intentional for now because Channel 2 and DSTK SPI are not connected yet.
- After Channel 2 implementation, CH3/CH4/CH5 placeholder errors should disappear if `EMG2_RAW`, `EMG2_RECT`, and `EMG2_ENV` are connected to U201.
- SPI placeholder errors may remain expected until DSTK SPI is connected.
- GUI ERC after footprint assignment showed 6 errors and 21 warnings.
- The 6 errors are the expected U201 placeholder input errors: CH3, CH4, CH5, CLK, Din, and `~CS/SHDN`.
- The temporary D331/D332 pin-not-connected errors were fixed.
- No new D331/D332 pin-not-connected errors appeared after footprint assignment.
- No new real RECT/ENV ERC errors appeared after footprint assignment.
- Warnings remain, including symbol/library mismatch and existing placeholder/single-pin label warnings.
- These warnings do not approve hardware and can be reviewed separately.
- Do not add No ERC markers for these placeholders yet.

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
- Channel 2 may start as a controlled schematic duplicate/adaptation of Channel 1.
- Channel 2 remains **NOT STARTED** in the repository at this checkpoint.
- PCB remains **NOT STARTED**.
- DSTK SPI/power remains **NOT CONNECTED**.
- This is not final hardware approval and not production ready.

## Important

This is not a finished V5 fabrication layout yet.

The V5 schematic and PCB still need the planned hardware expansion:

- second EMG channel
- Channel 2 controlled schematic duplication/adaptation from Channel 1
- RAW/RECT/ENV analog stages for Channel 2
- DSTK22807 carrier footprint
- power/regulator/BAT_MON details
- V5 test points and silkscreen safety warnings

PCB layout must not start for the DSTK22807 carrier until the physical board is measured and pin labels are confirmed.

## Next Actions

- Start Channel 2 schematic implementation only from the approved duplication/adaptation plan.
- Review D331/D332 `BAS70ZFILM` / `Diode_SMD:D_SOD-123` cathode-band orientation visually against the schematic before PCB.
- Review the U302 `MCP6004-I/P` / `Package_DIP:DIP-14_W7.62mm` assignment alongside the remaining MCP6004 symbol/library mismatch warning.
- Lock exact ordering MPNs for R333/R334/R336/R337/R338 as 0.1% thin-film 0805 from the same series where possible; keep R335 as 1% thin-film 0805 unless BOM simplification or sensitivity results justify 0.1%.
- Lock exact ordering MPNs for C321/C331 as 1nF C0G/NP0 0805, C351 as 4.7nF C0G/NP0 0805 preferred, and C341/C360 as X7R 0805 with suitable voltage ratings before Channel 2 or PCB.
- Confirm ENV behavior and analog VREF buffer loading after RECT/capacitor decisions.
- After Channel 2 is implemented, rerun root ERC; CH3/CH4/CH5 placeholder errors should disappear if EMG2 RAW/RECT/ENV are connected to U201, while SPI placeholder errors may remain expected until DSTK SPI is connected.
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
