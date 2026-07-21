# EMG V5 Current Status

## Repository checkpoint

| Item | Current state |
| --- | --- |
| Branch | `v5/KiCad` |
| Hardware implementation HEAD | `bbe257e2c9dc28f7537942b5f15370770654ba19` |
| Upstream at checkpoint | `github-emg/v5/KiCad`, aligned `0 / 0` |
| Checkpoint condition | Clean working tree and index before this documentation-only synchronization |
| Latest hardware milestones | `223897a` TRS terminal correction; `bbe257e` unequal-power SPI isolation |

The committed schematics, symbols, and project files at this checkpoint are the source of truth. Bench validation remains open unless explicitly stated otherwise below.

## Implemented architecture

### Electrode connectors

Both channels use the same terminal convention:

| Connector | Tip / T | Ring / R | Sleeve / S |
| --- | --- | --- | --- |
| J301 | `EMG1_IN_P` | `EMG1_IN_N` | `EMG1_REF_ELECTRODE` |
| J401 | `EMG2_IN_P` | `EMG2_IN_N` | `EMG2_REF_ELECTRODE` |

The terminal mapping is implemented and is not an open design decision.

### First-validation power path

```text
3xAA
→ J201
→ SW201
→ Q201 reverse-polarity PMOS
→ LDO_IN
   ├→ U203 TPS7A2033
   │   → R207 0 Ω
   │   → 3V3_ADC
   └→ U204 TPS22917
       → CARRIER_VBUS_SW
       → U501 pad 10 / 5V_VBUS
```

Power-domain boundaries:

- U501 pad 10 is `5V_VBUS`.
- U501 pad 11 is GND.
- U501 pad 12 is the monitored carrier `3V3` rail, named `CARRIER_3V3` at the isolation boundary.
- Carrier `3V3` does not power `3V3_ADC`.
- Carrier `5V_VBUS` does not directly power the analog or ADC rail.
- `analog VREF` remains the analog midscale bias and is distinct from `ADC_REF`.
- `ADC_REF` is derived from `3V3_ADC` through R206 = 0 Ω.

### Carrier mapping evidence

The accepted project mapping is:

- U501 pad 10 = `5V_VBUS`
- U501 pad 11 = GND
- U501 pad 12 = carrier `3V3`

Carrier `3V3` was physically observed at approximately 3.291–3.31 V during powered operation. This mapping is authoritative project evidence and is no longer an implementation blocker. The observation does not establish external-power direction, current capability, or safe USB backfeeding.

### SPI GPIO mapping

| Function | Carrier GPIO | U501 pad | Direction |
| --- | --- | --- | --- |
| `ADC_CS` | GPIO14 | 13 | Carrier to ADC |
| `ADC_SCLK` | GPIO4 | 7 | Carrier to ADC |
| `ADC_MOSI` | GPIO12 | 15 | Carrier to ADC |
| `ADC_MISO` | GPIO11 | 16 | ADC to carrier |

The tracked firmware definitions are stale and do not yet match this schematic mapping.

### Unequal-power SPI isolation

U205 is `TXU0304PWR` with:

- VCCA = `CARRIER_3V3`
- VCCB = `3V3_ADC`
- A1 → B1Y: CS
- A2 → B2Y: SCLK
- A3 → B3Y: MOSI
- B4 → A4Y: MISO

Carrier-side nets are `MCU_ADC_CS`, `MCU_ADC_SCLK`, `MCU_ADC_MOSI`, and `MCU_ADC_MISO`. ADC-side nets remain `ADC_CS`, `ADC_SCLK`, `ADC_MOSI`, and `ADC_MISO`. The committed netlist has no direct SPI bypass around U205.

R208 remains 10 kΩ entirely on the ADC side from `ADC_CS` to `3V3_ADC`, holding MCP3208 CS/SHDN high while U205 is disabled.

### Supervisor and OE qualification

| Item | Implemented connection or value |
| --- | --- |
| U206 | `TPS3899DL30DSER`; monitors `CARRIER_3V3` |
| U207 | `TPS3899DL29DSER`; monitors `3V3_ADC` |
| Supervisor VDD | Both powered by `LDO_IN` |
| RESET outputs | Open-drain, shared on `SPI_ISO_OE` |
| Fault behavior | Either invalid rail forces OE low |
| Valid behavior | Both rails must be valid before OE rises |
| CTS | Open for minimum fault-side assertion delay |
| C218 / C219 | 9.1 nF each |
| Nominal valid-side qualification | Approximately 6.04 ms; not an exact guaranteed delay |
| R209 | 47 kΩ ±1%, `LDO_IN` to `SPI_ISO_OE` |
| R210 | 220 kΩ ±1%, `SPI_ISO_OE` to GND |

Decoupling and access:

- C214 = 100 nF for TXU VCCA.
- C215 = 100 nF for TXU VCCB.
- C216 = 100 nF for U206 VDD.
- C217 = 100 nF for U207 VDD.
- TP207 = `CARRIER_3V3`.
- TP208 = `SPI_ISO_OE`.

### BAT_MON, ADC allocation, and analog state

- The implemented BAT_MON network is R204 = 68 kΩ from `LDO_IN`, R205 = 100 kΩ to GND, and C211 = 10 nF to GND; it feeds MCP3208 CH7.
- BAT_MON remains an unbuffered slow housekeeping measurement. Accuracy, settling, and calibration are not bench-validated.
- MCP3208 CH0–CH7 remain allocated to `EMG1_RAW`, `EMG1_RECT`, `EMG1_ENV`, `EMG2_RAW`, `EMG2_RECT`, `EMG2_ENV`, `VREF_MON`, and `BAT_MON` respectively.
- Both analog-channel candidates remain implemented. RAW polarity is corrected; RECT and ENV remain candidate circuits requiring bench validation.
- `MCP6004-I/P` and `BAS70ZFILM` remain first-prototype directions with package, operating-margin, and orientation gates still open.

### Reference implementation

- R206 = 0 Ω derives `ADC_REF` from `3V3_ADC`; C205 = 100 nF and C206 = 1 µF provide local reference bypass/storage.
- R203 = 1 kΩ and C207 = 220 pF form the `VREF_MON` interface from analog VREF to MCP3208 CH6.
- C203 = 100 nF, C204 = 1 µF, and C208 = 100 nF retain their distinct ADC-supply, local-bulk, and U202-decoupling roles.
- Reference ripple, conversion disturbance, startup settling, channel coupling, and physical capacitor placement remain open validation items.

## Latest validated ERC

Root ERC at hardware checkpoint `bbe257e2c9dc28f7537942b5f15370770654ba19`:

- Errors: 0
- Warnings: 2
- Exclusions: 0

The existing warnings are:

1. U501 project-local footprint-library resolution warning.
2. U204 TPS22917 library-symbol mismatch.

The design is not warning-free.

## Safety boundary

SPI unequal-power protection is an internal common-ground power-sequencing measure. It is not galvanic isolation and is not a patient-protection barrier.

For human-connected acquisition:

- operation must be battery-only;
- USB must be physically absent; and
- mains-referenced instrumentation must not be connected.

The TXU0304 circuit does not make USB-connected human testing acceptable.

## Active blockers and open work

### Firmware

The tracked firmware SPI definitions must be updated in a separate task to CS GPIO14, SCLK GPIO4, MOSI GPIO12, and MISO GPIO11.

### Bench validation

Still required:

- OE rise delay and threshold behavior during rail ramps;
- OE low voltage with either supervisor asserted;
- carrier-powered / ADC-off and ADC-powered / carrier-off tests;
- asymmetric rail-collapse and residual-charge tests;
- aggregate leakage, inactive-rail rise, and pin-relative voltage;
- absence of phantom powering, unintended SPI activity, and false CS assertion;
- SPI function and signal integrity with both rails valid;
- carrier startup and RF-burst rail behavior;
- BAT_MON accuracy;
- `3V3_ADC` dropout and noise; and
- `ADC_REF` and analog VREF disturbance.

TXU0304 remains bench-gated until the unequal-power acceptance criteria in `DECISIONS_V5.md` pass.

### PCB readiness

The project is not ready for PCB implementation. Remaining blockers include:

- unresolved footprint assignments across much of the design;
- carrier mechanical orientation and antenna/USB keepout completion;
- the U501 project-local library-resolution warning;
- the U204 TPS22917 symbol mismatch warning;
- placement, routing, DRC, EMC, thermal, and fabrication review; and
- bench validation of the implemented power, reference, and SPI boundaries.

The legacy unsynchronized PCB must not be used as V5 implementation evidence.

## Next recommended technical action

Synchronize the tracked firmware SPI GPIO definitions with the committed schematic mapping: CS GPIO14, SCLK GPIO4, MOSI GPIO12, and MISO GPIO11.

## Readiness verdict

- Schematic architecture: implemented at the stated checkpoint.
- Bench qualification: not complete.
- PCB implementation readiness: **NOT READY**.
