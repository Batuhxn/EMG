# EMG V5 Status — Refined Mirror

This is the maintained concise mirror of `STATUS_V5.md`. The primary status document and committed hardware at `bbe257e2c9dc28f7537942b5f15370770654ba19` control if detail is needed.

## Checkpoint

- Branch: `v5/KiCad`
- Hardware HEAD: `bbe257e2c9dc28f7537942b5f15370770654ba19`
- Upstream at checkpoint: aligned `0 / 0`
- Milestones: `223897a` corrected both TRS terminal mappings; `bbe257e` implemented unequal-power SPI isolation
- ERC: 0 errors, 2 warnings, 0 exclusions
- Existing warnings: U501 project-local footprint-library resolution; U204 TPS22917 symbol mismatch

## Implemented interfaces

- J301/J401 Tip → positive input, Ring → negative input, Sleeve → reference electrode.
- U501 pad 10 = `5V_VBUS`, pad 11 = GND, pad 12 = carrier `3V3`/`CARRIER_3V3`.
- Carrier 3V3 was observed at approximately 3.291–3.31 V during powered operation.
- SPI GPIOs: CS GPIO14, SCLK GPIO4, MOSI GPIO12, MISO GPIO11.
- U205 `TXU0304PWR`: VCCA `CARRIER_3V3`, VCCB `3V3_ADC`; CS/SCLK/MOSI A→B and MISO B→A.
- Carrier-side SPI nets use `MCU_ADC_*`; ADC-side nets remain `ADC_*`; no direct bypass remains.
- R208 remains 10 kΩ from ADC-side `ADC_CS` to `3V3_ADC`.

## Supervisor network

- U206 `TPS3899DL30DSER` monitors `CARRIER_3V3`.
- U207 `TPS3899DL29DSER` monitors `3V3_ADC`.
- Both VDD pins use `LDO_IN`; RESET outputs share `SPI_ISO_OE`.
- Either invalid rail holds OE low; both valid rails are required before release.
- CTS is open; C218/C219 are 9.1 nF; valid-side qualification is approximately 6.04 ms nominal.
- R209 = 47 kΩ ±1%; R210 = 220 kΩ ±1%.
- C214–C217 are 100 nF local decouplers; TP207 is `CARRIER_3V3`; TP208 is `SPI_ISO_OE`.

## Power and safety boundaries

- `3V3_ADC` is generated from `LDO_IN` through U203 and R207.
- Carrier VBUS is generated from `LDO_IN` through U204 and reaches U501 pad 10.
- Carrier 3V3 does not power `3V3_ADC`.
- `analog VREF` remains distinct from `ADC_REF`; R206 connects `3V3_ADC` to `ADC_REF`.
- SPI protection is not galvanic or patient isolation.
- Human-connected operation remains battery-only with USB physically absent and no mains-referenced instrumentation.

## Open work

- Correct tracked firmware to CS GPIO14, SCLK GPIO4, MOSI GPIO12, MISO GPIO11.
- Complete unequal-power, rail-ramp, leakage, phantom-power, SPI, power-noise, BAT_MON, and reference-disturbance bench tests.
- Resolve footprint and mechanical work, the two ERC warnings, placement/routing/DRC, EMC, thermal, and fabrication checks.

PCB readiness: **NOT READY**.

Next recommended technical action: synchronize the tracked firmware SPI GPIO definitions with the committed schematic mapping: CS GPIO14, SCLK GPIO4, MOSI GPIO12, and MISO GPIO11.
