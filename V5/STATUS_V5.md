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

- Select and review a valid analog rectifier / absolute-value topology around `analog VREF`.
- Target remains `EMG1_RECT = analog VREF + abs(EMG1_RAW_DRV - analog VREF)`.
- After RECT is selected, update Channel 1 and review again before Channel 2 duplication.
- Do not start PCB layout yet.
- Do not start Channel 2 yet.
- Do not connect DSTK22807 power/SPI nets until pinout and power behavior are verified.
- Do not proceed to PCB until RECT topology and Channel 1 schematic review are complete.

## Next Required Input

Measure or photograph the DSTK22807 board clearly enough to confirm:

- board length and width
- header/castellated pitch
- exact pin labels
- antenna location and keepout
- USB connector overhang
- usable SPI-capable pins
