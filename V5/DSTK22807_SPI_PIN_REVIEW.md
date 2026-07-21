# DSTK22807 SPI Pin and Isolation Review

This document reflects the committed implementation at `bbe257e2c9dc28f7537942b5f15370770654ba19`. Older direct-SPI and candidate-pin conclusions are superseded.

## Current carrier assignment

| SPI function | GPIO | U501 pad | Carrier-side net | ADC-side net |
| --- | --- | --- | --- | --- |
| CS | GPIO14 | 13 | `MCU_ADC_CS` | `ADC_CS` |
| SCLK | GPIO4 | 7 | `MCU_ADC_SCLK` | `ADC_SCLK` |
| MOSI | GPIO12 | 15 | `MCU_ADC_MOSI` | `ADC_MOSI` |
| MISO | GPIO11 | 16 | `MCU_ADC_MISO` | `ADC_MISO` |

GPIO13 is not the current SCLK assignment. The tracked firmware remains stale and must later be changed from its old definitions to CS GPIO14, SCLK GPIO4, MOSI GPIO12, and MISO GPIO11.

Preserved selection rationale: reviewed ESP32-H2 SuperMini board-family evidence indicated a conventional onboard LED branch on GPIO13, creating an avoidable clock-correlated load. GPIO4 had no known onboard peripheral conflict, is not a documented ESP32-H2 strapping pin, and provides the native SPI2 `FSPICLK` function. Exact carrier-revision behavior, GPIO13 LED details, user-board GPIO4 behavior, final clock rate, routing, and assembled SCLK waveform remain unresolved rather than assumed.

## Physical unequal-power boundary

U205 is `TXU0304PWR`:

| U205 channel | Direction | Function |
| --- | --- | --- |
| A1 pin 2 → B1Y pin 13 | Carrier to ADC | CS |
| A2 pin 3 → B2Y pin 12 | Carrier to ADC | SCLK |
| A3 pin 4 → B3Y pin 11 | Carrier to ADC | MOSI |
| B4 pin 10 → A4Y pin 5 | ADC to carrier | MISO |

Supply and control connections:

- Pin 1 VCCA → `CARRIER_3V3`
- Pin 14 VCCB → `3V3_ADC`
- Pin 7 GND → GND
- Pin 8 OE → `SPI_ISO_OE`
- Pins 6 and 9 are explicitly unconnected

The committed root netlist proves that the carrier-side and ADC-side SPI nets are distinct. No direct wire, global-label, hierarchical-label, or passive bypass remains.

R208 = 10 kΩ remains on the ADC side from `ADC_CS` to `3V3_ADC`. It holds MCP3208 CS/SHDN high while the translator is disabled and does not directly pull GPIO14.

At 3.3 V, the retained schematic calculation is approximately 330 µA when CS is driven low and approximately 1.09 mW in R208. These are calculations, not bench measurements.

## Rail supervisors and OE

- U206 `TPS3899DL30DSER` monitors `CARRIER_3V3`.
- U207 `TPS3899DL29DSER` monitors `3V3_ADC`.
- Both supervisor VDD pins are powered from `LDO_IN`.
- Both active-low open-drain RESET outputs share `SPI_ISO_OE`.
- Either invalid rail forces OE low; both rails must be valid before OE rises.
- CTS is open for minimum fault-side assertion delay.
- C218 and C219 are 9.1 nF.
- Valid-side qualification is approximately 6.04 ms nominal, not an exact guaranteed delay.
- R209 = 47 kΩ ±1% from `LDO_IN` to `SPI_ISO_OE`.
- R210 = 220 kΩ ±1% from `SPI_ISO_OE` to GND.

Local support is C214/C215 = 100 nF at U205 VCCA/VCCB and C216/C217 = 100 nF at U206/U207 VDD. TP207 exposes `CARRIER_3V3`; TP208 exposes `SPI_ISO_OE`.

## Unequal-power bench gate

The earlier 0 µA target, below-1 µA-per-signal ceiling, and 10 MΩ inferred open-state resistance are superseded planning criteria. They are not manufacturer limits or patient-safety limits, and effective resistance is not a reliable semiconductor off-state model.

Current acceptance requires:

1. Inactive rail initially below 10 mV.
2. Inactive rail below 100 mV at all times.
3. SPI-caused steady-state rise no more than 50 mV after at least 60 seconds and stabilization below 1 mV/s.
4. Every inactive-domain SPI pin between local GND − 0.3 V and inactive rail + 0.3 V.
5. Candidate guaranteed leakage no worse than 2.5 µA magnitude per channel and 10 µA aggregate.
6. No functional phantom powering.
7. No unintended SPI activity or false CS assertion.

These tests have not yet passed. TXU0304 remains bench-gated.

## Safety boundary

This circuit is internal common-ground unequal-power protection, not galvanic isolation or a patient-protection barrier. Human-connected acquisition remains battery-only with USB physically absent and no mains-referenced instrumentation.

## Remaining validation

- OE rail-ramp timing and asserted-low voltage
- Carrier-on/ADC-off and ADC-on/carrier-off states
- Asymmetric collapse and residual-charge behavior
- Inactive-rail rise, aggregate leakage, and pin-relative voltage
- Phantom-power and false-CS absence
- SPI function and signal integrity with both rails valid
- Carrier startup and RF-burst disturbance

Current decision: the pin mapping and schematic isolation architecture are implemented; firmware synchronization and bench qualification remain open.
