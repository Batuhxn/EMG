# EMG V5 Current Status

Date: 2026-07-10

## Repository State

- Branch: `v5/KiCad`
- Current HEAD: `bc3c0d634800511806baf46fd018810f28c3e0ae`
- Tracking target: `origin/v5/KiCad`
- Ahead/behind after the latest fetch: `0 / 0`
- Current tracked V5 schematic root: `V5/EMG_v5.kicad_sch`
- Current power/reference sheet: `V5/POWER_REFERENCE_BLOCK.kicad_sch`
- Current analog sheets:
  - `V5/EMG_CHANNEL_1_ANALOG.kicad_sch`
  - `V5/EMG_CHANNEL_2_ANALOG.kicad_sch`

The repository also contains local untracked `V5/STATUS_V5.pdf` and `_recovery/` items. They are outside the synchronized tracked documentation state and must not be modified, added, staged, moved, or deleted without explicit approval.

All `*_REFINED.md` files remain untouched and read-only for the R208 schematic implementation.

## Current Architecture

EMG V5 is a controlled-development two-channel EMG acquisition system.

Each analog channel currently produces:

- `RAW`
- `RECT`
- `ENV`

The MCP3208 channel allocation is:

| MCP3208 channel | Current net |
|---|---|
| CH0 | `EMG1_RAW` |
| CH1 | `EMG1_RECT` |
| CH2 | `EMG1_ENV` |
| CH3 | `EMG2_RAW` |
| CH4 | `EMG2_RECT` |
| CH5 | `EMG2_ENV` |
| CH6 | `VREF_MON` |
| CH7 | `BAT_MON` |

The measured DSTK22807 carrier remains represented by:

- Symbol: `EMG_V5:DSTK22807_ESP32H2_SuperMini_Measured`
- Footprint: `EMG_V5:DSTK22807_ESP32H2_SuperMini_Carrier_Measured_THT`

The project-local symbol and measured footprint retain one-to-one numbering for pins/pads 1 through 18.

## Implemented First-Validation Power Path

The current tracked power/reference schematic implements the first-validation 3xAA path:

`J201 -> SW201 -> Q201 -> LDO_IN -> U203 -> R207 -> 3V3_ADC`

Current implementation details:

- `J201`: 3xAA battery connector.
- `SW201`: main high-side power switch.
- `Q201`: `PMV48XP` P-channel MOSFET used for pack-level reverse-polarity protection.
- Protected rail after Q201: `LDO_IN`.
- `U203`: `TPS7A2033PDBVR` 3.3 V LDO.
- U203 IN and EN are connected to protected `LDO_IN`.
- U203 OUT reaches `3V3_ADC` through `R207 = 0R`.
- `3V3_ADC` powers the analog and ADC domain.

This is a first-validation implementation. It is not final product power architecture, production MPN approval, BOM lock, human-test approval, or PCB approval.

### Power Component Engineering Basis

The current `3V3_ADC` planning basis remains:

- Approximate typical load estimate: 1.6 mA.
- Approximate conservative static/active subtotal: 2.5 mA.
- Controlled first-validation budget: 10 mA.
- Regulator capability planning floor: 50 mA.

These values are engineering estimates and planning margins, not measured rail current. The TPS7A2033 300 mA capability is regulator headroom; it is not the expected operating current.

TPS7A2033 remains the first-validation direction because the previous component review found a suitable combination of input-voltage margin for fresh 3xAA alkaline, low-load dropout, PSRR, output noise, quiescent current, capacitor stability, package practicality, and thermal margin. Reverse-current behavior remains a material constraint: no alternate source may hold `3V3_ADC` high after `LDO_IN` collapses unless a later protection review explicitly permits it.

At 4.8 V input, the simple LDO dissipation estimate is approximately 15 mW at the 10 mA validation budget and 75 mW at the 50 mA planning floor. Actual temperature depends on PCB copper, ambient conditions, package mounting, and measured load.

Q201 uses the recorded PMV48XP mapping `1=G`, `2=S`, `3=D`: gate to GND, drain to switched battery positive, and source to protected `LDO_IN`. Under correct polarity, initial body-diode conduction raises the source, makes VGS negative, and enhances the PMOS. Under reversed pack polarity, the intended body-diode direction blocks startup. This is pack-level protection only; one reversed AA cell may not be detected if the total series-pack output polarity remains positive. Lifecycle, availability, and final-production-MPN suitability remain open.

## Implemented BAT_MON Network

The current tracked schematic implements BAT_MON as:

- Source: protected `LDO_IN`.
- `R204 = 68k` from `LDO_IN` to `BAT_MON`.
- `R205 = 100k` from `BAT_MON` to GND.
- `C211 = 10nF` from `BAT_MON` to GND.
- `BAT_MON` is connected to MCP3208 CH7.

BAT_MON remains an unbuffered slow housekeeping measurement, not a precision voltmeter. The current first-validation architecture keeps the implemented network unchanged: no buffer is added, U202B is not repurposed, and R204, R205, and C211 retain their current values. Full 12-bit absolute accuracy is not claimed. Its real accuracy and settling behavior are not yet bench-validated.

### BAT_MON Engineering Basis

- Divider ratio: `100k / (68k + 100k)`, approximately 0.595.
- BAT_MON at 4.5 V: approximately 2.679 V.
- BAT_MON at 4.8 V: approximately 2.857 V.
- BAT_MON at the 5.5 V review point: approximately 3.274 V.
- Divider current at 4.8 V: approximately 28.6 uA.
- Thevenin resistance: approximately 40.5 kOhm.
- RC time constant with C211 = 10 nF: approximately 0.405 ms.
- Five time constants: approximately 2.0 ms.

MCP3208 CH7 source impedance is high relative to the manufacturer-characterized range. A first-validation candidate sampling policy keeps BAT_MON outside the time-critical fast EMG scan loop, performs one dummy CH7 conversion after channel selection, waits at least 5 ms before retaining a measurement, and uses a slow housekeeping cadence. Averaging may reduce random noise. Dummy conversion, delay, and averaging do not eliminate resistor-tolerance, ADC-input-leakage, ADC_REF, or other systematic errors. Exact cadence, retained-sample count, averaging depth, thresholds, guard bands, and calibration policy remain unresolved.

U202B is currently configured as a safe unloaded unity follower on `analog VREF` and appears available for possible future reassignment. Reusing it as a BAT_MON buffer would supersede the existing safe-unused-channel decision and would require a separately approved stability, headroom, output-isolation, and schematic review. No reassignment is currently approved. Hardware buffering remains a future-revision reconsideration option if first-validation measurements establish a need.

Future BAT_MON prototype validation is a no-human-connected gate. When physical current-V5 hardware is available, it should compare ADC-derived pack voltage against a calibrated DMM over representative `LDO_IN` values, compare immediate and delayed CH7 conversions, and exercise representative preceding-channel transitions. This decision establishes no precision, PCB-readiness, human-test, or medical-device approval claim.

The previous 330k/1M alternative is rejected for this architecture because it would produce approximately 3.61 V at 4.8 V input, above the intended 3.3 V ADC-reference operating range.

## Current Power and Reference Separation

- `3V3_ADC` is the analog/ADC supply rail.
- `ADC_REF` is the MCP3208 full-scale reference.
- `analog VREF` is the approximately mid-supply analog bias/reference.
- `ADC_REF` and `analog VREF` remain separate nets.
- `R206 = 0R` connects `3V3_ADC` to `ADC_REF`; MCP3208 VDD remains on `3V3_ADC` and VREF remains on the derived `ADC_REF` net. The zero-ohm link avoids an intentional load-dependent reference drop; a separate external reference is not justified for first validation.
- `C205 = 100nF` from `ADC_REF` to GND is the local high-frequency bypass, and populated `C206 = 1uF` from `ADC_REF` to GND provides local charge storage consistent with MCP3208 manufacturer application guidance.
- PCB entry must place C205 closest to MCP3208 pins 15/14, with C206 adjacent, R206 at the `ADC_REF` branch entry, a short `ADC_REF` route away from digital switching, and no digital return current routed through the reference-capacitor ground return.
- Actual `ADC_REF` ripple, conversion-correlated disturbance, final SPI-clock interaction, and bench behavior remain validation gates. Exact capacitor MPNs, packages, dielectric, tolerance, and voltage rating remain unresolved; startup discard/blanking remains a candidate firmware policy rather than a finalized requirement.
- The implemented VREF monitor interface is `analog VREF -> R203 1k -> VREF_MON -> MCP3208 CH6`, with `C207 = 220pF` from `VREF_MON` to GND. C207 is finalized for first validation as a balance between sampling-kickback suppression and acquisition settling.
- `C208 = 100nF` is finalized as U202's dedicated local high-frequency bypass from `3V3_ADC` to GND. PCB placement must keep C208 approximately within 2 mm of U202 supply pins 8 and 4 with a short, low-inductance return path; same-net schematic connectivity alone does not establish correct physical decoupling.
- C203 remains a separate 100 nF rail/local-bypass candidate whose role and placement are unresolved. C204 remains a separate `1uF or 4.7uF` bulk-capacitance candidate and does not replace C208's local high-frequency function.
- DSTK 3V3 and 5V/VBUS are not connected to `3V3_ADC`.

Simplified engineering modeling for C207 estimates an approximately 275 mV conservative full-scale local charge-sharing kick, approximately 0.24 us RC time constant, approximately 1.95 us recovery to 0.1 LSB, approximately 0.0013 LSB residual at 500 kHz, and approximately 0.66 LSB residual at 1 MHz. These are model estimates, not manufacturer guarantees or bench measurements. `VREF_MON` remains a lower-cadence diagnostic candidate rather than a time-critical fast-frame requirement; dummy-first CH6 sampling remains a candidate firmware policy, not finalized firmware behavior.

The shared analog-VREF architecture is accepted at schematic level for first validation. Analog-VREF dynamic movement, two-channel coupling, startup settling, and bench validation remain open; no manufacturer-guaranteed stability or bench-validated absence of coupling is claimed. Other open gates include C203 role/placement, C204 exact value, U202 exact MPN/footprint, and `ADC_REF` ripple/noise, conversion-correlated disturbance, final SPI-clock interaction, and bench validation of the implemented R206/C205/C206 network. AGND and DGND are controlled return and placement concepts on a common ground system; they are not approval for blind split planes or floating ground islands.

## Analog Channel and Interface State

- Both Channel 1 and Channel 2 analog schematics are implemented as controlled candidates.
- Connector mapping for both channels is Tip -> `EMGx_IN_P`, Ring -> `EMGx_IN_N`, Sleeve -> `EMGx_REF_ELECTRODE`.
- Recorded cable mapping is yellow tip -> Tip, green middle body -> Ring, red rear body -> Sleeve.
- RAW gain polarity is corrected: each RAW HPF node drives the non-inverting input, while the RAW gain feedback node drives the inverting input.
- Reference-electrode nets remain isolated from GND, chassis, and USB.

### RECT and ENV State

- U302A/U402A implement RAW gain; U302B/U302C and U402B/U402C implement the VREF-centered RECT candidate; U302D/U402D buffer ENV.
- RECT uses the two-op-amp, two-Schottky, 10k/20k weighted network and drives the 470R/ADC-cap output path.
- ENV is derived from RECT through the 33k/1uF storage path referenced to `analog VREF`, then buffered to the ADC output network.
- D331/D332/D431/D432 use the `BAS70ZFILM` / SOD-123 direction. Cathode-band orientation must be checked against the current schematic and footprint before PCB work.
- Ambiguous SOT-23 single-diode mappings and BAS70/BAT54 dual/common variants are not approved substitutes without a new symbol/footprint review.
- U302/U402 first-prototype direction remains socketed `MCP6004-I/P`, PDIP-14; orderability plus 3.3 V common-mode/output-swing behavior remain open.
- Ratio-critical RECT resistors use the current 0.1% thin-film 0805 direction for R333/R334/R336/R337/R338 and the corresponding Channel 2 parts; R335/R435 may remain 1% thin-film.
- C321/C331 and Channel 2 equivalents retain 1 nF C0G/NP0 0805 direction; C351/C451 retain 4.7 nF C0G/NP0 0805 preferred direction; C341/C441 retain 1 uF X7R 0805; C360/C460 retain 100 nF X7R 0805.
- C341/C441 tolerance and DC-bias derating affect ENV cutoff, and the storage load must remain part of the analog-VREF buffer stability review.

### Analog Validation Basis and Limits

- Behavioral simulation established the target `RECT_OUT = VREF + abs(RAW_IN - VREF)` and exercised the RECT/ENV loads.
- A real-topology candidate used explicit op-amp stages, Schottky approximations, and the resistor network.
- A conservative-RRIO candidate added finite gain/bandwidth and output-swing limits.
- The initial conservative model error was traced to reversed macromodel transconductance polarity and corrected without changing the intended rectifier topology.
- The simulations used generic op-amp and diode models rather than locally locked MCP600x and BAS70 vendor models.

This evidence supports schematic-level review and bench-test planning only. It does not establish final hardware correctness, production readiness, or manufacturer-level compliance.

## Current SPI State and Unequal-Power Constraint

The current SPI mapping is:

| SPI net | U501 GPIO | U501 pin | MCP3208 function |
|---|---:|---:|---|
| `ADC_CS` | GPIO14 | 13 | U201 `~CS/SHDN`, pin 10 |
| `ADC_SCLK` | GPIO13 | 14 | U201 CLK, pin 13 |
| `ADC_MOSI` | GPIO12 | 15 | U201 Din, pin 11 |
| `ADC_MISO` | GPIO11 | 16 | U201 Dout, pin 12 |

The four SPI signals remain direct controller-to-ADC connections with no series resistors, pull-downs, buffers, bus switches, or isolators. `R208 = 10k` is implemented on the MCP3208 side from `ADC_CS` to `3V3_ADC`. The carrier power path is not modeled in the schematic, and the GPIO/pad mapping remains provisional because official DSTK22807 carrier documentation is unavailable.

The unequal-power architecture decision is now established but not implemented:

- The current direct connection is not accepted as safe for unequal-power states.
- Physical disconnect of `ADC_CS`, `ADC_SCLK`, `ADC_MOSI`, and `ADC_MISO` is required before USB attachment, one-sided power, deliberate collapse of either domain, or programming/debugging with unequal power states.
- Removing the removable DSTK carrier may satisfy the requirement only if removal opens all four nets and no alternate conductive path remains.
- Firmware-only high-impedance policy, series resistors alone, and procedure-only prohibition are not accepted as sufficient standalone protection.
- No power-domain-aware buffer or digital isolator is selected for the current prototype.

The manufacturer-evidence basis is that MCP3208 digital pins are limited to `VSS - 0.6V` through `VDD + 0.6V`; a 3.3V high is therefore outside the published absolute maximum when MCP3208 VDD is 0V. No manufacturer-published safe off-power injection-current limit was found for resistor-only protection. MCP3208 DOUT behavior at VDD = 0V and DSTK carrier GPIO clamp behavior remain unspecified or undocumented.

Future project-level bench acceptance must show one action opens all four nets, no alternate path remains, open-state resistance is at least 10 MOhm, SPI-caused unpowered `3V3_ADC` rise is no more than 50mV, and injected current remains below the 1uA-per-signal project ceiling with a 0uA target. The 50mV and 1uA criteria are project thresholds, not manufacturer limits. Unequal-power validation must have no human connection.

MCP3208-side `R208 = 10k` now holds `CS/SHDN` high when controller drive is absent, keeping the ADC deselected while it remains powered. Calculated CS-low current at 3.3V is approximately 330uA and R208 dissipation is approximately 1.09mW. These are schematic calculations, not bench measurements. The pull-up does not replace the four-line physical-disconnect requirement. The tracked firmware SPI pin mapping remains separately unresolved.

## Legacy Placeholder State

The following legacy source-selection placeholders are absent from the current tracked V5 schematics:

- `JP201`
- `JP202`
- `DSTK_3V3_CANDIDATE`
- `LDO_3V3_FALLBACK`

## Fresh ERC State

The latest fresh KiCad 10 ERC result for `V5/EMG_v5.kicad_sch` is:

- 0 errors
- 1 warning related to unresolved `EMG_V5` footprint-library configuration

The warning is a footprint-library configuration/resolution warning. It is not an electrical design error.

Earlier issue categories closed during schematic development included MCP3208 SPI input-drive placeholders, isolated SPI labels, unused U501 pins, unplaced U202B, and stale MCP6004/Schottky symbol metadata. Obsolete intermediate ERC counts are not current status. Genuine future issues must not be hidden with No ERC markers.

## PCB State

`V5/EMG_v5.kicad_pcb` is a legacy, unsynchronized V4-style board. Its legacy references and nets do not represent the current V5 hierarchical schematic.

Therefore:

- It must not be treated as the current V5 PCB layout.
- It must not be used as evidence of V5 schematic-to-PCB synchronization.
- V5 PCB readiness is not claimed.
- PCB layout, routing, DRC, fabrication, and ordering approval remain outside the current state.

### PCB-Entry Conditions

Before current V5 PCB work can be approved:

- Assign and review missing footprints for current schematic components.
- Lock ordering MPNs where required, without treating schematic value text as purchase approval.
- Inspect D331/D332/D431/D432 cathode-band orientation against the selected footprints.
- Close MCP6004 package, orderability, common-mode, output-swing, and capacitive-load questions.
- Define required test points for power, reference, SPI, RAW, RECT, ENV, and electrode-input nodes.
- Define safety silkscreen and human-test warnings.
- Close analog stability, RECT crossover, ENV storage-load, and ADC input acquisition/source-impedance checks.
- Measure and accept `3V3_ADC`, `ADC_REF`, `analog VREF`, and `VREF_MON` behavior under relevant loads.
- Place the measured carrier using physical board dimensions; preserve antenna keepout and USB connector overhang.
- Create or synchronize a current V5 PCB from the current hierarchical schematic; do not reuse the legacy board as if synchronized.

## DSTK22807 Validation and Mechanical Gates

The measured removable THT carrier remains the approved mechanical direction for first prototype work. An unreviewed bare-module footprint is not an acceptable substitute because the project evidence is based on the physical DSTK22807 carrier, its measured header geometry, antenna side, and USB connector position.

Future DSTK validation must cover:

- 3V3 current capability and dummy-load testing.
- Loaded rail voltage and regulator temperature.
- ESP32-H2 reset and stability under load.
- BLE/RF-active rail noise and analog/ADC disturbance.
- USB coexistence and disconnected/connected source behavior.
- Physical-disconnect resistance, alternate-path, rail-rise, and injected-current validation for both unequal-power directions.
- CS-high behavior with implemented R208 during controller disconnect, reset, and boot conditions.
- Physical carrier dimensions, pin labels/orientation, antenna keepout, and USB overhang before placement approval.

## Evidence Limits and Open Work

- The repository does not contain a complete local datasheet set or populated MPN evidence for all components.
- Schematic/library consistency does not establish manufacturer-level component correctness.
- Exact component pinouts, package variants, operating limits, and application requirements still require local manufacturer evidence before final hardware approval.
- Multiple schematic parts still lack finalized footprints or ordering MPNs.
- Power-path and BAT_MON behavior still require controlled bench validation.
- The SPI physical-disconnect architecture is selected but remains unimplemented and unvalidated; direct unequal-power operation remains prohibited.
- MCP3208 ADC-side CS pull-up is implemented as `R208 = 10k`; bench validation remains open.
- Human-connected testing remains battery-only and still requires separate safety approval.

Human-connected operation additionally requires USB disconnected, no bench supply, no mains-connected test equipment, and no earth-referenced oscilloscope connection while electrodes are attached. EMG V5 is a prototype development system, not a medical device, and the current documentation is not human-test approval.

## Decision and Evidence Basis

Current technical state and open gates are based principally on:

- `V5/DSTK22807_PHYSICAL_PINOUT_OBSERVATION.md`
- `V5/DSTK22807_POWER_PIN_MEASUREMENT.md`
- `V5/DSTK22807_SPI_PIN_REVIEW.md`
- `V5/DSTK22807_3V3_RAIL_CURRENT_BUDGET_REVIEW.md`
- `V5/MCP3208_MPN_SYMBOL_FOOTPRINT_REVIEW.md`
- `V5/POWER_REFERENCE_ARCHITECTURE_REVIEW.md`
- `V5/POWER_REFERENCE_VALUE_LEVEL_DESIGN_REVIEW.md`
- `V5/FIRST_SCHEMATIC_POWER_SOURCE_STRATEGY.md`
- Current tracked V5 schematics and project-local libraries
- Rectifier planning/model notes under `V5/sim/rectifier/`
- Fresh KiCad ERC output described above

Current readiness: schematic implementation under controlled review; not PCB-ready, fabrication-ready, production-ready, or approved for human testing.
