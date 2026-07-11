# EMG V5 Current Decisions and Constraints

Date synchronized: 2026-07-10

This document contains current project decisions and mandatory constraints. Repository status, historical checkpoint narration, and validation logs belong in `STATUS_V5.md`.

## 1. Development Control

- EMG V5 remains a controlled-development hardware project.
- Architecture, component selections, values, net names, pin mappings, footprints, power strategy, and ADC allocation must not change without explicit review and approval.
- No ERC marker may be added merely to suppress a genuine unresolved issue.
- PCB layout, routing, fabrication outputs, committing, and pushing require separate explicit approval.
- One logical change per commit remains the required workflow when commits are later authorized.

## 2. System Architecture

- The system has two EMG acquisition channels.
- Each channel physically produces `RAW`, `RECT`, and `ENV` nodes.
- The analog and ADC supply rail is `3V3_ADC`.
- The ADC is MCP3208.
- The first-prototype MCP3208 direction remains `MCP3208-CI/P`, PDIP-16, using `Analog_ADC:MCP3208` and `Package_DIP:DIP-16_W7.62mm`.

The MCP3208 channel allocation is locked as:

| MCP3208 channel | Net |
|---|---|
| CH0 | `EMG1_RAW` |
| CH1 | `EMG1_RECT` |
| CH2 | `EMG1_ENV` |
| CH3 | `EMG2_RAW` |
| CH4 | `EMG2_RECT` |
| CH5 | `EMG2_ENV` |
| CH6 | `VREF_MON` |
| CH7 | `BAT_MON` |

## 3. DSTK22807 Carrier Mapping

- The controller carrier is the measured DSTK22807 ESP32-H2 Super Mini board.
- The project symbol is `EMG_V5:DSTK22807_ESP32H2_SuperMini_Measured`.
- The measured footprint is `EMG_V5:DSTK22807_ESP32H2_SuperMini_Carrier_Measured_THT`.
- Pins 1 through 18 must remain mapped one-to-one to footprint pads 1 through 18 unless repository evidence establishes a correction and that correction is explicitly approved.

The SPI allocation is:

| SPI net | U501 GPIO | U501 pin | MCP3208 function |
|---|---:|---:|---|
| `ADC_CS` | GPIO14 | 13 | `~CS/SHDN` |
| `ADC_SCLK` | GPIO13 | 14 | CLK |
| `ADC_MOSI` | GPIO12 | 15 | Din |
| `ADC_MISO` | GPIO11 | 16 | Dout |

## 4. First-Validation Power Strategy

- The first analog validation source is a 3xAA alkaline battery pack.
- The implemented first-validation power path is:

  `J201 -> SW201 -> Q201 -> LDO_IN -> U203 -> R207 -> 3V3_ADC`

- J201 is the 3xAA battery connector.
- SW201 is the main high-side power switch.
- Q201 is the `PMV48XP` P-channel MOSFET used for pack-level reverse-polarity protection.
- Q201 gate is connected to GND, drain to switched battery positive, and source to protected `LDO_IN`.
- U203 is `TPS7A2033PDBVR`.
- U203 IN and EN are connected to protected `LDO_IN`.
- U203 OUT reaches `3V3_ADC` only through `R207 = 0R`.
- `3V3_ADC` must not be tied directly to DSTK 3V3 or DSTK 5V/VBUS.
- No other source may intentionally hold `3V3_ADC` high while the TPS7A20 input is collapsed unless a later reviewed protection topology explicitly permits it.
- The 3xAA path is for first validation only; it is not a final product battery or production architecture decision.

### TPS7A2033 Rationale and Planning Limits

- Recorded `3V3_ADC` estimates are approximately 1.6 mA typical and 2.5 mA conservative static/active subtotal.
- The controlled first-validation design budget is 10 mA.
- The regulator capability planning floor is 50 mA.
- TPS7A2033 output capability is headroom and must not be described as expected operating current.
- Selection rationale includes fresh-3xAA input-voltage margin, low-load dropout, PSRR, output noise, quiescent current, stable capacitor requirements, package/assembly practicality, thermal behavior, and reverse-current risk.
- Approximate LDO dissipation at 4.8 V input is 15 mW at 10 mA and 75 mW at 50 mA; measured load and eventual PCB thermal conditions remain controlling evidence.
- TPS7A2033 reverse current remains an unresolved constraint when output is held high after input collapse.

### PMV48XP Decision and Caveats

- Q201 uses physical mapping `1=G`, `2=S`, `3=D`.
- Intended topology is gate to GND, drain to `SW_BAT+`, source to protected `LDO_IN`.
- The prior component review recorded a -20 V P-channel device with low resistance at the relevant negative gate drive, supporting low-drop pack-level polarity protection for first validation.
- With correct polarity, the body diode initially conducts from switched battery toward `LDO_IN`; the rising source makes VGS negative and turns the PMOS on.
- With reversed pack polarity, the intended body-diode orientation blocks startup and the PMOS remains off.
- This does not guarantee detection or protection for one individually reversed AA cell when the total series-pack output polarity remains positive.
- PMV48XP lifecycle, prototype availability, and final-production-MPN status remain open.

### Rejected or Deferred Power Alternatives

- 3xAA alkaline is selected for first validation because it provides a simple isolated source without charger/load-sharing variables and allows low-noise LDO evaluation.
- 3xAA NiMH is not electrically equivalent: approximately 3.6 V nominal pack voltage leaves much less 3.3 V LDO dropout margin.
- Protected 1S Li-ion/LiPo remains deferred until charger/protection, low-voltage regulation, backfeed, analog-noise, and human-test safety behavior are separately reviewed.
- DSTK 3V3 remains rejected/deferred as the analog/ADC source because current capability, BLE/RF noise, USB coexistence, and backfeed behavior are not closed.
- DSTK 5V/VBUS remains rejected/deferred because it is USB-related, is not the selected 3.3 V analog domain, and would change ADC/SPI voltage compatibility and isolation assumptions.
- The former selectable/jumper strategy is superseded by the implemented single first-validation battery/LDO path. Reintroducing source selection would reopen mutual-exclusion and backfeed risks and requires a new decision.

## 5. BAT_MON Decision

The implemented BAT_MON network is locked for the current first-validation schematic as:

- Sense source: protected `LDO_IN`.
- `R204 = 68k` from `LDO_IN` to `BAT_MON`.
- `R205 = 100k` from `BAT_MON` to GND.
- `C211 = 10nF` from `BAT_MON` to GND.
- `BAT_MON` connects to MCP3208 CH7.

BAT_MON operating constraints:

- The current first-validation architecture keeps the existing unbuffered BAT_MON network.
- BAT_MON is a slow housekeeping measurement, not a precision voltmeter.
- Full 12-bit absolute accuracy is not guaranteed or claimed.
- No buffer is added, U202B is not repurposed, and R204, R205, and C211 are not changed by this decision.
- Hardware buffering is not justified before first-validation measurements establish a need.
- Actual accuracy and settling are not yet bench-validated and remain a future prototype-validation gate.
- Exact ordering MPNs for R204, R205, and C211 remain unlocked.

Engineering basis:

- Divider ratio is approximately 0.595.
- BAT_MON is approximately 2.679 V at 4.5 V pack input, 2.857 V at 4.8 V, and 3.274 V at the 5.5 V review point.
- Divider current is approximately 28.6 uA at 4.8 V.
- Thevenin resistance is approximately 40.5 kOhm.
- With C211 = 10 nF, the time constant is approximately 0.405 ms and five time constants are approximately 2.0 ms.
- MCP3208 CH7 source impedance is high relative to the manufacturer-characterized range.
- Dummy conversion and settling delay can mitigate channel-history disturbance but cannot eliminate resistor-tolerance, ADC-input-leakage, ADC_REF, or other systematic errors.
- The prior 330k/1M alternative is rejected because it produces approximately 3.61 V at 4.8 V input, above the intended 3.3 V ADC-reference operating range.

First-validation candidate sampling policy, not a finalized firmware requirement:

- Keep BAT_MON outside the time-critical fast EMG scan loop.
- Perform one dummy CH7 conversion after channel selection.
- Wait at least 5 ms before retaining a measurement.
- Use a slow housekeeping cadence; averaging may be used for random-noise reduction.
- Exact cadence, retained-sample count, averaging depth, thresholds, guard bands, and calibration policy remain unresolved.

Buffer-resource context:

- U202B remains configured as a safe unloaded unity follower on `analog VREF` and appears available for possible future reassignment.
- Reusing U202B for BAT_MON would supersede the existing safe-unused-channel decision and requires a separate approved stability, headroom, output-isolation, and schematic review.
- No U202B reassignment is approved now. A unity-gain buffer, preferably using an existing suitable spare op-amp channel if later justified, remains a future-revision reconsideration option.

Future no-human-connected prototype validation should compare ADC-derived pack voltage against a calibrated DMM over representative `LDO_IN` values, compare immediate and delayed CH7 conversions, and test representative preceding-channel transitions. This decision establishes no precision, PCB-readiness, human-test, or medical-device approval claim.

## 6. Reference-Rail Decisions

- `ADC_REF` is the MCP3208 full-scale reference.
- `analog VREF` is the analog midscale bias/reference.
- `ADC_REF` and `analog VREF` must remain separate.
- MCP3208 VDD remains `3V3_ADC`.
- U202A remains the analog-VREF buffer direction.
- U202B remains placed in its safe unused unity-follower configuration.
- Finalize `R206 = 0R` between `3V3_ADC` and `ADC_REF`. MCP3208 VDD remains on `3V3_ADC`, while pin 15 VREF remains on the derived `ADC_REF` net. The zero-ohm link avoids an intentional load-dependent reference drop; a separate external reference is not justified for first validation.
- Finalize `C205 = 100nF` and populate `C206 = 1uF`, each from `ADC_REF` to GND. C205 provides local high-frequency bypassing and C206 provides local charge storage consistent with MCP3208 manufacturer application guidance.
- PCB implementation constraint: place C205 closest to MCP3208 pins 15/14, place C206 adjacent, place R206 at the `ADC_REF` branch entry, keep `ADC_REF` short and away from digital switching, and do not route digital return current through the reference-capacitor ground return.
- This decision does not close actual `ADC_REF` ripple, conversion-correlated disturbance, final SPI-clock interaction, or bench validation. Exact capacitor MPNs, packages, dielectric, tolerance, and voltage rating remain unresolved. Startup discard/blanking remains a candidate firmware policy, not finalized behavior.
- The first-validation VREF monitor interface keeps `R203 = 1k` from `analog VREF` to `VREF_MON` and finalizes `C207 = 220pF` from `VREF_MON` to GND; `VREF_MON` remains connected to MCP3208 CH6.
- C207 is selected as a first-validation balance between sampling-kickback suppression and acquisition settling. Simplified modeling estimates an approximately 275 mV conservative full-scale local charge-sharing kick, approximately 0.24 us RC time constant, approximately 1.95 us recovery to 0.1 LSB, approximately 0.0013 LSB residual at 500 kHz, and approximately 0.66 LSB residual at 1 MHz. These are engineering-model estimates, not manufacturer guarantees.
- `VREF_MON` remains a lower-cadence diagnostic candidate rather than a time-critical fast-frame requirement. Dummy-first CH6 sampling remains a candidate firmware policy, not finalized firmware behavior.
- `C208 = 100nF` is finalized as U202's dedicated local high-frequency bypass from `3V3_ADC` to GND. PCB placement must keep it approximately within 2 mm of U202 supply pins 8 and 4 with a short, low-inductance return path; same-net connectivity alone does not satisfy this placement constraint.
- Finalize `C203 = 100nF` as the dedicated MCP3208 VDD-local high-frequency bypass and place it immediately adjacent to U201 pin 16/VDD with an appropriate low-inductance ground return; C203 alone is not claimed to satisfy every MCP3208 supply-bypass need. Finalize populated `C204 = 1uF` as distributed/local bulk support near the ADC and central analog-load region; the larger 4.7uF candidate is not justified without a measured transient requirement.
- C210 remains the TPS7A2033 regulator-output capacitor. C203, C204, C210, ADC_REF-local C205/C206, U202-local C208, INA333-local C310/C410, and MCP6004-local C360/C460 have distinct physical roles despite rail overlap. Their nominal approximately 4.9uF rail-effective bookkeeping total does not make them physically or electrically interchangeable, and no additional `3V3_ADC` decoupling redesign is currently justified.
- PCB implementation constraints: keep C210 close to U203 OUT/GND with a minimal regulator-output loop; C204 near the ADC/central analog cluster; C205 closest to U201 VREF/AGND with C206 adjacent; C208 approximately within 2 mm of U202 supply pins with a short return; and C310/C410 and C360/C460 at their respective INA333 and MCP6004 supply regions. Keep `ADC_REF` and `analog VREF` away from SPI switching edges, do not route digital return current through ADC/reference capacitor returns, and avoid ground splits that force signal-return detours.
- Actual rail ripple, conversion-correlated disturbance, transient response, startup behavior, effective-capacitance derating, and bench validation remain open. Exact capacitor MPNs, packages, dielectrics, tolerances, and voltage ratings remain unresolved; no measured or guaranteed assembled-board performance is established.
- The shared analog-VREF architecture is accepted at schematic level for first validation. Analog-VREF dynamic movement, two-channel coupling, startup settling, and bench validation remain open; manufacturer-guaranteed stability and bench-validated absence of coupling are not claimed.
- The finalized first-validation MCP3208 supply/reference topology is VDD on `3V3_ADC` and VREF on `ADC_REF` derived through R206; its actual dynamic performance remains a validation gate.
- ADC input source impedance and acquisition-time behavior must be reviewed for all eight channels.
- U202 exact MPN/footprint and actual `ADC_REF` dynamic behavior remain unresolved.
- AGND and DGND are controlled return and placement concepts on a common ground system. Blindly split or floating ground islands are not approved.

## 7. Analog Interface, RECT, and ENV Decisions

- Both analog channel candidates remain implemented.
- J301/J401 mapping is Tip -> `EMGx_IN_P`, Ring -> `EMGx_IN_N`, Sleeve -> `EMGx_REF_ELECTRODE`.
- Recorded physical cable mapping is yellow tip, green middle/Ring, and red rear/Sleeve.
- RAW gain polarity is non-inverting: the RAW HPF node drives `+` and the feedback divider drives `-` for U302A/U402A.
- U302B/U302C and U402B/U402C implement the VREF-centered RECT candidate; U302D/U402D implement ENV buffering.
- D331/D332/D431/D432 direction remains `BAS70ZFILM` in SOD-123 using the current two-pin Schottky symbol/footprint approach.
- Cathode-band orientation must be inspected against the schematic and footprint before PCB approval.
- Ambiguous SOT-23 single-diode mappings and BAS70/BAT54 dual/common variants are rejected unless a later explicit symbol/footprint review approves them.
- First-prototype op-amp package direction is socketed `MCP6004-I/P`, PDIP-14. Exact orderability and 3.3 V common-mode/output-swing behavior remain gates.
- Ratio-critical RECT resistors R333/R334/R336/R337/R338 and Channel 2 equivalents retain the 0.1% thin-film 0805 direction, preferably from one series. R335/R435 may remain 1% thin-film.
- C321/C331 and Channel 2 equivalents retain 1 nF C0G/NP0 0805; C351/C451 retain 4.7 nF C0G/NP0 0805 preferred; C341/C441 retain 1 uF X7R 0805; C360/C460 retain 100 nF X7R 0805.
- C341/C441 tolerance and DC-bias derating affect ENV cutoff; their load on `analog VREF` remains part of the buffer-stability gate.

### Analog Validation Decision Basis

- Behavioral simulation established the intended absolute-value target and output/storage loads.
- Real-topology simulation added explicit op-amp stages, Schottky approximations, and the resistor network.
- Conservative-RRIO simulation added finite gain, bandwidth, swing, and crossover limitations.
- A reversed transconductance-source polarity in the initial generic op-amp macromodel was corrected; this was a model repair, not a rectifier-topology decision.
- Exact MCP600x and BAS70 vendor models are not locally locked; generic op-amp and diode models limit the evidence to candidate behavior and bench-test planning.
- Simulation results do not grant final hardware, manufacturer-level, PCB, or production approval.

## 8. SPI Unequal-Power Architecture Decision

- The current direct DSTK22807-to-MCP3208 SPI connection is not accepted as safe for unequal-power states.
- The minimum accepted architecture for the removable-carrier prototype is physical disconnect of all four nets: `ADC_CS`, `ADC_SCLK`, `ADC_MOSI`, and `ADC_MISO`.
- One disconnect action must open all four signals before USB is attached, either side is powered alone, `3V3_ADC` is intentionally collapsed while DSTK remains powered, DSTK power is intentionally collapsed while MCP3208 remains powered, or programming/debugging occurs with unequal power states.
- Physically removing the removable DSTK carrier may satisfy this decision only if removal demonstrably opens all four SPI nets and no alternate conductive path remains.
- Firmware-only high-impedance policy, series resistors alone, and procedure-only unequal-power prohibition are rejected as sufficient standalone protection.
- Series resistors may still be reviewed for signal integrity or fault-current reduction, but they are not the selected off-power architecture and no value is locked.
- A power-domain-aware buffer, bus switch, or digital isolator is not selected for the current prototype.
- Manufacturer evidence limits MCP3208 digital pins to `VSS - 0.6V` through `VDD + 0.6V`; therefore a 3.3V high is outside the published absolute maximum when MCP3208 VDD is 0V.
- No manufacturer-published safe off-power injection-current limit was found for defensible resistor-only protection. MCP3208 DOUT behavior at VDD = 0V and DSTK carrier GPIO clamp behavior remain undocumented.
- Future physical-disconnect bench acceptance requires one-action four-line opening, no alternate path, at least 10 MOhm open-state resistance, no more than 50mV SPI-caused rise on unpowered `3V3_ADC`, and a 0uA injected-current target with a project ceiling below 1uA per signal.
- The 50mV and 1uA criteria are project-level bench acceptance thresholds, not manufacturer-published limits.
- Unequal-power validation must have no human connection.

### CS/SHDN Pull-Up Decision

- `R208 = 10k` is implemented from MCP3208-side `ADC_CS` to `3V3_ADC`.
- R208 must hold `CS/SHDN` high and keep the MCP3208 deselected when controller drive is absent.
- Calculated CS-low current at 3.3V is approximately 330uA; calculated resistor dissipation while low is approximately 1.09mW.
- These values are schematic calculations and do not constitute bench validation.
- R208 does not replace the required physical disconnect of all four SPI signals.
- The tracked firmware SPI pin mapping remains separately unresolved and is not approved or changed by this decision.

## 9. Human-Test Safety Constraints

- Human-connected EMG testing must remain battery-only.
- USB must be disconnected whenever electrodes are attached to a human.
- Bench supplies must not be connected while electrodes are attached to a human.
- Mains-connected test equipment must not be connected while electrodes are attached to a human.
- Earth-referenced oscilloscope connections must not be used while electrodes are attached to a human.
- Battery operation alone does not constitute final human-test approval.
- Electrode reference nets must not be tied to USB, chassis, or system GND without a separate approved safety architecture.
- PCB, enclosure, isolation, test procedure, and risk controls require separate review before any human-test approval.
- EMG V5 is a prototype development system, not a medical device.

## 10. PCB Constraint

- `V5/EMG_v5.kicad_pcb` must be treated as a legacy, unsynchronized V4-style board, not as the current V5 layout.
- The legacy PCB file must not be synchronized, edited, routed, used for fabrication, or treated as evidence of PCB readiness without explicit approval.
- Current V5 schematic work does not grant PCB readiness.
- PCB entry requires current schematic-to-board synchronization; finalized footprints and ordering MPNs; diode orientation inspection; MCP6004 package/orderability and 3.3 V behavior; test points; safety silkscreen; analog stability; ADC acquisition/source-impedance checks; `3V3_ADC`/`ADC_REF`/`analog VREF` validation; and mechanical carrier placement with antenna keepout and USB overhang.
- The measured removable THT carrier remains the first-prototype mechanical direction. An unreviewed bare-module footprint is rejected because it does not represent the measured DSTK22807 carrier geometry or its antenna/USB constraints.

## 11. Evidence Constraint

- Repository consistency is not manufacturer-level correctness.
- A component without local manufacturer datasheet evidence or a populated, reviewable MPN must not be described as manufacturer-approved or physically proven.
- Exact pinout, package, footprint, operating-limit, and application-circuit checks remain open where that evidence is absent.
- Existing unresolved candidate component values remain candidates and must not be promoted to final decisions without explicit review.

## 12. ERC and Decision Provenance

- Current ERC status belongs in `STATUS_V5.md`: 0 errors and one non-electrical `EMG_V5` footprint-library configuration warning.
- Previously resolved categories include SPI input-drive placeholders, isolated SPI labels, unused U501 pins, unplaced U202B, and stale embedded MCP6004/Schottky metadata.
- Superseded intermediate ERC counts are not decision evidence and must not be restored.
- No ERC marker may be used to suppress a genuine unresolved issue.

Major decision basis includes the DSTK physical/power/SPI reviews, MCP3208 symbol/footprint review, power architecture and value-level reviews, 3V3 current-budget review, current tracked schematics/project-local libraries, and rectifier planning/model notes under `V5/sim/rectifier/`.
