# DSTK22807 ESP32-H2 Super Mini Measured Symbol Pin Mapping

Symbol: `EMG_V5:DSTK22807_ESP32H2_SuperMini_Measured`

Assigned footprint: `EMG_V5:DSTK22807_ESP32H2_SuperMini_Carrier_Measured_THT`

Source basis:

- Measured project footprint: `V5/EMG_V5.pretty/DSTK22807_ESP32H2_SuperMini_Carrier_Measured_THT.kicad_mod`
- Physical pinout observation: `V5/DSTK22807_PHYSICAL_PINOUT_OBSERVATION.md`
- Power measurement note: `V5/DSTK22807_POWER_PIN_MEASUREMENT.md`
- SPI/GPIO capability review: `V5/DSTK22807_SPI_PIN_REVIEW.md`

## Assumptions

- Footprint pad 1 is the top-left pad and aligns with the observed `TX` board label.
- Left header column maps pads 1 through 9 from top to bottom.
- Right header column maps pads 10 through 18 from top to bottom, based on the existing right-row reversal note.
- `GND` is set as `Power Input`.
- `5V_VBUS` and `3V3` are set as `Power Input` pins. This is conservative for ERC because the symbol does not declare the DSTK22807 board as a power source. USB measurement showed 5V and 3V3 are present while USB is connected, but external 3V3 powering and using 5V as an analog/ADC supply are not approved by the project notes.
- GPIO pins are set as `Bidirectional`.
- `TX` is set as `Output` and `RX` as `Input` because those labels indicate fixed UART/debug roles on the board header.
- No No ERC markers are part of this symbol.

## Pin Mapping

| Footprint pad number | Schematic pin number | Pin name | Electrical type | Notes |
| --- | --- | --- | --- | --- |
| 1 | 1 | TX | Output | Observed top-left pad label; keep free for programming/debug unless intentionally assigned. |
| 2 | 2 | RX | Input | Observed left-row label; keep free for programming/debug unless intentionally assigned. |
| 3 | 3 | GPIO0 | Bidirectional | Observed board label `0`. |
| 4 | 4 | GPIO1 | Bidirectional | Observed board label `1`; SPI2 IO_MUX CS0-capable at SoC level per project review. |
| 5 | 5 | GPIO2 | Bidirectional | Observed board label `2`; treated cautiously because ESP-IDF flags GPIO2 as a strapping pin. |
| 6 | 6 | GPIO3 | Bidirectional | Observed board label `3`; treated cautiously because ESP-IDF flags GPIO3 as a strapping pin. |
| 7 | 7 | GPIO4 | Bidirectional | Observed board label `4`; SPI2 IO_MUX SCLK-capable at SoC level per project review. |
| 8 | 8 | GPIO5 | Bidirectional | Observed board label `5`; SPI2 IO_MUX MOSI-capable at SoC level per project review. |
| 9 | 9 | GPIO8 | Bidirectional | Observed board label `8`; strapping pin, avoid unless intentionally handled. |
| 10 | 10 | 5V_VBUS | Power Input | Provisional right-row reversal mapping. USB-powered measurement observed about 5.126 V on this pin; not approved as analog/ADC supply. |
| 11 | 11 | GND | Power Input | Provisional right-row reversal mapping; measured ground reference pin. |
| 12 | 12 | 3V3 | Power Input | Provisional right-row reversal mapping. USB-powered measurement observed about 3.291 V; external powering/backfeed behavior remains unresolved. |
| 13 | 13 | GPIO14 | Bidirectional | Provisional right-row reversal mapping; preferred SPI candidate `ADC_CS` in project review. |
| 14 | 14 | GPIO13 | Bidirectional | Provisional right-row reversal mapping; preferred SPI candidate `ADC_SCLK` in project review. |
| 15 | 15 | GPIO12 | Bidirectional | Provisional right-row reversal mapping; preferred SPI candidate `ADC_MOSI` in project review. |
| 16 | 16 | GPIO11 | Bidirectional | Provisional right-row reversal mapping; preferred SPI candidate `ADC_MISO` in project review. |
| 17 | 17 | GPIO10 | Bidirectional | Provisional right-row reversal mapping; spare right-row GPIO candidate. |
| 18 | 18 | GPIO9 | Bidirectional | Provisional right-row reversal mapping; strapping pin, avoid unless intentionally handled. |
