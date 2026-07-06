# EMG V5 Status

Date: 2026-07-04

## Current State

The V5 folder now contains a clean separate KiCad working base copied from the verified V4 project, plus V5 documentation and ESP32-H2 firmware scaffold.

Current schematic checkpoint:

- `3825181 cleanup channel 1 rectifier schematic readability`
- Root schematic: `V5/EMG_v5.kicad_sch`
- Power/reference child sheet: `V5/POWER_REFERENCE_BLOCK.kicad_sch`
- Channel 1 analog child sheet: `V5/EMG_CHANNEL_1_ANALOG.kicad_sch`
- Root schematic now includes the `EMG_CHANNEL_1_ANALOG` hierarchical sheet.
- `POWER_REFERENCE_BLOCK` now receives `EMG1_RAW`, `EMG1_RECT`, and `EMG1_ENV` as hierarchical inputs.

Current schematic status:

- Power/reference schematic block: **ADDED / REVIEWED ENOUGH FOR CURRENT PLANNING**
- Channel 1 RECT KiCad candidate: **IMPLEMENTED / REVIEWED / CLEANED READABILITY / NOT FINAL HARDWARE APPROVAL**
- Channel 2 analog schematic: **NOT STARTED / NO**
- PCB layout: **NOT STARTED / NO**
- Full two-channel analog EMG chain: **NOT COMPLETE**
- DSTK external 3.3 V powering: **NOT APPROVED YET**
- USB plus external 3.3 V simultaneous connection: **NOT APPROVED**
- Human USB testing: **FORBIDDEN**
- 5V analog/ADC: **NOT APPROVED**

Channel 1 analog / RECT implementation status at checkpoint `3825181`:

- Channel 1 analog schematic remains polarity fixed.
- `V5/EMG_CHANNEL_1_ANALOG.kicad_sch` now includes a reviewable Channel 1 VREF-centered absolute-value rectifier candidate.
- RECT is no longer only a blocked placeholder in the KiCad schematic.
- Channel 1 RECT KiCad candidate: **IMPLEMENTED / REVIEWED / CLEANED READABILITY / NOT FINAL HARDWARE APPROVAL**.
- Checkpoint `3825181 cleanup channel 1 rectifier schematic readability` applied the Channel 1 RECT schematic readability cleanup.
- Cleanup target was `V5/EMG_CHANNEL_1_ANALOG.kicad_sch` only.
- RECT block readability was improved without intentional topology/function change.
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
- `EMG1_RECT` and `EMG1_ENV` are no longer merely placeholder-only in KiCad, but still need review/validation before Channel 2 or PCB.
- Channel 2 remains **NOT STARTED**.
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
- D331/D332 and MCP6004 symbol/library mismatch warnings should be reviewed.
- D331/D332 diode MPN/footprint direction is now recorded for first-prototype planning, but schematic footprint fields are not assigned yet.
- D331/D332 KiCad symbol convention: `Device:D_Schottky` uses pin 1 = K / cathode and pin 2 = A / anode.
- Current D331/D332 orientation matches the previous Channel 1 RECT review: D331 K -> `EMG1_RECT_U302B_DRV`, D331 A -> `EMG1_RECT_A1_OUT`; D332 K -> `EMG1_RECT_SUM1`, D332 A -> `EMG1_RECT_U302B_DRV`.
- Do not use `BAS70FILM` with a plain 2-pin `Device:D_Schottky` symbol plus generic 3-pad SOT-23 footprint for the first prototype.
- `BAS70FILM` is deferred because the ST SOT-23 single diode uses two active package pins plus a third unused/NC-style pad and needs explicit custom mapping or a project-local 3-pin symbol/footprint review.
- Recommended D331/D332 first-prototype direction: MPN `BAS70ZFILM`, package SOD-123, KiCad symbol `Device:D_Schottky`, KiCad footprint `Diode_SMD:D_SOD-123`.
- `BAS70ZFILM` is preferred because the two-terminal SOD-123 package avoids SOT-23 third-pad / NC ambiguity and maps cleanly to the 2-pin diode symbol.
- Avoid `BAS70-04`, `BAS70-05`, `BAS70-06`, and BAT54A/C/S dual/common variants for this rectifier pass.
- Before PCB/layout, visually verify D331/D332 cathode-band orientation against the KiCad footprint and rectifier schematic.
- MCP6004 footprint/BOM choice must be reviewed before PCB.
- Resistor tolerance/matching for the 10k/20k network should be specified.
- ENV/output cap notes were clarified to distinguish `C341 = 1uF to analog VREF`, `C351 = 4.7nF to GND`, and RAW/RECT output caps `C321/C331 = 1nF to GND`.
- Do not start Channel 2 until the cleaned Channel 1 RECT schematic is reviewed/accepted.
- Do not start PCB.

Final Channel 1 RECT status: **CHANNEL_1_RECT_IMPLEMENTED_REVIEWED_CLEANED_READABILITY_NOT_FINAL_HW_APPROVAL**.

D331/D332 diode MPN/footprint decision status: **CHANNEL_1_RECT_DIODE_MPN_FOOTPRINT_DECISION_READY_NOT_FINAL_HW_APPROVAL**.

Current root ERC status after the Channel 1 RECT KiCad candidate fix pass:

- Remaining ERC errors: 6 expected placeholder errors.
- Real new ERC errors after RECT implementation: 0.
- `U201` CH3 input not driven.
- `U201` CH4 input not driven.
- `U201` CH5 input not driven.
- `U201` CLK input not driven.
- `U201` Din input not driven.
- `U201` `~CS/SHDN` input not driven.
- These are intentional for now because Channel 2 and DSTK SPI are not connected yet.
- GUI ERC after diode fix showed 6 errors only.
- The 6 errors are the expected U201 placeholder input errors: CH3, CH4, CH5, CLK, Din, and `~CS/SHDN`.
- The temporary D331/D332 pin-not-connected errors were fixed.
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

- Channel 1 RECT KiCad candidate: **IMPLEMENTED / REVIEWED / CLEANED READABILITY / NOT FINAL HARDWARE APPROVAL**.
- KiCad schematic has been updated with the Channel 1 RECT candidate in `V5/EMG_CHANNEL_1_ANALOG.kicad_sch`.
- Checkpoint `3825181` applied readability cleanup only to `V5/EMG_CHANNEL_1_ANALOG.kicad_sch`.
- `EMG1_RECT_DRV` is no longer placeholder-biased to `analog VREF` through the old active 1M placeholder.
- `EMG1_RECT` and `EMG1_ENV` are no longer merely placeholder-only in KiCad, but still need review and validation before Channel 2 or PCB.
- Channel 2 remains **NOT STARTED**.
- PCB remains **NOT STARTED**.
- DSTK SPI/power remains **NOT CONNECTED**.
- This is not final hardware approval and not production ready.

## Important

This is not a finished V5 fabrication layout yet.

The V5 schematic and PCB still need the planned hardware expansion:

- second EMG channel
- cleaned Channel 1 RECT review acceptance
- RAW/RECT/ENV analog stages for Channel 2
- DSTK22807 carrier footprint
- power/regulator/BAT_MON details
- V5 test points and silkscreen safety warnings

PCB layout must not start for the DSTK22807 carrier until the physical board is measured and pin labels are confirmed.

## Next Actions

- Review and accept the cleaned Channel 1 RECT schematic.
- Review and assign D331/D332 `BAS70ZFILM` / `Diode_SMD:D_SOD-123` only after cathode-band orientation is visually checked against the schematic.
- Review symbol/footprint/BOM choices for MCP6004.
- Confirm resistor matching/tolerance plan for the 10k/20k rectifier network.
- Confirm ENV behavior after RECT implementation.
- Do not duplicate to Channel 2 until cleaned Channel 1 RECT review is accepted.
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
