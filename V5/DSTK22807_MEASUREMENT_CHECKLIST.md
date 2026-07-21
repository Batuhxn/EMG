# DSTK22807 Remaining Measurement and Mechanical Checklist

The electrical header mapping used by the committed schematic is resolved. This checklist covers remaining mechanical and bench evidence; it must not be used to reopen U501 pads 10–12 as an implementation blocker.

## Accepted electrical mapping

| Board label / pad | Current project meaning |
| --- | --- |
| Pad 10 / 5V | `5V_VBUS` |
| Pad 11 / GND | GND |
| Pad 12 / 3V3 | Carrier `3V3` / `CARRIER_3V3` |
| Pad 13 / GPIO14 | `ADC_CS` through U205 |
| Pad 7 / GPIO4 | `ADC_SCLK` through U205 |
| Pad 15 / GPIO12 | `ADC_MOSI` through U205 |
| Pad 16 / GPIO11 | `ADC_MISO` through U205 |

## Mechanical checks before PCB implementation

- Confirm board length, width, and thickness against the real carrier.
- Confirm 2.54 mm header pitch, row spacing, and physical fit.
- Confirm USB connector side and overhang.
- Confirm antenna location and keepout rectangle.
- Confirm carrier orientation and assembly access.
- Print or otherwise scale-check the project-local footprint against the real board.

Keep the carrier USB accessible at the PCB edge, keep analog input routing away from the antenna and digital pins, and place no copper, vias, traces, or components in the antenna keepout.

## Power and interface bench checks

- Carrier startup and RF-burst rail behavior.
- OE rise delay and threshold behavior during rail ramps.
- OE low voltage with either supervisor asserted.
- Carrier-on/ADC-off and ADC-on/carrier-off states.
- Asymmetric rail collapse and residual-charge behavior.
- Aggregate leakage, inactive-rail rise, and pin-relative voltage.
- Absence of phantom powering, unintended SPI activity, and false CS assertion.
- SPI function and signal integrity with both rails valid.

All human-connected acquisition remains battery-only with USB physically absent and no mains-referenced instrumentation.
