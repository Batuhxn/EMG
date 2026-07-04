# EMG V5 Status

Date: 2026-07-04

## Current State

The V5 folder now contains a clean separate KiCad working base copied from the verified V4 project, plus V5 documentation and ESP32-H2 firmware scaffold.

Current schematic checkpoint:

- `adea50a fix channel 1 analog polarity and block rectifier placeholder`
- Root schematic: `V5/EMG_v5.kicad_sch`
- Power/reference child sheet: `V5/POWER_REFERENCE_BLOCK.kicad_sch`
- Channel 1 analog child sheet: `V5/EMG_CHANNEL_1_ANALOG.kicad_sch`
- Root schematic now includes the `EMG_CHANNEL_1_ANALOG` hierarchical sheet.
- `POWER_REFERENCE_BLOCK` now receives `EMG1_RAW`, `EMG1_RECT`, and `EMG1_ENV` as hierarchical inputs.

Current schematic status:

- Power/reference schematic block: **ADDED / REVIEWED ENOUGH FOR CURRENT PLANNING**
- Channel 1 analog schematic candidate: **POLARITY FIXED / RECT BLOCKED PLACEHOLDER**
- Channel 2 analog schematic: **NOT STARTED / NO**
- PCB layout: **NOT STARTED / NO**
- Full two-channel analog EMG chain: **NOT COMPLETE**
- DSTK external 3.3 V powering: **NOT APPROVED YET**
- USB plus external 3.3 V simultaneous connection: **NOT APPROVED**
- Human USB testing: **FORBIDDEN**
- 5V analog/ADC: **NOT APPROVED**

Channel 1 analog review/fix status at checkpoint `adea50a`:

- J301 TRS mapping is fixed: Tip/T = `EMG1_IN_P`, Ring/R = `EMG1_IN_N`, Sleeve/S = `EMG1_REF_ELECTRODE`.
- U301 INA polarity is fixed: IN+ = `EMG1_IN_P_PROT`, IN- = `EMG1_IN_N_PROT`.
- U302A RAW gain topology is corrected to non-inverting feedback.
- U302D ENV buffer topology is corrected.
- U302B and U302C are safe reserved followers.
- RECT topology is **NOT IMPLEMENTED**.
- The previous questionable RECT active topology was removed.
- `EMG1_RECT_DRV` is temporarily biased to `analog VREF` through `1M R_RECT_PLACEHOLDER`.
- This is an ERC-safe placeholder only; `EMG1_RECT` is not a functional rectified output yet.
- `EMG1_ENV` is not a functional envelope output yet because it depends on future RECT implementation.
- Do not duplicate this placeholder to Channel 2 as a real rectifier.
- Do not proceed to PCB until RECT topology is selected and reviewed.

Current root ERC status after the Channel 1 analog polarity/RECT-placeholder fix pass:

- Remaining ERC errors: 6 expected placeholder errors.
- Real ERC errors: 0.
- `U201` CH3 input not driven.
- `U201` CH4 input not driven.
- `U201` CH5 input not driven.
- `U201` CLK input not driven.
- `U201` Din input not driven.
- `U201` `~CS/SHDN` input not driven.
- These are intentional for now because Channel 2 and DSTK SPI are not connected yet.
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

KiCad status remains unchanged:

- Channel 1 analog schematic remains **POLARITY FIXED / RECT BLOCKED PLACEHOLDER**.
- KiCad schematic has **NOT** been updated with the rectifier topology.
- `EMG1_RECT_DRV` in KiCad is still placeholder-biased to `analog VREF` through `1M R_RECT_PLACEHOLDER`.
- `EMG1_RECT` and `EMG1_ENV` are still not functional in the KiCad schematic.
- Channel 2 remains **NOT STARTED**.
- PCB remains **NOT STARTED**.

## Important

This is not a finished V5 fabrication layout yet.

The V5 schematic and PCB still need the planned hardware expansion:

- second EMG channel
- valid Channel 1 RECT topology selection and review
- RAW/RECT/ENV analog stages for Channel 2
- DSTK22807 carrier footprint
- power/regulator/BAT_MON details
- V5 test points and silkscreen safety warnings

PCB layout must not start for the DSTK22807 carrier until the physical board is measured and pin labels are confirmed.

## Next Actions

- Review the real and conservative LTspice `.cir` files manually before any KiCad edit.
- Target remains `EMG1_RECT = analog VREF + abs(EMG1_RAW_DRV - analog VREF)`.
- Confirm diode orientation, resistor ratios, op-amp stage behavior, output range, crossover behavior, and model limitations before considering a KiCad edit prompt.
- Do not start PCB layout yet.
- If a KiCad edit prompt is created later, it must be limited to replacing the RECT placeholder in Channel 1 only.
- Do not start Channel 2 yet.
- Do not connect DSTK22807 power/SPI nets until pinout and power behavior are verified.
- Do not treat the LTspice candidate as final hardware approval; do not proceed to PCB until RECT topology and Channel 1 schematic review are complete.

## Next Required Input

Measure or photograph the DSTK22807 board clearly enough to confirm:

- board length and width
- header/castellated pitch
- exact pin labels
- antenna location and keepout
- USB connector overhang
- usable SPI-capable pins
