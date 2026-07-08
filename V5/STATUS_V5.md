# EMG V5 Status

Date: 2026-07-08

## Current Snapshot

- Branch: `v5/KiCad`
- Current HEAD: `bc5ff05 sync docs with 3xaa ldo target review`
- Repository baseline before documentation refinement: clean
- Root schematic: `V5/EMG_v5.kicad_sch`
- Power/reference child sheet: `V5/POWER_REFERENCE_BLOCK.kicad_sch`
- Channel 1 analog child sheet: `V5/EMG_CHANNEL_1_ANALOG.kicad_sch`
- Channel 2 analog child sheet: `V5/EMG_CHANNEL_2_ANALOG.kicad_sch`
- PCB layout: not started
- Final hardware approval: not granted

The V5 project is a separate KiCad development line derived from the verified V4 base. V4 remains preserved. V5 currently has reviewable schematic candidates for the two-channel analog EMG chain, a power/reference block, MCP3208 ADC integration, and DSTK22807 SPI-only integration.

## Project Summary

V5 is moving toward a two-channel EMG prototype with:

- Two analog channels, each producing `RAW`, `RECT`, and `ENV` outputs.
- MCP3208 ADC sampling six EMG outputs plus monitor channels.
- ESP32-H2 DSTK22807 development board used as the first wireless/controller module.
- A measured removable THT carrier/socket footprint for the DSTK22807.
- A battery-isolated first-validation power direction based on 3xAA alkaline plus a low-noise 3.3 V LDO candidate.

The current design is still in controlled schematic and planning review. It is not ready for PCB layout, fabrication, final ordering, or any human-contact test procedure approval.

## Implemented So Far

### Schematic Structure

- Root schematic uses hierarchical sheets for:
  - `EMG_CHANNEL_1_ANALOG`
  - `EMG_CHANNEL_2_ANALOG`
  - `POWER_REFERENCE_BLOCK`
- The old flat/legacy root EMG block was removed.
- `POWER_REFERENCE_BLOCK` receives these analog outputs as hierarchical inputs:
  - `EMG1_RAW`
  - `EMG1_RECT`
  - `EMG1_ENV`
  - `EMG2_RAW`
  - `EMG2_RECT`
  - `EMG2_ENV`
- `POWER_REFERENCE_BLOCK` exposes MCP3208 SPI nets to root:
  - `ADC_CS`
  - `ADC_SCLK`
  - `ADC_MOSI`
  - `ADC_MISO`

### Analog Channels

- Channel 1 analog schematic candidate is implemented and reviewed enough for controlled duplication, but not final hardware approval.
- Channel 2 analog schematic candidate is implemented as a controlled Channel 1 adaptation.
- TRS mapping is corrected for both channels:
  - Tip/T -> `EMGx_IN_P`
  - Ring/R -> `EMGx_IN_N`
  - Sleeve/S -> `EMGx_REF_ELECTRODE`
- Physical TRS cable measurement note:
  - yellow tip -> Tip/T -> `EMGx_IN_P`
  - green middle body -> Ring/R -> `EMGx_IN_N`
  - red rear body -> Sleeve/S -> `EMGx_REF_ELECTRODE`
- RAW gain input polarity is corrected for both channels:
  - Channel 1: `RAW_HPF_NODE` to U302A non-inverting input, `RAW_GAIN_FB` to U302A inverting input.
  - Channel 2: `EMG2_RAW_HPF_NODE` to U402A non-inverting input, `EMG2_RAW_GAIN_FB` to U402A inverting input.
- `EMG1_REF_ELECTRODE` and `EMG2_REF_ELECTRODE` remain isolated from GND, chassis, and USB.

### Rectifier / Envelope Candidate

- Channel 1 RECT/ENV candidate has supporting LTspice planning and simulation files under `V5/sim/rectifier/`.
- Simulation status is reviewable, not final approval:
  - Behavioral target validates `RECT_OUT = VREF + abs(RAW_IN - VREF)`.
  - Real-topology and conservative RRIO candidates are reviewable.
  - Exact MCP600x and BAS70 vendor models are not locked in the repo.
- Channel 2 inherits the Channel 1 RECT/ENV topology and footprint strategy.
- First-prototype direction for rectifier diodes:
  - D331/D332/D431/D432 value: `BAS70ZFILM`
  - Symbol: `Device:D_Schottky`
  - Footprint: `Diode_SMD:D_SOD-123`
- D331/D332/D431/D432 cathode-band orientation must still be visually verified before PCB.

### MCP3208 ADC

MCP3208 package direction:

- MPN direction: `MCP3208-CI/P`
- KiCad symbol: `Analog_ADC:MCP3208`
- KiCad footprint: `Package_DIP:DIP-16_W7.62mm`

Locked ADC channel mapping:

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

### DSTK22807 SPI Integration

The DSTK22807 is represented by U501:

- Symbol: `EMG_V5:DSTK22807_ESP32H2_SuperMini_Measured`
- Footprint: `EMG_V5:DSTK22807_ESP32H2_SuperMini_Carrier_Measured_THT`
- U501 GND / pin 11 is connected to root/system GND for SPI logic reference.
- U501 3V3 is not connected to `3V3_ADC`.
- U501 5V/VBUS is not connected to any analog/ADC rail.

Current SPI mapping:

| SPI Net | U501 GPIO | U501 Pin |
| --- | --- | --- |
| `ADC_CS` | GPIO14 | Pin 13 |
| `ADC_SCLK` | GPIO13 | Pin 14 |
| `ADC_MOSI` | GPIO12 | Pin 15 |
| `ADC_MISO` | GPIO11 | Pin 16 |

MCP3208 SPI mapping:

| MCP3208 Pin / Function | V5 Net |
| --- | --- |
| CLK | `ADC_SCLK` |
| Din | `ADC_MOSI` |
| Dout | `ADC_MISO` |
| CS/SHDN | `ADC_CS` |

Intentional U501 no-connect pins are TX, RX, GPIO0, GPIO1, GPIO2, GPIO3, GPIO4, GPIO5, GPIO8, 5V/VBUS, 3V3, GPIO10, and GPIO9. No ERC suppression markers were added for the remaining planning/library warnings.

## Current Schematic Status

| Area | Status |
| --- | --- |
| Root hierarchical schematic | Implemented as V5 structure |
| Channel 1 analog chain | Candidate implemented / reviewable |
| Channel 2 analog chain | Candidate implemented / reviewable |
| Power/reference block | Added / reviewable |
| MCP3208 ADC channel mapping | Explicit and connected |
| DSTK SPI logic | Connected |
| DSTK power source | Not approved |
| 3xAA LDO path | Planning candidate only |
| PCB layout | Not started |
| Final hardware approval | Not granted |

## Power and Safety Status

### Current Power Direction

The current first-validation power direction is:

`3xAA alkaline battery pack -> switch/protection -> low-noise 3.3 V LDO -> 3V3_ADC`

This is a first analog validation candidate only. It is not a final product battery decision and not an approved schematic implementation.

Current LDO target review:

- Practical first-validation current capability target: at least 50 mA, pending datasheet review and measured analog/ADC load.
- 100 mA or higher regulator class may be acceptable only if noise, dropout, thermal behavior, quiescent current, reverse-current behavior, capacitor stability, and package suitability remain acceptable.
- Preliminary dropout target: <=300 mV at the selected current budget.
- Preferred dropout target: <=200 mV at the measured expected analog/ADC load.
- Preliminary Vin max target: >=5.5 V preferred minimum, >=6.0 V more comfortable for fresh 3xAA alkaline margin.
- Regulator MPN, package, footprint, BOM item, and schematic implementation are not selected or approved.

### Required Net Separation

- `3V3_ADC` is the selected analog/ADC rail name.
- `ADC_REF` is the MCP3208 full-scale reference and must remain separate from `analog VREF`.
- `analog VREF` is the midscale analog front-end bias/reference node, not the ADC full-scale reference.
- `REF_ELECTRODE` nets must not be tied to GND, chassis, or USB.
- Local regulator output must not be directly shorted to DSTK 3V3.
- Any future selectable source strategy must make source selection mutually exclusive.

### Human-Contact Safety Boundary

When electrodes are attached to a human:

- Use battery-isolated operation only.
- Disconnect USB.
- Do not use USB power, 5 V power, bench-supply power, mains-connected equipment, or oscilloscope earth.
- Do not connect `REF_ELECTRODE` nets to GND, chassis, or USB.

Battery-powered operation is required for human-contact testing, but it is not by itself final safety approval.

## ERC Status

Latest documented root ERC after DSTK SPI/U501 cleanup:

- 0 errors
- 2 known warnings

Known warnings:

- U501 footprint library warning caused by the CLI config not including the `EMG_V5` footprint library.
- `DSTK_3V3_CANDIDATE` isolated pin label.

The `DSTK_3V3_CANDIDATE` warning is intentionally left visible as a planning reminder. Do not suppress it with a No ERC marker unless a later explicit review approves that action.

Superseded historical ERC results are not active status. Earlier U201 SPI placeholder errors and isolated SPI label warnings were closed by the DSTK SPI-only integration.

## Blocked / Not Approved

The following remain blocked or not approved:

- PCB layout.
- Fabrication output.
- Final hardware approval.
- DSTK 3V3 as the source for `3V3_ADC`.
- DSTK 5V/VBUS as any analog/ADC rail.
- USB-powered human EMG testing.
- Human-contact testing with USB connected.
- Tying `EMG1_REF_ELECTRODE` or `EMG2_REF_ELECTRODE` to GND, chassis, or USB.
- Merging `ADC_REF` and `analog VREF`.
- Selecting a regulator MPN/package/footprint/BOM item.
- Implementing the 3xAA LDO path in schematic.
- Starting a full board schematic expansion beyond the current controlled candidates.

Open engineering gates:

- Confirm DSTK power-path behavior before any DSTK rail is used as a supply source.
- Estimate and/or measure analog/ADC current budget.
- Review candidate LDO datasheets against dropout, noise, PSRR, stability, reverse-current, package, and availability requirements.
- Validate `BAT_MON` divider against fresh 3xAA voltage and ADC source impedance.
- Confirm `ADC_REF` ripple/noise and relationship to MCP3208 VDD.
- Confirm `analog VREF` buffer stability and loading.
- Verify D331/D332/D431/D432 physical cathode-band orientation before PCB.
- Lock exact passive ordering MPNs for ratio-critical rectifier resistors and analog capacitors.

## Next Safe Steps

Recommended next work should stay in review/planning unless explicitly approved otherwise:

1. Create a narrow LDO candidate review for the 3xAA alkaline `3V3_ADC` path.
2. Estimate first-pass analog/ADC current budget from active parts and divider loads.
3. Review `BAT_MON` scaling for fresh 3xAA alkaline maximum voltage.
4. Review `ADC_REF` filtering/decoupling and MCP3208 VDD/VREF relationship.
5. Visually verify BAS70ZFILM SOD-123 cathode-band orientation before any PCB step.

Do not edit KiCad schematic, PCB, symbols, footprints, firmware, or simulation files without an explicit approved plan.

## Historical Notes

Recent relevant checkpoints:

- `bc5ff05 sync docs with 3xaa ldo target review`
- `2f772d8 sync docs with 3xaa ldo requirements`
- `854e438 sync docs with first 3xaa battery decision`
- `eba22c8 sync docs with local 3v3 adc regulator plan`
- `54d9089 sync docs with dstk power strategy`
- `79730dd connect dstk spi and mark unused pins`
- `1e5d3b0 fix channel trs mapping and raw gain input polarity`
- `60d5881 implement channel 2 analog schematic`
- `080eef1 remove legacy root emg schematic block`

Older branch names and older ERC counts in prior notes are historical only. The current repository branch is `v5/KiCad`, and the current status baseline for this document is HEAD `bc5ff05`.
