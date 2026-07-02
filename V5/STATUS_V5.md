# EMG V5 Status

Date: 2026-07-02

## Current State

The V5 folder now contains a clean separate KiCad working base copied from the verified V4 project, plus V5 documentation and ESP32-H2 firmware scaffold.

Current documentation sync:

- `1f759a6 add power reference schematic block`
- Small power/reference block added as a hierarchical child sheet.
- Root: `V5/EMG_v5.kicad_sch`
- Child: `V5/POWER_REFERENCE_BLOCK.kicad_sch`
- Project metadata: `V5/EMG_v5.kicad_pro`

Current power/reference schematic status:

- Power/reference schematic block: **ADDED / NEEDS REVIEW**
- PCB layout: **NOT STARTED / NO**
- Analog EMG chain: **NOT STARTED / NO**
- DSTK external 3.3 V powering: **NOT APPROVED YET**
- USB plus external 3.3 V simultaneous connection: **NOT APPROVED**
- Human USB testing: **FORBIDDEN**
- 5V analog/ADC: **NOT APPROVED**

Previous clean V5 base checks, before the `POWER_REFERENCE_BLOCK.kicad_sch` child-sheet addition:

- ERC: 0 violations on the clean copied V5 base.
- DRC: 0 violations on the clean copied V5 base.
- These results do not yet certify the new `V5/POWER_REFERENCE_BLOCK.kicad_sch` child schematic.
- `V5/POWER_REFERENCE_BLOCK.kicad_sch` still needs visual/electrical review.
- A later KiCad ERC review is still required after the child schematic review.

## Important

This is not a finished V5 fabrication layout yet.

The V5 schematic and PCB still need the planned hardware expansion:

- second EMG channel
- RAW/RECT/ENV analog stages for both channels
- MCP3208 ADC
- DSTK22807 carrier footprint
- power/regulator/BAT_MON details
- V5 test points and silkscreen safety warnings

PCB layout must not start for the DSTK22807 carrier until the physical board is measured and pin labels are confirmed.

## Next Actions

- Review child schematic `V5/POWER_REFERENCE_BLOCK.kicad_sch`.
- Later update/clean only if needed after review.
- Do not start PCB layout yet.
- Do not start analog EMG chain schematic yet.
- Do not connect DSTK22807 power/SPI nets until pinout and power behavior are verified.

## Next Required Input

Measure or photograph the DSTK22807 board clearly enough to confirm:

- board length and width
- header/castellated pitch
- exact pin labels
- antenna location and keepout
- USB connector overhang
- usable SPI-capable pins
