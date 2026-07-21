# EMG V5 Decisions — Refined Mirror

This is the maintained concise mirror of `DECISIONS_V5.md`. The primary decisions document controls for complete acceptance criteria.

## Locked mappings

- Two EMG channels retain `RAW`, `RECT`, and `ENV`; MCP3208 CH0–CH5 carry those signals, CH6 carries `VREF_MON`, and CH7 carries `BAT_MON`.
- J301: Tip `EMG1_IN_P`, Ring `EMG1_IN_N`, Sleeve `EMG1_REF_ELECTRODE`.
- J401: Tip `EMG2_IN_P`, Ring `EMG2_IN_N`, Sleeve `EMG2_REF_ELECTRODE`.
- U501 pad 10 = `5V_VBUS`, pad 11 = GND, pad 12 = carrier `3V3`.
- SPI: GPIO14 CS, GPIO4 SCLK, GPIO12 MOSI, GPIO11 MISO.

## Locked power boundaries

- Battery path: 3xAA → J201 → SW201 → Q201 → `LDO_IN`.
- ADC path: `LDO_IN` → U203 → R207 0 Ω → `3V3_ADC`.
- Carrier path: `LDO_IN` → U204 → `CARRIER_VBUS_SW` → U501 pad 10.
- Carrier `3V3` does not power `3V3_ADC`.
- `analog VREF` and `ADC_REF` remain separate; R206 = 0 Ω from `3V3_ADC` to `ADC_REF`.
- BAT_MON remains R204 = 68 kΩ, R205 = 100 kΩ, and C211 = 10 nF from protected `LDO_IN` to MCP3208 CH7; accuracy and settling remain bench gates.
- The implemented two-channel RAW/RECT/ENV analog candidates and their package, diode-orientation, stability, and model-limit gates remain in force.

## Locked SPI isolation architecture

- U205 = `TXU0304PWR`; VCCA = `CARRIER_3V3`; VCCB = `3V3_ADC`.
- CS, SCLK, and MOSI use A→B channels; MISO uses B→A.
- Carrier nets are `MCU_ADC_*`; ADC nets remain `ADC_*`.
- R208 = 10 kΩ on ADC-side `ADC_CS` to `3V3_ADC`.
- No direct or label-based bypass is permitted.

## Locked OE qualification

- U206 = `TPS3899DL30DSER`, sensing `CARRIER_3V3`.
- U207 = `TPS3899DL29DSER`, sensing `3V3_ADC`.
- Both supervisors use `LDO_IN`; RESET outputs share `SPI_ISO_OE`.
- CTS is open; C218/C219 = 9.1 nF; valid-side delay is approximately 6.04 ms nominal.
- R209 = 47 kΩ ±1%; R210 = 220 kΩ ±1%.
- C214–C217 = 100 nF; TP207 = `CARRIER_3V3`; TP208 = `SPI_ISO_OE`.

## Qualification and safety gates

- The former 1 µA, 10 MΩ, and 0 µA planning thresholds are superseded and are not manufacturer or safety-standard limits.
- Current gates use inactive-rail start/rise, pin-relative voltage, manufacturer leakage, no phantom powering, and no unintended SPI/CS activity.
- TXU0304 remains bench-gated; bench validation is not complete.
- Human-connected acquisition remains battery-only, USB-absent, and free of mains-referenced instrumentation.
- SPI protection is not galvanic isolation or a patient-protection barrier.

## Release gates

- Validated ERC at `bbe257e2c9dc28f7537942b5f15370770654ba19`: 0 errors, 2 warnings, 0 exclusions.
- Firmware GPIO synchronization remains open.
- PCB implementation remains blocked by footprint, mechanical, warning-resolution, bench, layout, DRC, EMC, thermal, and fabrication work.
