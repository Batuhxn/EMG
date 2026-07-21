# EMG V5 Current Decisions and Constraints

This file records current load-bearing decisions and acceptance gates. Historical planning records remain useful evidence but do not override these decisions or the committed implementation at `bbe257e2c9dc28f7537942b5f15370770654ba19`.

## 1. Development control

- Current hardware branch: `v5/KiCad`.
- Authoritative hardware checkpoint: `bbe257e2c9dc28f7537942b5f15370770654ba19`.
- Current schematic and committed netlist take precedence over older proposal documents.
- Bench measurements must not be described as passed until they have been performed and recorded.

## 2. System and ADC allocation

- The system has two EMG acquisition channels, each with `RAW`, `RECT`, and `ENV` nodes.
- The analog and ADC supply rail is `3V3_ADC`.
- The first-prototype ADC remains `MCP3208-CI/P`, PDIP-16, using `Analog_ADC:MCP3208` and `Package_DIP:DIP-16_W7.62mm`.

The MCP3208 allocation remains:

| Channel | Net |
| --- | --- |
| CH0 | `EMG1_RAW` |
| CH1 | `EMG1_RECT` |
| CH2 | `EMG1_ENV` |
| CH3 | `EMG2_RAW` |
| CH4 | `EMG2_RECT` |
| CH5 | `EMG2_ENV` |
| CH6 | `VREF_MON` |
| CH7 | `BAT_MON` |

## 3. Electrode terminal mapping

The final implemented mapping is:

- J301 Tip / T → `EMG1_IN_P`
- J301 Ring / R → `EMG1_IN_N`
- J301 Sleeve / S → `EMG1_REF_ELECTRODE`
- J401 Tip / T → `EMG2_IN_P`
- J401 Ring / R → `EMG2_IN_N`
- J401 Sleeve / S → `EMG2_REF_ELECTRODE`

The electrode terminal mapping is resolved.

## 4. Carrier pad and SPI mapping

The following carrier mapping is accepted as authoritative project evidence:

- U501 pad 10 = `5V_VBUS`
- U501 pad 11 = GND
- U501 pad 12 = carrier `3V3`

Carrier `3V3` was physically observed at approximately 3.291–3.31 V during powered operation. This evidence resolves the mapping used by the schematic but does not approve external power injection or USB backfeeding.

The final schematic SPI GPIO mapping is:

- GPIO14 → `ADC_CS`
- GPIO4 → `ADC_SCLK`
- GPIO12 → `ADC_MOSI`
- GPIO11 ← `ADC_MISO`

Firmware synchronization is a separate open task.

GPIO4 was selected for SCLK instead of GPIO13 because the reviewed ESP32-H2 SuperMini board-family evidence indicated an avoidable onboard LED load on GPIO13, while GPIO4 had no known onboard peripheral conflict and provides the native SPI2 `FSPICLK` function. Exact carrier-revision behavior and assembled waveform quality remain bench gates.

## 5. Power architecture boundaries

The implemented first-validation power path is:

```text
3xAA → J201 → SW201 → Q201 reverse-polarity PMOS → LDO_IN
LDO_IN → U203 TPS7A2033 → R207 0 Ω → 3V3_ADC
LDO_IN → U204 TPS22917 → CARRIER_VBUS_SW → U501 pad 10 / 5V_VBUS
```

Decisions:

- U501 pad 12 provides the monitored carrier `3V3` rail.
- Carrier `3V3` does not power `3V3_ADC`.
- Carrier `5V_VBUS` does not directly power the analog or ADC rail.
- `analog VREF` and `ADC_REF` are separate nets with separate purposes.
- `ADC_REF` is derived from `3V3_ADC` through R206 = 0 Ω.
- Settled component values are not reopened unless the active schematic directly contradicts this record.

Preserved first-validation constraints and engineering basis:

- The 3xAA alkaline source is for first validation, not a final product-battery decision. The controlled `3V3_ADC` design budget remains 10 mA, with a 50 mA regulator-capability planning floor; TPS7A2033 capability must not be described as expected load current.
- Q201 remains `PMV48XP` with physical mapping 1=G, 2=S, 3=D; gate to GND, drain to switched battery positive, and source to `LDO_IN`. This protects against reversed pack polarity but does not guarantee detection of one reversed cell when total pack polarity remains positive. Lifecycle and production-MPN suitability remain open.
- U204 VIN and ON remain on `LDO_IN`. C212 = 2.2 nF remains the CT-to-VIN timing capacitor; the reviewed output-rise estimate is approximately 3.5–4.0 ms and is a typical planning value, not a guaranteed measurement.
- U204 QOD remains floating. C213 = 22 µF remains on `CARRIER_VBUS_SW`; effective capacitance, startup peak current, RF-burst droop, and backfeed behavior remain validation gates.
- TPS7A2033 reverse-current behavior remains an open constraint if its output is held high after input collapse.

## 6. BAT_MON decision

The implemented slow housekeeping monitor remains:

- R204 = 68 kΩ from `LDO_IN` to `BAT_MON`.
- R205 = 100 kΩ from `BAT_MON` to GND.
- C211 = 10 nF from `BAT_MON` to GND.
- `BAT_MON` connects to MCP3208 CH7.

It remains unbuffered and is not a precision-voltmeter claim. The retained engineering basis is a divider ratio of approximately 0.595, approximately 2.679 V at 4.5 V input, 2.857 V at 4.8 V, and 3.274 V at the 5.5 V review point; divider current is approximately 28.6 µA at 4.8 V, Thevenin resistance approximately 40.5 kΩ, and the C211 time constant approximately 0.405 ms. A dummy conversion and at least 5 ms settling remain candidate firmware policy, not completed validation or a finalized firmware requirement. Accuracy, calibration, channel-history disturbance, and settling remain bench gates. U202B is not reassigned as a BAT_MON buffer.

## 7. Reference-rail decisions

- MCP3208 VDD remains on `3V3_ADC`; `ADC_REF` remains derived through R206 = 0 Ω.
- C205 = 100 nF and C206 = 1 µF remain the local `ADC_REF` bypass/storage components.
- `analog VREF` remains the shared analog midscale bias and must not be merged with `ADC_REF`.
- `analog VREF` reaches MCP3208 CH6 as `VREF_MON` through R203 = 1 kΩ, with C207 = 220 pF to GND.
- C203 = 100 nF remains the MCP3208 VDD-local bypass, C204 = 1 µF remains local/distributed bulk support, and C208 = 100 nF remains U202 local bypass.
- U202A remains the analog-VREF buffer direction; U202B remains a safe unloaded unity follower unless a later review approves reassignment.
- AGND and DGND remain controlled placement/return concepts on one common ground system; blind split or floating ground islands are not approved.

Actual ripple, conversion-correlated disturbance, startup behavior, two-channel coupling, effective-capacitance derating, and placement-dependent behavior remain bench and PCB gates. Earlier C207 charge-sharing and recovery figures are engineering-model estimates, not manufacturer guarantees or measurements.

The preserved simplified C207 model estimated an approximately 275 mV conservative full-scale local charge-sharing kick, approximately 0.24 µs RC time constant, approximately 1.95 µs recovery to 0.1 LSB, approximately 0.0013 LSB residual at 500 kHz, and approximately 0.66 LSB residual at 1 MHz. These figures remain planning evidence only.

## 8. Analog-interface decisions

- Both analog channel candidates remain implemented with Tip → positive input, Ring → negative input, and Sleeve → reference electrode.
- RAW gain polarity remains non-inverting: the RAW HPF node drives `+` and the feedback divider drives `−` for U302A/U402A.
- U302B/U302C and U402B/U402C implement the VREF-centered RECT candidate; U302D/U402D buffer ENV.
- D331/D332/D431/D432 retain the `BAS70ZFILM` SOD-123 direction; cathode-band orientation remains a PCB-entry inspection gate.
- The first-prototype op-amp direction remains socketed `MCP6004-I/P`, PDIP-14, with orderability and 3.3 V common-mode/output-swing behavior still open.
- Ratio-critical RECT resistors R333/R334/R336/R337/R338 and their Channel 2 equivalents retain the 0.1% thin-film 0805 direction; R335/R435 may remain 1% thin-film.
- C321/C331 and Channel 2 equivalents remain 1 nF C0G/NP0 0805; C351/C451 remain 4.7 nF C0G/NP0 0805 preferred; C341/C441 remain 1 µF X7R 0805; C360/C460 remain 100 nF X7R 0805.

Behavioral, real-topology, and conservative-RRIO simulations support candidate behavior and bench planning only. They do not establish final hardware correctness, manufacturer-model validation, medical compliance, or PCB readiness.

## 9. Physical four-line SPI isolation

All four SPI signals must cross a physical unequal-power boundary. U205 `TXU0304PWR` implements that boundary.

Rail assignments:

- U205 VCCA → `CARRIER_3V3`
- U205 VCCB → `3V3_ADC`
- U205 OE → `SPI_ISO_OE`

Signal directions:

- A1 → B1Y: CS
- A2 → B2Y: SCLK
- A3 → B3Y: MOSI
- B4 → A4Y: MISO

Carrier-side nets are `MCU_ADC_CS`, `MCU_ADC_SCLK`, `MCU_ADC_MOSI`, and `MCU_ADC_MISO`. ADC-side nets remain `ADC_CS`, `ADC_SCLK`, `ADC_MOSI`, and `ADC_MISO`.

No direct wire, global label, hierarchical pin, or passive may bypass U205. R208 remains 10 kΩ from ADC-side `ADC_CS` to `3V3_ADC` and must not pull the carrier GPIO directly.

## 10. Dual-supervisor OE decision

- U206 is `TPS3899DL30DSER` and monitors `CARRIER_3V3`.
- U207 is `TPS3899DL29DSER` and monitors `3V3_ADC`.
- Both supervisors are powered from `LDO_IN`, not from the rail each monitors.
- Both active-low open-drain RESET outputs share `SPI_ISO_OE`.
- Either invalid monitored rail must force OE low.
- Both rails must be valid before OE may rise.
- CTS remains open for minimum fault-side assertion delay.
- C218 and C219 are 9.1 nF.
- Valid-side qualification is approximately 6.04 ms nominal and is not an exact guaranteed delay.
- R209 is 47 kΩ ±1% from `LDO_IN` to `SPI_ISO_OE`.
- R210 is 220 kΩ ±1% from `SPI_ISO_OE` to GND.

Required local support:

- C214 = 100 nF at U205 VCCA.
- C215 = 100 nF at U205 VCCB.
- C216 = 100 nF at U206 VDD.
- C217 = 100 nF at U207 VDD.
- TP207 exposes `CARRIER_3V3`.
- TP208 exposes `SPI_ISO_OE`.

## 11. Unequal-power qualification criteria

The former below-1 µA-per-signal, at-least-10 MΩ, and 0 µA target criteria are **superseded project planning thresholds**. They were not manufacturer limits, were not safety-standard requirements, and were not equivalent to each other. Effective resistance is not a reliable model of semiconductor off-state behavior.

The current internal-interface acceptance criteria are:

1. In one-sided-power tests, the inactive rail starts below 10 mV.
2. The inactive rail remains below 100 mV at all times.
3. SPI-caused steady-state inactive-rail rise does not exceed 50 mV after at least 60 seconds and stabilization below 1 mV/s.
4. Every inactive-domain SPI pin remains between local GND − 0.3 V and inactive-rail voltage + 0.3 V.
5. Manufacturer-guaranteed candidate leakage is no worse than 2.5 µA magnitude per channel and 10 µA aggregate across four channels.
6. No functional phantom powering is allowed.
7. No unintended SPI activity or false CS assertion is allowed.
8. These are internal interface qualification limits, not patient-leakage limits.
9. TXU0304 remains bench-gated until these tests pass.

Bench validation has not yet passed.

## 12. Human-test safety policy

- SPI unequal-power isolation is an internal common-ground sequencing measure.
- It is not galvanic isolation and is not a patient-protection barrier.
- Human-connected acquisition requires battery-only operation.
- USB must be physically absent during human-connected acquisition.
- Mains-referenced instrumentation must not be connected during human-connected acquisition.
- The TXU0304 implementation does not authorize USB-connected human testing.

## 13. ERC and PCB gates

The validated root ERC state at the authoritative checkpoint is 0 errors, 2 warnings, and 0 exclusions. The two existing warnings are the U501 project-local footprint-library resolution warning and the U204 TPS22917 library-symbol mismatch.

PCB implementation is not approved. Footprint completion, carrier mechanical orientation and keepouts, warning resolution, placement, routing, DRC, EMC, thermal, fabrication review, and bench validation remain open. The legacy unsynchronized PCB is not V5 implementation evidence.
