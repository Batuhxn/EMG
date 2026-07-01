# DSTK22807 Measurement Checklist

Do this before PCB footprint placement.

## Physical

- Board length: listing says `23 mm`, confirm physically
- Board width: listing says `18 mm`, confirm physically
- PCB thickness: `TBD`
- Header/castellated pitch: assumed `2.54 mm`, confirm physically
- Number of pins per side: listing/photo indicates `9`
- USB connector type and side: `TBD`
- USB connector overhang beyond PCB edge: `TBD`
- Antenna location: `TBD`
- Antenna keepout rectangle: `TBD`

## Pin Labels

Record the exact printed labels from the physical board.

| Board Pin Label | ESP32-H2 GPIO | V5 Use |
| --- | --- | --- |
| `4` photo-derived, confirm physically | GPIO4 assumed | MCP3208_CS |
| `5` photo-derived, confirm physically | GPIO5 assumed | MCP3208_CLK |
| `10` photo-derived, confirm physically | GPIO10 assumed | MCP3208_MOSI |
| `11` photo-derived, confirm physically | GPIO11 assumed | MCP3208_MISO |
| TBD | TBD | 3V3 |
| TBD | TBD | GND |
| TBD | TBD | Optional EN/BOOT/TX/RX |

Photo-derived visible labels include `TX`, `RX`, `0`, `1`, `2`, `3`, `4`, `5`, `BAT`, `5V`, `GND`, `3V3`, `14`, `13`, `12`, `11`, `10`, `9`, and bottom pads `22`, `25`, `26`, `27`.

## Footprint Rules

- Keep DSTK22807 onboard USB accessible at the EMG PCB edge.
- Keep analog input traces away from the DSTK22807 antenna and digital pins.
- Do not put copper, vias, traces, or components under the antenna keepout.
- Do not add a separate USB-C connector on the EMG PCB for V5 first prototype.
- A first-pass carrier footprint exists under `EMG_V5.pretty`, based on `18 mm x 23 mm`, two `1x9` rows, `2.54 mm` pitch, and `15.24 mm` row spacing. Print/check against the real DSTK22807 board before fabrication.
