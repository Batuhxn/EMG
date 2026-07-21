# DSTK22807 ESP32-H2 Super Mini Symbol Pin Mapping

Symbol: `EMG_V5:DSTK22807_ESP32H2_SuperMini_Measured`

Assigned footprint: `EMG_V5:DSTK22807_ESP32H2_SuperMini_Carrier_Measured_THT`

This table reflects the mapping used by committed schematic checkpoint `bbe257e2c9dc28f7537942b5f15370770654ba19`.

## Accepted mapping

| Pad / symbol pin | Pin name | Electrical type | Current project use or constraint |
| --- | --- | --- | --- |
| 1 | TX | Output | Reserved unless intentionally assigned |
| 2 | RX | Input | Reserved unless intentionally assigned |
| 3 | GPIO0 | Bidirectional | Unassigned |
| 4 | GPIO1 | Bidirectional | Unassigned |
| 5 | GPIO2 | Bidirectional | Unassigned; strapping caution |
| 6 | GPIO3 | Bidirectional | Unassigned; strapping caution |
| 7 | GPIO4 | Bidirectional | `ADC_SCLK` through `MCU_ADC_SCLK` and U205 |
| 8 | GPIO5 | Bidirectional | Unassigned |
| 9 | GPIO8 | Bidirectional | Unassigned; strapping caution |
| 10 | `5V_VBUS` | Power input | Fed from `CARRIER_VBUS_SW` in the battery architecture |
| 11 | GND | Power input | Common project ground |
| 12 | `3V3` | Power input | Monitored as `CARRIER_3V3`; measured approximately 3.291–3.31 V while powered |
| 13 | GPIO14 | Bidirectional | `ADC_CS` through `MCU_ADC_CS` and U205 |
| 14 | GPIO13 | Bidirectional | Unassigned; not the current SCLK pin |
| 15 | GPIO12 | Bidirectional | `ADC_MOSI` through `MCU_ADC_MOSI` and U205 |
| 16 | GPIO11 | Bidirectional | `ADC_MISO` through `MCU_ADC_MISO` and U205 |
| 17 | GPIO10 | Bidirectional | Unassigned spare |
| 18 | GPIO9 | Bidirectional | Unassigned; strapping caution |

## Mapping status and limits

- Pads 10, 11, and 12 are accepted as `5V_VBUS`, GND, and carrier `3V3` respectively.
- The implemented SPI assignment is GPIO14 CS, GPIO4 SCLK, GPIO12 MOSI, and GPIO11 MISO.
- The symbol conservatively declares the exposed power pins as power inputs; it does not declare the carrier as a project power source.
- The mapping evidence does not establish safe external 3V3 injection, current capability, or USB backfeed behavior.
- No `No ERC` markers are part of the project-local carrier symbol.

Mechanical orientation, USB accessibility, antenna keepout, and physical fit remain PCB-entry gates even though the electrical mapping used by the schematic is resolved.
