# EMG V5 Status

Date: 2026-07-04

## Current State

The V5 folder now contains a clean separate KiCad working base copied from the verified V4 project, plus V5 documentation and ESP32-H2 firmware scaffold.

Current schematic checkpoint:

- `8428e5e add channel 1 analog schematic candidate`
- Root schematic: `V5/EMG_v5.kicad_sch`
- Power/reference child sheet: `V5/POWER_REFERENCE_BLOCK.kicad_sch`
- Channel 1 analog child sheet: `V5/EMG_CHANNEL_1_ANALOG.kicad_sch`
- Root schematic now includes the `EMG_CHANNEL_1_ANALOG` hierarchical sheet.
- `POWER_REFERENCE_BLOCK` now receives `EMG1_RAW`, `EMG1_RECT`, and `EMG1_ENV` as hierarchical inputs.

Current schematic status:

- Power/reference schematic block: **ADDED / REVIEWED ENOUGH FOR CURRENT PLANNING**
- Channel 1 analog schematic candidate: **ADDED / NEEDS REVIEW**
- Channel 2 analog schematic: **NOT STARTED / NO**
- PCB layout: **NOT STARTED / NO**
- Full two-channel analog EMG chain: **NOT COMPLETE**
- DSTK external 3.3 V powering: **NOT APPROVED YET**
- USB plus external 3.3 V simultaneous connection: **NOT APPROVED**
- Human USB testing: **FORBIDDEN**
- 5V analog/ADC: **NOT APPROVED**

Current root ERC status after channel 1 analog schematic checkpoint and child-sheet PWR_FLAG cleanup:

- Remaining ERC errors: 6 expected placeholder errors.
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
- Channel 1 analog review pass
- RAW/RECT/ENV analog stages for Channel 2
- DSTK22807 carrier footprint
- power/regulator/BAT_MON details
- V5 test points and silkscreen safety warnings

PCB layout must not start for the DSTK22807 carrier until the physical board is measured and pin labels are confirmed.

## Next Actions

- Review `V5/EMG_CHANNEL_1_ANALOG.kicad_sch` before Channel 2 or PCB work.
- Review INA333 pinout, MCP6004 unit usage, rectifier topology, biasing, ADC output ranges, and saturation risk.
- Do not start PCB layout yet.
- Do not start Channel 2 yet.
- Do not connect DSTK22807 power/SPI nets until pinout and power behavior are verified.
- Do not proceed to PCB until schematic review is complete.

## Next Required Input

Measure or photograph the DSTK22807 board clearly enough to confirm:

- board length and width
- header/castellated pitch
- exact pin labels
- antenna location and keepout
- USB connector overhang
- usable SPI-capable pins
