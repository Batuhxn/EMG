# DSTK22807 SPI Pin Review for MCP3208

This document reviews provisional ESP32-H2 GPIO candidates for connecting an MCP3208 ADC over SPI through the DSTK22807 ESP32-H2 Super Mini carrier footprint.

This is a review document only. No schematic connection, footprint pad rename, or firmware assignment is approved by this document.

## 1. Current repo state

- Branch: `v5/measured-dstk22807-footprint`
- Git status before edit: clean
- Last 5 commits at start of review:
  - `f6e2b14 document observed DSTK22807 pinout mapping`
  - `3824d20 clean up DSTK22807 carrier footprint silkscreen`
  - `b4ba0e9 add measured provisional DSTK22807 carrier footprint`
  - `b7384a1 emergency baseline from transferred EMG project`

## 2. Existing observed pinout

Source document: `V5/DSTK22807_PHYSICAL_PINOUT_OBSERVATION.md`

| Footprint Pad | Observed Label | Current Interpretation | Confidence |
| --- | --- | --- | --- |
| 1 | TX | UART/debug TX label | Observed |
| 2 | RX | UART/debug RX label | Observed |
| 3 | 0 | GPIO0 | Observed |
| 4 | 1 | GPIO1 | Observed |
| 5 | 2 | GPIO2 | Observed |
| 6 | 3 | GPIO3 | Observed |
| 7 | 4 | GPIO4 | Observed |
| 8 | 5 | GPIO5 | Observed |
| 9 | 8 | GPIO8 | Observed |
| 10 | 5V | 5V rail | Provisional from right-row reversal |
| 11 | GND | Ground | Provisional from right-row reversal |
| 12 | 3V3 | 3.3V rail | Provisional from right-row reversal |
| 13 | 14 | GPIO14 | Provisional from right-row reversal |
| 14 | 13 | GPIO13 | Provisional from right-row reversal |
| 15 | 12 | GPIO12 | Provisional from right-row reversal |
| 16 | 11 | GPIO11 | Provisional from right-row reversal |
| 17 | 10 | GPIO10 | Provisional from right-row reversal |
| 18 | 9 | GPIO9 | Provisional from right-row reversal |

## 3. ESP32-H2 GPIO capability review

Official sources checked:

- Espressif ESP32-H2 Series Datasheet v1.2: https://www.espressif.com/sites/default/files/documentation/esp32-h2_datasheet_en.pdf
- ESP-IDF GPIO & RTC GPIO guide for ESP32-H2: https://docs.espressif.com/projects/esp-idf/en/latest/esp32h2/api-reference/peripherals/gpio.html
- ESP-IDF SPI Master Driver guide for ESP32-H2: https://docs.espressif.com/projects/esp-idf/en/latest/esp32h2/api-reference/peripherals/spi_master.html

Findings from official Espressif documentation:

- ESP32-H2 has 19 programmable GPIOs in the datasheet feature list, and ESP-IDF states user-available pins for SiP flash variants are GPIO0-GPIO5, GPIO8-GPIO14, and GPIO22-GPIO27.
- ESP-IDF documents GPIO0-GPIO27 as physical GPIO-capable pins, with peripheral input and output routable through GPIO matrix / IO MUX.
- ESP32-H2 strapping pins include GPIO2, GPIO3, GPIO8, GPIO9, and GPIO25 per ESP-IDF GPIO documentation.
- ESP32-H2 datasheet feature list highlights GPIO8, GPIO9, and GPIO25 as strapping pins; ESP-IDF additionally flags GPIO2 and GPIO3 as strapping pins.
- GPIO15-GPIO21 are usually used for SPI flash and are not recommended for user applications; on SiP flash variants they are not fan-out to external pins.
- GPIO26/GPIO27 are USB Serial/JTAG by default and should not be used unless that function is intentionally disabled.
- SPI2 IO_MUX pins are CS0=GPIO1, SCLK=GPIO4, MISO=GPIO0, MOSI=GPIO5, QUADWP=GPIO2, QUADHD=GPIO3.
- ESP-IDF SPI master driver allows SPI signals to be routed through GPIO matrix; for SPI host speeds at 80 MHz or lower, GPIO matrix routing behaves the same as IO_MUX routing according to the ESP32-H2 SPI guide.

Implications for the observed DSTK22807 pins:

- GPIO14, GPIO13, GPIO12, GPIO11, and GPIO10 are attractive right-row candidates because they avoid TX/RX labels and are not documented as strapping pins in the checked official sources.
- GPIO9 and GPIO8 should be avoided for now because they are strapping pins.
- GPIO2 and GPIO3 should also be treated cautiously because ESP-IDF flags them as strapping pins, even though they appear on the left row.
- GPIO0, GPIO1, GPIO4, and GPIO5 are SPI2 IO_MUX-capable candidates, but they are on the left row and overlap with the measured board labels near TX/RX; using them is not necessary for low-speed MCP3208 SPI.
- TX/RX should remain free for programming/debug until firmware and bring-up flow are intentionally defined.

Unverified or board-specific items:

- The DSTK22807 board schematic/pinout was not verified from an official vendor datasheet.
- The observed right-row reversal must still be checked physically.
- Whether the board routes TX/RX only to UART0 pins or through USB bridge circuitry is not confirmed.
- USB connector side and antenna side remain physical-orientation blockers.

## 4. MCP3208 SPI needs

Official Microchip source checked:

- Microchip MCP3204/3208 datasheet: https://ww1.microchip.com/downloads/aemDocuments/documents/APID/ProductDocuments/DataSheets/21298e.pdf

Relevant MCP3208 digital interface facts:

- MCP3208 uses SPI-compatible serial interface.
- Digital SPI pins are `CS/SHDN`, `CLK`, `DIN`, and `DOUT`.
- Single-supply operation is 2.7V to 5.5V.
- Digital input high threshold is specified as `0.7 * VDD`; digital input low threshold is `0.3 * VDD`.
- If MCP3208 is powered from 3.3V, ESP32-H2 3.3V logic is compatible in principle.
- If MCP3208 is powered from 5V, MCP3208 `DOUT` can present 5V-level output to the ESP32-H2 and is not safe without level shifting or powering MCP3208 at 3.3V.
- ADC reference and analog front-end reference selection are separate analog design topics and are not approved by this SPI pin review.

MCP3208 net needs for this project:

| MCP3208 Net | Direction from ESP32-H2 point of view | Notes |
| --- | --- | --- |
| `ADC_CS` | Output | Connects to MCP3208 `CS/SHDN` |
| `ADC_SCLK` | Output | Connects to MCP3208 `CLK` |
| `ADC_MOSI` | Output | Connects to MCP3208 `DIN` |
| `ADC_MISO` | Input | Connects to MCP3208 `DOUT` |

## 5. Candidate SPI assignment

Recommended candidate set avoids TX/RX, power pins, GPIO8/GPIO9 strapping pins, and left-row strapping candidates.

| MCP3208 Net | ESP32-H2 GPIO Candidate | Footprint Pad | Reason | Risk | Confidence |
| --- | --- | --- | --- | --- | --- |
| `ADC_CS` | GPIO14 | Pad 13 | Right-row GPIO; not identified as strapping in checked official docs; convenient chip-select output | Pad mapping still provisional from right-row reversal | Medium |
| `ADC_SCLK` | GPIO13 | Pad 14 | Right-row GPIO; not identified as strapping in checked official docs; suitable GPIO-matrix SPI clock candidate | Pad mapping still provisional; GPIO13 can be RTC/32k-related on bare SoC context, confirm board does not use it | Medium |
| `ADC_MOSI` | GPIO12 | Pad 15 | Right-row GPIO; not identified as strapping in checked official docs; output to MCP3208 DIN | Pad mapping still provisional | Medium |
| `ADC_MISO` | GPIO11 | Pad 16 | Right-row GPIO; not identified as strapping in checked official docs; input from MCP3208 DOUT | Pad mapping still provisional; confirm MCP3208 powered at 3.3V or level-shifted | Medium |

Backup candidate:

| Use | Candidate | Footprint Pad | Notes |
| --- | --- | --- | --- |
| Spare right-row GPIO | GPIO10 | Pad 17 | Keep as spare interrupt/debug/alternate CS candidate; avoid using until GPIO14-11 are confirmed |

## 6. Pins to avoid for now

| Pin / Label | Footprint Pad | Reason |
| --- | --- | --- |
| TX | Pad 1 | Keep free for programming/debug until bring-up strategy is defined |
| RX | Pad 2 | Keep free for programming/debug until bring-up strategy is defined |
| GPIO2 | Pad 5 | ESP-IDF flags as strapping pin |
| GPIO3 | Pad 6 | ESP-IDF flags as strapping pin |
| GPIO8 | Pad 9 | Officially documented strapping pin |
| 5V | Pad 10 | Power rail, not GPIO; also avoid 5V/human-test hazards |
| GND | Pad 11 | Ground, not GPIO |
| 3V3 | Pad 12 | Power rail, not GPIO |
| GPIO9 | Pad 18 | Officially documented strapping pin |

Also avoid assuming GPIO26/GPIO27 or USB-related pins are available unless the actual DSTK22807 breakout exposes them and USB Serial/JTAG usage is intentionally handled.

## 7. Blockers before schematic

- Physical pinout must be confirmed a second time by clear photo and/or continuity measurement.
- Right-row reversal must be confirmed: top-right footprint pad 10 must be `5V`, and bottom-right footprint pad 18 must be `GPIO9`.
- USB side and antenna side must be physically confirmed.
- DSTK22807 vendor board pinout/schematic should be located or independently verified.
- ESP32-H2 official pin capability has been checked at SoC/ESP-IDF level, but the specific development board routing is still unverified.
- MCP3208 exact MPN, package, symbol-footprint mapping, VDD, VREF, and level compatibility must be verified separately before schematic integration.
- MCP3208 should be powered at 3.3V for direct ESP32-H2 SPI compatibility; if powered at 5V, level shifting is required for the MCP3208 `DOUT` path into ESP32-H2.
- Human-test safety: electrodes must not be connected to a human subject while the system is powered from USB, 5V, mains-connected equipment, oscilloscope earth, or any non-isolated supply path.

## 8. Recommendation

Recommended SPI candidate set:

- `ADC_CS = GPIO14 / Pad 13`
- `ADC_SCLK = GPIO13 / Pad 14`
- `ADC_MOSI = GPIO12 / Pad 15`
- `ADC_MISO = GPIO11 / Pad 16`

Confidence: Medium for SoC GPIO/SPI feasibility; Low-to-Medium for the actual DSTK22807 board until physical pinout and right-row reversal are confirmed.

Remaining blockers:

- Confirm physical pin mapping again.
- Confirm right-row top/bottom orientation.
- Confirm board USB and antenna orientation.
- Confirm MCP3208 supply at 3.3V or add level shifting if 5V is used.
- Confirm selected GPIOs are not consumed by board hardware, boot configuration, or future firmware needs.

Should schematic implementation proceed? `NO` unless all blockers above are closed.

## Decision

`SPI_PIN_REVIEW_READY_FOR_REVIEW`
