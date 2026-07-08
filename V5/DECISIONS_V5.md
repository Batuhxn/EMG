# V5 Decisions Log

Date: 2026-07-08

## Purpose

This file records the current V5 design decisions for the EMG project. It separates locked decisions, provisional decisions, rejected or blocked options, and gates that must close before PCB work.

This document is not schematic implementation, PCB implementation, BOM lock, fabrication approval, or human-test approval. It is the decision record that should guide the next controlled steps.

Current repository baseline for this decision log:

- Branch: `v5/KiCad`
- HEAD: `bc5ff05 sync docs with 3xaa ldo target review`

## Locked Decisions

### Project Line

- V4 is preserved.
- V5 continues as a separate development line.
- The current V5 work remains controlled schematic/planning work until explicit approval is given for broader implementation.
- PCB layout has not started and is not approved to start.

### Controller / Wireless Module

- The first V5 prototype uses the DSTK22807 ESP32-H2 development board.
- A bare ESP32-H2-MINI module footprint is not used for the first V5 prototype.
- The DSTK22807 is mounted through a removable header/socket style carrier for first prototype bring-up.
- The DSTK22807 carrier footprint is geometry/mechanical planning only; signal labels are not embedded into the footprint geometry.

### ADC

- MCP3208 is the selected ADC.
- First prototype MCP3208 package direction: `MCP3208-CI/P` PDIP-16.
- KiCad symbol: `Analog_ADC:MCP3208`.
- KiCad footprint: `Package_DIP:DIP-16_W7.62mm`.
- MCP3208 TSSOP is not used unless an exact official MPN and datasheet-backed package/pinout proof are found later.

Locked MCP3208 channel mapping:

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

### Analog Signal Strategy

- V5 physically produces `RAW`, `RECT`, and `ENV` nodes for each channel.
- Channel 1 and Channel 2 use the hierarchical analog sheet structure.
- `ADC_REF` and `analog VREF` must remain separate.
- `ADC_REF` is the MCP3208 ADC full-scale reference.
- `analog VREF` is the analog front-end midscale bias/reference node.
- `EMG1_REF_ELECTRODE` and `EMG2_REF_ELECTRODE` must not be tied to GND, chassis, or USB.

### Channel Connector and Gain Fixes

- TRS mapping is fixed:
  - Tip/T -> `EMGx_IN_P`
  - Ring/R -> `EMGx_IN_N`
  - Sleeve/S -> `EMGx_REF_ELECTRODE`
- Physical cable observation is recorded as:
  - yellow tip -> Tip/T
  - green middle body -> Ring/R
  - red rear body -> Sleeve/S
- RAW gain polarity is fixed:
  - Channel 1 U302A: non-inverting input from `RAW_HPF_NODE`, inverting input from `RAW_GAIN_FB`.
  - Channel 2 U402A: non-inverting input from `EMG2_RAW_HPF_NODE`, inverting input from `EMG2_RAW_GAIN_FB`.

### Rectifier / Envelope First-Prototype Direction

- D331/D332/D431/D432 first-prototype diode direction:
  - Value / MPN direction: `BAS70ZFILM`
  - Symbol: `Device:D_Schottky`
  - Footprint: `Diode_SMD:D_SOD-123`
- Do not use `BAS70FILM` with a plain two-pin diode symbol plus generic three-pad SOT-23 footprint for the first prototype.
- Avoid `BAS70-04`, `BAS70-05`, `BAS70-06`, and BAT54A/C/S dual/common variants for this rectifier pass.
- U302/U402 first-prototype op-amp package direction remains socketed/inspectable:
  - U302 direction: `MCP6004-I/P`, `Package_DIP:DIP-14_W7.62mm`
  - U402 direction: MCP6004, `Package_DIP:DIP-14_W7.62mm`
- Exact ordering MPNs for op-amps, passives, and capacitors remain open.

### Human-Contact Safety Boundary

When electrodes are attached to a human:

- Use battery-isolated operation only.
- Disconnect USB.
- Do not use USB power, 5 V power, bench-supply power, mains-connected equipment, or oscilloscope earth.
- Do not connect `REF_ELECTRODE` nets to GND, chassis, or USB.

Battery operation is required for human-contact testing, but it is not by itself final safety approval.

## Provisional Decisions

### DSTK22807 Pinout and SPI

Observed DSTK22807 pin mapping:

| Footprint Pad | Observed / Provisional Label | Status |
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
| 10 | 5V | Confirmed by right-row order review |
| 11 | GND | Confirmed by right-row order review |
| 12 | 3V3 | Confirmed by right-row order review |
| 13 | GPIO14 | Confirmed by right-row order review |
| 14 | GPIO13 | Confirmed by right-row order review |
| 15 | GPIO12 | Confirmed by right-row order review |
| 16 | GPIO11 | Confirmed by right-row order review |
| 17 | GPIO10 | Confirmed by right-row order review |
| 18 | GPIO9 | Confirmed by right-row order review |

DSTK SPI-only integration is connected as:

| SPI Net | U501 GPIO | U501 Pin |
| --- | --- | --- |
| `ADC_CS` | GPIO14 | Pin 13 |
| `ADC_SCLK` | GPIO13 | Pin 14 |
| `ADC_MOSI` | GPIO12 | Pin 15 |
| `ADC_MISO` | GPIO11 | Pin 16 |

This is approved only as SPI logic integration. It does not approve DSTK 3V3 or 5V/VBUS as an analog/ADC supply.

U501 GND / pin 11 is connected to root/system GND for the SPI logic reference. This is not a power-source approval.

### DSTK Power Status

- USB-powered measurement observed approximately:
  - 5V/VBUS: 5.125 V to 5.126 V
  - 3V3: 3.291 V to 3.295 V
- Unpowered 3V3 rail decay was observed and is consistent with capacitor discharge/leakage.
- DSTK 3V3 current capacity, noise, BLE/RF load behavior, and backfeed behavior remain unresolved.
- DSTK external 3.3 V powering is not approved.
- USB plus external 3.3 V simultaneous connection is not approved.
- DSTK 5V/VBUS is not approved for analog/ADC rails.

### First-Validation Battery / Regulator Direction

Current first-validation candidate:

`3xAA alkaline battery pack -> switch/protection -> low-noise 3.3 V LDO -> 3V3_ADC`

Decision scope:

- This is a first analog validation candidate only.
- This is not a final product battery decision.
- The final product battery candidate remains deferred, with 1S protected Li-ion/LiPo still possible later.
- 3xAA NiMH is outside this decision because the lower nominal pack voltage gives much narrower LDO dropout margin.
- Regulator MPN, package, footprint, BOM item, and schematic implementation are not selected or approved.

Current LDO target review:

- Current capability target: at least 50 mA for first validation, pending actual current budget.
- Higher-current LDO classes may be considered if noise, dropout, thermal behavior, quiescent current, reverse-current behavior, capacitor stability, and package suitability are acceptable.
- Preliminary dropout target: <=300 mV at the selected current budget.
- Preferred dropout target: <=200 mV at the measured expected analog/ADC load.
- Vin max target: >=5.5 V preferred minimum; >=6.0 V is more comfortable for fresh 3xAA alkaline margin.
- Fresh 3xAA alkaline pack voltage is treated as approximately 4.5 V to 4.8 V for margin review.

### Power and Reference Architecture

- `3V3_ADC` remains the selected analog/ADC rail name.
- MCP3208 VDD is planned to be powered from the selected `3V3_ADC` source.
- MCP3208 VREF remains `ADC_REF` and must stay within MCP3208 VDD limits.
- `ADC_REF` remains separate from `analog VREF`.
- `analog VREF` remains the analog front-end bias/reference node.
- `VREF_MON` measures `analog VREF` on MCP3208 CH6.
- `BAT_MON` divider/scaling remains unresolved.
- `AGND` and `DGND` should be handled as controlled return strategy on a common ground system, not as blindly split floating grounds.

## Rejected / Blocked Options

The following are rejected or blocked at the current checkpoint:

- Starting PCB layout.
- Treating the current schematic candidates as final hardware approval.
- Using DSTK 3V3 as `3V3_ADC` without load, noise, and backfeed review.
- Using DSTK 5V/VBUS as an analog/ADC rail.
- USB-powered human-contact EMG testing.
- Human-contact testing while USB is connected.
- Tying `EMG1_REF_ELECTRODE` or `EMG2_REF_ELECTRODE` to GND, chassis, or USB.
- Merging `ADC_REF` and `analog VREF`.
- Shorting a local regulator output directly to DSTK 3V3.
- Allowing two 3.3 V sources to be populated/closed at the same time in any future source-selection scheme.
- Suppressing the `DSTK_3V3_CANDIDATE` planning warning with a No ERC marker at this checkpoint.
- Selecting regulator MPN/package/footprint/BOM without datasheet review and current budget.
- Implementing the 3xAA LDO path in schematic without a separate approved plan.
- Using a bare ESP32-H2 module for the first V5 prototype.
- Using MCP3208 TSSOP without exact MPN/package/pinout proof.

## Decision Tree

Current high-level path:

1. Keep V5 as controlled schematic/planning work.
2. Preserve the implemented two-channel analog and DSTK SPI candidate schematics.
3. Do not start PCB until power, safety, and component gates close.
4. Use 3xAA alkaline plus low-noise 3.3 V LDO as the first-validation power candidate.
5. Review candidate LDOs against current, dropout, noise, PSRR, stability, reverse-current, thermal, package, and availability requirements.
6. Keep DSTK 3V3 and 5V/VBUS disconnected from analog/ADC rails unless a later explicit review approves a source strategy.
7. If a future selectable source strategy is used, make the selection mutually exclusive by design and by population rule.

Power-source decision branch:

```text
Need 3V3_ADC source
|
+-- DSTK 3V3 direct?
|   +-- Current decision: blocked
|   +-- Required first: load/noise/current/backfeed/BLE behavior review
|
+-- DSTK 5V/VBUS?
|   +-- Current decision: rejected for analog/ADC rails
|
+-- Local battery-side regulator?
    +-- Current decision: preferred first-validation candidate
    +-- First path: 3xAA alkaline -> switch/protection -> low-noise 3.3 V LDO -> 3V3_ADC
    +-- Required first: regulator review, current budget, BAT_MON review, schematic plan approval
```

Human-test decision branch:

```text
Electrodes attached to human?
|
+-- Yes
|   +-- Battery isolated only
|   +-- USB disconnected
|   +-- No bench supply, mains equipment, USB power, 5 V power, or oscilloscope earth
|   +-- REF_ELECTRODE nets isolated from GND/chassis/USB
|
+-- No
    +-- Bench measurements may be planned separately
    +-- Still do not approve unresolved power/backfeed paths by implication
```

## Open Gates Before PCB

PCB work must not start until these gates are reviewed and accepted:

- Confirm the intended `3V3_ADC` source strategy.
- Select and review a 3.3 V regulator candidate, if the local LDO path proceeds.
- Lock regulator MPN, suffix, package, footprint, capacitor requirements, and BOM entry.
- Estimate or measure analog/ADC current budget.
- Confirm 3xAA alkaline minimum usable voltage under expected load and selected LDO dropout.
- Review `BAT_MON` divider values for fresh 3xAA maximum voltage, ADC input range, source impedance, and sampling behavior.
- Confirm MCP3208 VDD / `ADC_REF` relationship.
- Review `ADC_REF` filtering, decoupling, ripple/noise, and startup behavior.
- Review `analog VREF` buffer stability, loading, noise, and interaction with ENV capacitors.
- Confirm MCP6004 input common-mode range and output swing at `3V3_ADC = 3.3 V`.
- Confirm MCP6004 output stability with ADC/output RC loads and ENV storage load.
- Visually verify D331/D332/D431/D432 cathode-band orientation against the SOD-123 footprint.
- Lock exact ordering MPNs for ratio-critical rectifier resistors.
- Lock exact ordering MPNs and dielectric/voltage choices for analog capacitors.
- Confirm DSTK power-path behavior before any DSTK rail is considered as a source.
- Define a human-contact safety procedure.
- Confirm enclosure/isolation/mechanical safety expectations for any later human test.
- Review all remaining ERC warnings and decide whether they are acceptable or must be closed by real design changes.

## Historical Notes

Relevant recent checkpoints:

- `bc5ff05 sync docs with 3xaa ldo target review`
- `2f772d8 sync docs with 3xaa ldo requirements`
- `854e438 sync docs with first 3xaa battery decision`
- `eba22c8 sync docs with local 3v3 adc regulator plan`
- `54d9089 sync docs with dstk power strategy`
- `79730dd connect dstk spi and mark unused pins`
- `3494631 sync kicad metadata and analog label placement`
- `1e5d3b0 fix channel trs mapping and raw gain input polarity`
- `080eef1 remove legacy root emg schematic block`
- `60d5881 implement channel 2 analog schematic`

Historical cleanup notes:

- MCP6004 and `Device:D_Schottky` metadata mismatch warnings were cleaned as metadata/library-cache cleanup, not topology changes.
- U202B unused op-amp unit was placed as a safe unity follower using `analog VREF`; it is not connected into `ADC_REF`, `3V3_ADC`, GND, `VREF_MON`, `BAT_MON`, or another signal path.
- Earlier ERC counts with U201 SPI placeholder errors are superseded by the DSTK SPI integration.
- Latest documented ERC status is 0 errors and 2 known warnings:
  - U501 footprint library warning from CLI configuration.
  - `DSTK_3V3_CANDIDATE` isolated pin label.

Source documents that fed this decision log include:

- `V5/STATUS_V5.md`
- `V5/DSTK22807_PHYSICAL_PINOUT_OBSERVATION.md`
- `V5/DSTK22807_SPI_PIN_REVIEW.md`
- `V5/DSTK22807_POWER_PIN_MEASUREMENT.md`
- `V5/FIRST_SCHEMATIC_POWER_SOURCE_STRATEGY.md`
- `V5/POWER_REFERENCE_ARCHITECTURE_REVIEW.md`
- `V5/POWER_REFERENCE_SCHEMATIC_BLOCK_PROPOSAL.md`
- `V5/POWER_REFERENCE_COMPONENT_CANDIDATES.md`
- `V5/BOM_V5_DRAFT.md`
- `V5/sim/rectifier/RECTIFIER_MODEL_CONFIRMATION.md`
- `V5/EMG_CHANNEL_1_ANALOG.kicad_sch`
- `V5/EMG_CHANNEL_2_ANALOG.kicad_sch`
- `V5/POWER_REFERENCE_BLOCK.kicad_sch`
- `V5/EMG_v5.kicad_sch`

Current final decision marker:

**3XAA_LDO_CURRENT_DROPOUT_TARGETS_REVIEWED_NOT_IMPLEMENTED**
