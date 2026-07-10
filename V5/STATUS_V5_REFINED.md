# EMG V5 Status - Refined

Date: 2026-07-10

## Current Snapshot

- Branch: `v5/KiCad`
- HEAD: `70feb41 sync power helper symbols and refine schematic layout`
- Tracking: `origin/v5/KiCad`, ahead/behind `0 / 0`
- Root schematic: `V5/EMG_v5.kicad_sch`
- Development state: controlled schematic review
- PCB readiness: not claimed
- Final hardware or human-test approval: not granted

## Implemented Architecture

The tracked V5 schematic contains two analog EMG channels. Each channel produces `RAW`, `RECT`, and `ENV`.

MCP3208 allocation:

| Channel | Net |
|---|---|
| CH0-CH2 | `EMG1_RAW`, `EMG1_RECT`, `EMG1_ENV` |
| CH3-CH5 | `EMG2_RAW`, `EMG2_RECT`, `EMG2_ENV` |
| CH6 | `VREF_MON` |
| CH7 | `BAT_MON` |

The measured DSTK22807 symbol and carrier footprint retain one-to-one pin/pad numbering 1 through 18.

## Implemented First-Validation Power

Current tracked power path:

`J201 -> SW201 -> Q201 -> LDO_IN -> U203 -> R207 -> 3V3_ADC`

- Q201 is the `PMV48XP` pack-level reverse-polarity PMOS.
- U203 is `TPS7A2033PDBVR`.
- R207 is the `0R` link between U203 OUT and `3V3_ADC`.

Implemented BAT_MON network:

- Source: protected `LDO_IN`
- `R204 = 68k`
- `R205 = 100k`
- `C211 = 10nF`
- Destination: MCP3208 CH7

The following legacy placeholders are absent from current tracked V5 schematics: `JP201`, `JP202`, `DSTK_3V3_CANDIDATE`, and `LDO_3V3_FALLBACK`.

## Current ERC

- 0 errors
- 1 warning related to unresolved `EMG_V5` footprint-library configuration

This warning is not an electrical design error.

## Open Constraints

- Powered DSTK22807 with unpowered/collapsing MCP3208 remains an unresolved SPI backfeed condition.
- DSTK 3V3 and 5V/VBUS are not analog/ADC supply sources.
- `ADC_REF` and `analog VREF` remain separate.
- Existing candidate values such as `R206 = 0R/10R candidate` remain unresolved.
- No manufacturer-level correctness claim is made where local datasheet or populated MPN evidence is absent.
- Human-connected testing remains battery-only and is not yet approved.

Highest-risk open engineering items:

- Powered-DSTK/unpowered-MCP3208 SPI backfeed needs a hardware-safe resolution and bench evidence.
- The TPS7A2033 direction is sized against approximately 1.6 mA typical, 2.5 mA conservative, 10 mA validation-budget, and 50 mA planning-floor estimates; actual current, dropout, noise, temperature, capacitor stability, and reverse current remain to be measured.
- BAT_MON uses the 68k/100k/10nF network and requires discard-first, at least 5 ms settling, and multi-sample averaging during first validation.
- ADC acquisition/source impedance, MCP3208 VDD/VREF relationship, analog-VREF stability, `VREF_MON` disturbance, and `ADC_REF` ripple remain open.
- Both analog channels are implemented, but rectifier evidence uses generic op-amp/diode models; diode orientation, MCP6004 3.3 V behavior, and C341/C441 stability/derating remain open.
- Human-connected operation requires battery-only power with USB, bench supply, mains-connected instruments, and earth-referenced oscilloscope connections absent; this is not medical-device or human-test approval.

## PCB State

`V5/EMG_v5.kicad_pcb` is a legacy, unsynchronized V4-style board. It does not represent the current V5 schematic and does not establish PCB readiness.

PCB entry remains blocked by missing/final footprints and MPNs, diode orientation, analog/ADC/VREF validation, test-point and safety-silkscreen definition, measured carrier placement, antenna keepout, USB overhang, and creation of a current V5-synchronized board.
