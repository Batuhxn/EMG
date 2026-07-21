# DSTK22807 Physical Pinout Observation

This record preserves the user-observed header labels and identifies the mapping accepted by the current project. It is not an official manufacturer datasheet.

## Physical observation basis

- Pad 1 is the top-left pad and aligns with the observed `TX` label.
- The left row was observed top-to-bottom as TX, RX, 0, 1, 2, 3, 4, 5, 8.
- The right row was observed bottom-to-top as 9, 10, 11, 12, 13, 14, 3V3, GND, 5V.

## Current accepted pad mapping

| Pad | Observed label | Current status |
| --- | --- | --- |
| 1 | TX | Accepted physical observation |
| 2 | RX | Accepted physical observation |
| 3 | 0 | Accepted physical observation |
| 4 | 1 | Accepted physical observation |
| 5 | 2 | Accepted physical observation |
| 6 | 3 | Accepted physical observation |
| 7 | 4 | Accepted; current `ADC_SCLK` GPIO4 pad |
| 8 | 5 | Accepted physical observation |
| 9 | 8 | Accepted physical observation |
| 10 | 5V | Authoritative project mapping: `5V_VBUS` |
| 11 | GND | Authoritative project mapping: GND |
| 12 | 3V3 | Authoritative project mapping: carrier `3V3` / `CARRIER_3V3` |
| 13 | 14 | Accepted; current `ADC_CS` GPIO14 pad |
| 14 | 13 | Accepted; unassigned in current SPI mapping |
| 15 | 12 | Accepted; current `ADC_MOSI` GPIO12 pad |
| 16 | 11 | Accepted; current `ADC_MISO` GPIO11 pad |
| 17 | 10 | Accepted; unassigned spare |
| 18 | 9 | Accepted; unassigned with strapping caution |

Carrier `3V3` was physically observed at approximately 3.291–3.31 V during powered operation. Pads 10–12 are no longer blocked on a second photo or continuity confirmation for the implemented schematic.

## Remaining physical and electrical limits

The accepted mapping does not prove:

- external-power direction or safe 3V3 injection;
- carrier rail current capability;
- safe USB/external-power coexistence or reverse-current behavior;
- final mechanical orientation, USB overhang, antenna keepout, or production fit.

Those items remain bench or PCB-entry work, not mapping blockers.

## Safety

Human-connected acquisition remains battery-only. USB must be physically absent, and mains-referenced instrumentation must not be connected. The internal SPI protection circuit is not galvanic isolation.
