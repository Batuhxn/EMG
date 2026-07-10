# EMG V5 Decisions - Refined

Date synchronized: 2026-07-10

## Locked Architecture

- Two EMG channels, each producing `RAW`, `RECT`, and `ENV`.
- MCP3208 on `3V3_ADC`.
- Channel allocation:
  - CH0-CH2: `EMG1_RAW`, `EMG1_RECT`, `EMG1_ENV`
  - CH3-CH5: `EMG2_RAW`, `EMG2_RECT`, `EMG2_ENV`
  - CH6: `VREF_MON`
  - CH7: `BAT_MON`
- `ADC_REF` and `analog VREF` remain separate.

## DSTK22807 Mapping

- Use `EMG_V5:DSTK22807_ESP32H2_SuperMini_Measured` with `EMG_V5:DSTK22807_ESP32H2_SuperMini_Carrier_Measured_THT`.
- Pins/pads 1-18 remain one-to-one.
- SPI mapping:
  - `ADC_CS`: U501 pin 13 / GPIO14
  - `ADC_SCLK`: U501 pin 14 / GPIO13
  - `ADC_MOSI`: U501 pin 15 / GPIO12
  - `ADC_MISO`: U501 pin 16 / GPIO11

## First-Validation Power Decision

The implemented first-validation path is:

`J201 -> SW201 -> Q201 -> LDO_IN -> U203 -> R207 -> 3V3_ADC`

- Source: 3xAA alkaline for first validation only.
- Q201: `PMV48XP` pack-level reverse-polarity PMOS.
- U203: `TPS7A2033PDBVR`.
- R207: `0R` output/current-measure link.
- DSTK 3V3 and 5V/VBUS must not feed `3V3_ADC`.
- No other source may hold `3V3_ADC` high while the LDO input is collapsed without a separately approved protection topology.

## BAT_MON Decision

- Source: protected `LDO_IN`
- `R204 = 68k`
- `R205 = 100k`
- `C211 = 10nF`
- Destination: MCP3208 CH7
- Accuracy and settling require bench measurement; exact ordering MPNs remain open.

## Mandatory Constraints

- Powered DSTK22807 with unpowered/collapsing MCP3208 remains a forbidden first-validation SPI condition.
- Firmware-only high-impedance handling and candidate SPI series resistors are not complete off-power safety solutions.
- Human-connected testing remains battery-only, with USB disconnected, and still requires separate safety approval.
- `V5/EMG_v5.kicad_pcb` is a legacy, unsynchronized V4-style board and must not be treated as the current V5 layout or as PCB-readiness evidence.
- `R206 = 0R/10R candidate` and other unresolved candidate values remain unresolved.
- Manufacturer-level correctness must not be claimed without local datasheet or populated MPN evidence.
- No PCB, fabrication, or production approval follows from the current schematic decisions.

## Rejected or Blocked Alternatives

- 3xAA NiMH is not equivalent to 3xAA alkaline because its nominal voltage leaves substantially less LDO dropout margin.
- Protected 1S Li-ion/LiPo is deferred pending charger/protection, backfeed, noise, and safety review.
- DSTK 3V3 and 5V/VBUS are not approved analog/ADC sources pending load/noise/USB/backfeed evidence.
- The former selectable/jumper source approach is superseded by the implemented single first-validation path; reintroduction requires a new mutual-exclusion/backfeed decision.
- Ambiguous SOT-23 or dual-diode rectifier substitutions are blocked without explicit symbol/footprint review.
- An unreviewed bare-module footprint is blocked; first prototype work uses the measured removable DSTK22807 THT carrier direction.

## Decision Tree

1. Keep human-connected operation battery-only with USB and earth/mains-connected equipment absent.
2. Use the implemented 3xAA alkaline -> PMV48XP -> TPS7A2033 -> `3V3_ADC` path for controlled first validation.
3. Keep DSTK power isolated from the analog/ADC rail and treat SPI off-power behavior as unresolved.
4. Bench-check power, BAT_MON, ADC/VREF, analog stability, and RECT/ENV behavior.
5. Close footprints, ordering MPNs, carrier placement, antenna/USB constraints, test points, and safety markings.
6. Only then request separate approval for a current V5-synchronized PCB.

## Open Gates Before PCB

- Actual `3V3_ADC` current, dropout, noise, temperature, capacitor stability, and reverse-current behavior.
- PMV48XP single-cell reversal limitation plus lifecycle/final-MPN status.
- MCP3208 acquisition/source impedance, VDD/VREF, `VREF_MON`, analog-VREF stability, and `ADC_REF` ripple.
- Diode orientation, MCP6004 orderability/3.3 V behavior, ratio parts, capacitor derating, and generic-model limitations.
- DSTK dummy-load/current/noise/reset/USB/backfeed measurements.
- Final footprints, ordering MPNs, test points, safety silkscreen, mechanical carrier placement, antenna keepout, USB overhang, and current V5 board synchronization.
