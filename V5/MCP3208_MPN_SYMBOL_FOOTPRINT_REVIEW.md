# MCP3208 MPN, Symbol, Footprint, and Pin 1 Review

This document reviews the proposed MCP3208 ADC MPNs, official Microchip package/pinout evidence, KiCad symbol mapping, KiCad footprint candidates, and Pin 1 orientation before schematic integration.

This is a review document only. No schematic, PCB, footprint, firmware, gerber, or production file changes are approved by this document.

## 1. Current repo state

- Branch: `v5/measured-dstk22807-footprint`
- Git status before edit: clean
- Last 5 commits at start of review:
  - `8a252b4 document DSTK22807 SPI pin candidates`
  - `f6e2b14 document observed DSTK22807 pinout mapping`
  - `3824d20 clean up DSTK22807 carrier footprint silkscreen`
  - `b4ba0e9 add measured provisional DSTK22807 carrier footprint`
  - `b7384a1 emergency baseline from transferred EMG project`

## 2. MCP3208 candidate MPNs

Official Microchip source checked:

- Microchip MCP3204/3208 datasheet, DS21298E: https://ww1.microchip.com/downloads/aemDocuments/documents/APID/ProductDocuments/DataSheets/21298e.pdf
- Microchip MCP3208 product page: https://www.microchip.com/en-us/product/MCP3208

Datasheet ordering information states:

- `MCP3208` is the 8-channel 12-bit serial A/D converter family.
- Grade `C` means +/-2 LSB INL.
- Grade `B` means +/-1 LSB INL.
- `MCP3208-CI/P` is the C-grade accuracy version. For first EMG prototype bring-up this is acceptable, but if accuracy margin becomes critical, `MCP3208-BI/P` or `MCP3208-BI/SL` can be considered.
- Temperature range `I` means -40 C to +85 C industrial temperature.
- Package `P` means Plastic DIP, 300 mil body.
- Package `SL` means Plastic SOIC, 150 mil body.

| Candidate MPN | Package | Pin count | Mounting | Role | Confidence | Notes |
| --- | --- | --- | --- | --- | --- | --- |
| `MCP3208-CI/P` | 300 mil PDIP | 16 | Through-hole | Recommended first prototype ADC | High | Official ordering code format supports `C`, `I`, and `/P`; best for socket/hand assembly |
| `MCP3208-CI/SL` | 150 mil SOIC | 16 | SMD | Compact alternative | High | Official ordering code format supports `C`, `I`, and `/SL`; useful after DIP prototype is verified |

## 3. Official datasheet package/pinout review

The MCP3208 package pinout table in the official datasheet lists MCP3208 PDIP/SOIC pin numbers as follows.

| Pin Number | Datasheet Pin Name | Function |
| --- | --- | --- |
| 1 | CH0 | Analog Input |
| 2 | CH1 | Analog Input |
| 3 | CH2 | Analog Input |
| 4 | CH3 | Analog Input |
| 5 | CH4 | Analog Input |
| 6 | CH5 | Analog Input |
| 7 | CH6 | Analog Input |
| 8 | CH7 | Analog Input |
| 9 | DGND | Digital Ground |
| 10 | CS/SHDN | Chip Select/Shutdown Input |
| 11 | DIN | Serial Data In |
| 12 | DOUT | Serial Data Out |
| 13 | CLK | Serial Clock |
| 14 | AGND | Analog Ground |
| 15 | VREF | Reference Voltage Input |
| 16 | VDD | +2.7V to +5.5V Power Supply |

Package evidence:

- Datasheet description states MCP3208 is offered in 16-pin PDIP and SOIC packages.
- Datasheet package marking section includes examples for 16-lead PDIP and 16-lead SOIC MCP3208.
- Datasheet family text and ordering table contain TSSOP references, but the explicit MCP3208 package description and footprint-filter match support PDIP/SOIC for this review. TSSOP is not selected for this project.

## 4. KiCad symbol review

KiCad source checked locally:

- Symbol: `Analog_ADC:MCP3208`
- Local file: `C:\Program Files\KiCad\10.0\share\kicad\symbols\Analog_ADC.kicad_sym`

KiCad symbol metadata:

- Value: `MCP3208`
- Datasheet property: Microchip MCP3204/3208 datasheet URL
- Description: `A/D Converter, 12-Bit, 8-Channel, SPI Interface , 2.7V-5.5V`
- Footprint filters: `DIP*W7.62mm* SOIC*3.9x9.9mm*P1.27mm*`

| Symbol Pin Number | Symbol Pin Name | Datasheet Match? | Notes |
| --- | --- | --- | --- |
| 1 | CH0 | Yes | Matches MCP3208 pin 1 |
| 2 | CH1 | Yes | Matches MCP3208 pin 2 |
| 3 | CH2 | Yes | Matches MCP3208 pin 3 |
| 4 | CH3 | Yes | Matches MCP3208 pin 4 |
| 5 | CH4 | Yes | Matches MCP3208 pin 5 |
| 6 | CH5 | Yes | Matches MCP3208 pin 6 |
| 7 | CH6 | Yes | Matches MCP3208 pin 7 |
| 8 | CH7 | Yes | Matches MCP3208 pin 8 |
| 9 | DGND | Yes | Matches MCP3208 pin 9 |
| 10 | `~{CS}/SHDN` | Yes | Same function as datasheet `CS/SHDN`; KiCad marks active-low CS visually |
| 11 | Din | Yes | Same as datasheet `DIN` |
| 12 | Dout | Yes | Same as datasheet `DOUT` |
| 13 | CLK | Yes | Matches MCP3208 pin 13 |
| 14 | AGND | Yes | Matches MCP3208 pin 14 |
| 15 | Vref | Yes | Same as datasheet `VREF` |
| 16 | Vdd | Yes | Same as datasheet `VDD` |

Result: KiCad symbol `Analog_ADC:MCP3208` matches the official MCP3208 PDIP/SOIC pinout.

## 5. DIP footprint review

Candidate footprint checked locally:

- `Package_DIP:DIP-16_W7.62mm`
- Local file: `C:\Program Files\KiCad\10.0\share\kicad\footprints\Package_DIP.pretty\DIP-16_W7.62mm.kicad_mod`

Findings:

- Pad count: 16 pads.
- Mounting: through-hole.
- Row spacing: 7.62 mm / 300 mil, matching `MCP3208-CI/P` PDIP package family.
- Pad numbering: 1-8 down the left side, 9-16 up the right side, standard DIP numbering.
- Pin 1 marker: present by roundrect pad 1, F.Fab chamfer/notch cue, and F.SilkS notch arc.
- Through-hole hand assembly: suitable.
- Socket usage: suitable for a DIP-16 socket, subject to final mechanical clearance on PCB.
- Symbol-to-footprint mapping: symbol pin numbers 1-16 map directly to footprint pads 1-16.

| Footprint Pad | Expected Symbol/Datasheet Pin | Match? |
| --- | --- | --- |
| 1 | CH0 | Yes |
| 2 | CH1 | Yes |
| 3 | CH2 | Yes |
| 4 | CH3 | Yes |
| 5 | CH4 | Yes |
| 6 | CH5 | Yes |
| 7 | CH6 | Yes |
| 8 | CH7 | Yes |
| 9 | DGND | Yes |
| 10 | CS/SHDN | Yes |
| 11 | DIN | Yes |
| 12 | DOUT | Yes |
| 13 | CLK | Yes |
| 14 | AGND | Yes |
| 15 | VREF | Yes |
| 16 | VDD | Yes |

Result: `Package_DIP:DIP-16_W7.62mm` is verified as the recommended first prototype footprint for `MCP3208-CI/P`.

## 6. SOIC alternative review

Candidate footprint checked locally:

- `Package_SO:SOIC-16_3.9x9.9mm_P1.27mm`
- Local file: `C:\Program Files\KiCad\10.0\share\kicad\footprints\Package_SO.pretty\SOIC-16_3.9x9.9mm_P1.27mm.kicad_mod`

Findings:

- Pad count: 16 pads.
- Mounting: SMD.
- Pitch: 1.27 mm.
- Package body: 3.9 mm x 9.9 mm class, matching narrow 150 mil SOIC candidate family.
- Pin 1 marker: present via F.SilkS triangle marker and F.Fab chamfer/pin-1 body cue.
- Symbol-to-footprint mapping: symbol pin numbers 1-16 map directly to footprint pads 1-16.
- Manual soldering risk: moderate; manageable with flux and fine tip, but less forgiving than DIP.
- First prototype comparison: DIP is easier for socketing/rework and safer for urgent bring-up; SOIC is smaller and better for later compact PCB revisions.

Result: `Package_SO:SOIC-16_3.9x9.9mm_P1.27mm` is a valid compact alternative for `MCP3208-CI/SL`, but not the first prototype recommendation.

## 7. TSSOP rejection

- MCP3208 TSSOP will not be used for this project revision.
- The first prototype direction is DIP-16 using `MCP3208-CI/P` and `Package_DIP:DIP-16_W7.62mm`.
- Although the Microchip datasheet family text contains TSSOP-related references, the MCP3208 package evidence used for this review supports 16-pin PDIP and 16-pin SOIC as the practical candidates.
- TSSOP may be reconsidered only if an exact official MCP3208 TSSOP MPN and matching datasheet package/pinout evidence are verified later.

## 8. V5 channel mapping compatibility

| MCP3208 Channel | V5 Net |
| --- | --- |
| CH0 | EMG1_RAW |
| CH1 | EMG1_RECT |
| CH2 | EMG1_ENV |
| CH3 | EMG2_RAW |
| CH4 | EMG2_RECT |
| CH5 | EMG2_ENV |
| CH6 | VREF_MON |
| CH7 | BAT_MON |

Datasheet compatibility: MCP3208 provides CH0-CH7 on pins 1-8, so the proposed eight-channel V5 mapping is feasible at the pin-count level.

## 9. Digital SPI mapping compatibility

Reference document:

- `V5/DSTK22807_SPI_PIN_REVIEW.md`

DSTK22807 candidate SPI nets:

- `ADC_CS = GPIO14 / Pad 13`
- `ADC_SCLK = GPIO13 / Pad 14`
- `ADC_MOSI = GPIO12 / Pad 15`
- `ADC_MISO = GPIO11 / Pad 16`

MCP3208 pin counterparts:

| Net | MCP3208 Pin | MCP3208 Pin Name | Direction |
| --- | --- | --- | --- |
| `ADC_CS` | 10 | CS/SHDN | ESP32-H2 output to ADC input |
| `ADC_SCLK` | 13 | CLK | ESP32-H2 output to ADC input |
| `ADC_MOSI` | 11 | DIN | ESP32-H2 output to ADC input |
| `ADC_MISO` | 12 | DOUT | ADC output to ESP32-H2 input |

Result: digital SPI net naming is compatible with the MCP3208 datasheet pin functions.

## 10. Power/reference blockers

- MCP3208 `VDD` is not final; for direct ESP32-H2 compatibility, 3.3V is preferred.
- MCP3208 `VREF` is the ADC reference pin and must not be confused with the analog front-end midscale `VREF` bias net.
- Use distinct net names such as `ADC_REF` and analog `VREF`; do not merge them without an explicit analog architecture decision.
- AGND/DGND grounding strategy must be reviewed separately before layout.
- MCP3208 input voltages must remain within datasheet limits relative to VSS/AGND and VREF/VDD.
- Layout/schematic must include proper `VREF` decoupling, `VDD` decoupling, AGND/DGND handling, and analog input range protection so CH0-CH7 never exceed VSS to VREF in single-ended mode.
- If MCP3208 is powered at 5V, `DOUT` level shifting is required before ESP32-H2.
- ADC input source impedance, anti-alias filtering, and sample/hold settling must be checked before final component values are approved.
- Human-test safety remains active: do not power the system from USB, 5V, mains-connected tools, or oscilloscope earth while electrodes are connected to a human subject.

## 11. Recommendation

Recommended first prototype ADC:

- MPN: `MCP3208-CI/P`
- Package: 16-pin PDIP, 300 mil body
- KiCad symbol: `Analog_ADC:MCP3208`
- KiCad footprint: `Package_DIP:DIP-16_W7.62mm`
- Status: Recommended for first prototype / hand assembly / socketed bring-up

Compact alternative:

- MPN: `MCP3208-CI/SL`
- Package: 16-pin SOIC, 150 mil body
- KiCad footprint: `Package_SO:SOIC-16_3.9x9.9mm_P1.27mm`
- Status: Valid compact alternative after DIP prototype is verified

Should schematic implementation proceed?

- `NO`, unless all blockers are closed.

## 12. Blockers

| Check | Status |
| --- | --- |
| Official datasheet pinout verified? | Yes |
| KiCad symbol verified? | Yes |
| DIP footprint verified? | Yes |
| Pin 1 orientation verified? | Yes for KiCad symbol/footprint and datasheet pin numbering; still check physical IC/socket orientation during assembly |
| MCP3208 supply/reference architecture still open? | Yes; include VREF/VDD decoupling and keep ADC_REF distinct from analog midscale VREF |
| Analog input range still open? | Yes; protect CH0-CH7 so single-ended inputs remain within VSS to VREF |
| Physical DSTK22807 pinout still needs second confirmation? | Yes |
| AGND/DGND grounding strategy still open? | Yes |
| Human-test safety still applies? | Yes |

## Decision

`MCP3208_REVIEW_READY_FOR_REVIEW`



