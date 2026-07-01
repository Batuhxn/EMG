# DSTK22807 Photo-Derived Pin Notes

Source: user-provided front/back photos, 2026-06-30.

These notes are useful for schematic planning, but they are not enough to create the final carrier footprint. Confirm all labels on the physical board before PCB layout.

Product listing dimensions provided by user:

- Board size: `18 mm x 23 mm`
- Connector format: two side headers, `2x9`
- Supply/logic: `3.3 V`

## Visible Pin Labels

Front photo, USB connector at top:

Left side, top to bottom:

- `TX`
- `RX`
- `0`
- `1`
- `2`
- `3`
- `4`
- `5`

Right side, top to bottom:

- `BAT`
- `5V`
- `GND`
- `3V3`
- `14`
- `13`
- `12`
- `11`
- `10`
- `9`

Bottom pads visible:

- `26`
- `25`
- `22`
- `27`

Back photo confirms the board is marked `Super Mini ESP32-H2`, with castellated side pads and bottom pads `22`, `25`, `26`, `27`.

## Recommended MCP3208 SPI Mapping For This Board

Use these only after confirming the same labels on the physical board:

| MCP3208 Signal | DSTK22807 Pin Label |
| --- | --- |
| `MCP3208_CS` | `4` |
| `MCP3208_CLK` | `5` |
| `MCP3208_MOSI` | `10` |
| `MCP3208_MISO` | `11` |

Rationale:

- Avoids visible `2`, `3`, `9`, and `25`.
- Reserves bottom pads `26` and `27`.
- Keeps SPI on clearly labelled exposed pads.

## Still Required Before Footprint

- Confirm board length and width against listing value `18 mm x 23 mm`.
- Confirm header/castellated pitch, assumed `2.54 mm`.
- Measure USB-C connector overhang.
- Locate antenna keepout from physical board. The red/white ceramic-looking antenna area is at the bottom/front side in the photo.
- Confirm whether the side pads are castellated only, through-hole header only, or both.
