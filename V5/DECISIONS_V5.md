# V5 Decisions Log

## 1. Purpose

This file is the central V5 decision log for the EMG project.

It reconstructs the missing `V5/DECISIONS_V5.md` file from the current V5 review documents. It records what is locked, what is provisional, and what must be confirmed before schematic or PCB implementation.

This file is not schematic implementation. This file is not PCB implementation. It does not approve footprint, firmware, gerber, or production output changes by itself.

## 2. Locked Decisions

The following decisions are locked for the current V5 planning direction:

- V4 will be preserved.
- V5 will continue as a separate development line.
- DSTK22807 will be used as a ready-made development board.
- A bare ESP32-H2-MINI module footprint will not be used for the first V5 prototype.
- DSTK22807 will use the measured THT carrier/socket footprint.
- The first prototype will mount the module in a removable header/socket style.
- MCP3208 ADC will be used.
- MCP3208 first prototype package: `MCP3208-CI/P` PDIP-16.
- KiCad MCP3208 symbol: `Analog_ADC:MCP3208`.
- KiCad MCP3208 footprint: `Package_DIP:DIP-16_W7.62mm`.
- MCP3208 TSSOP will not be used unless an exact official MPN and datasheet-backed package/pinout proof are found later.
- `RAW`, `RECT`, and `ENV` analog nodes will be physically produced.
- ADC channel mapping is locked as the V5 planning map:

| MCP3208 Channel | V5 Net |
| --- | --- |
| CH0 | `EMG1_RAW` |
| CH1 | `EMG1_RECT` |
| CH2 | `EMG1_ENV` |
| CH3 | `EMG2_RAW` |
| CH4 | `EMG2_RECT` |
| CH5 | `EMG2_ENV` |
| CH6 | `VREF_MON` |
| CH7 | `BAT_MON` |

- Human electrode safety rule is locked: when electrodes are connected to a human, do not use USB power, 5 V power, bench supply power, mains-connected equipment, or oscilloscope earth connection.

## 3. Provisional Decisions

The following decisions are provisional and must be confirmed before full schematic implementation:

- DSTK22807 Pad 1 = `TX` was observed.
- DSTK22807 observed pinout mapping is recorded as physical observation, not as an official datasheet.
- Current SPI candidate set from `V5/DSTK22807_SPI_PIN_REVIEW.md`:

| SPI Net | ESP32-H2 Candidate | Footprint Pad | Status |
| --- | --- | --- | --- |
| `ADC_CS` | GPIO14 | Pad 13 | Provisional candidate |
| `ADC_SCLK` | GPIO13 | Pad 14 | Provisional candidate |
| `ADC_MOSI` | GPIO12 | Pad 15 | Provisional candidate |
| `ADC_MISO` | GPIO11 | Pad 16 | Provisional candidate |

- MCP3208 `VDD` should preferably be 3.3 V.
- `ADC_REF` is a clean/filtered 3.3 V candidate.
- `analog VREF` is a buffered 1.65 V midscale candidate.
- `analog VCC` is a 3.3 V candidate, but INA/op-amp headroom must be verified.
- `BAT_MON` is planned, but divider/scaling is unresolved.
- `AGND` and `DGND` should be handled with controlled return strategy on a common ground system, not with a blind split.

Note on older photo-derived SPI notes: `V5/DSTK22807_PHOTO_PIN_NOTES.md` contains an earlier photo-derived SPI suggestion using labels `4`, `5`, `10`, and `11`. The newer SPI review uses official ESP32-H2 GPIO capability information and currently supersedes that suggestion for planning. The physical board pinout still needs a second confirmation before any schematic connection.

## 4. Explicit Net Naming Distinctions

These names must stay distinct in schematic review and implementation:

| Net / Name | Meaning | Do not confuse with |
| --- | --- | --- |
| `ADC_REF` | MCP3208 `VREF` pin / ADC full-scale reference. Candidate is clean/filtered 3.3 V. | `analog VREF` |
| `analog VREF` | 1.65 V midscale bias/reference for the analog front-end. Should be buffered. | `ADC_REF` |
| `VREF_MON` | MCP3208 ADC channel that measures `analog VREF`. Planned on CH6. | MCP3208 `VREF` pin |
| `3V3` | Main regulated 3.3 V system rail, source ownership still TBD. | DSTK22807 module-only 3.3 V assumption |
| `3V3_ADC` | Quiet/local ADC supply branch if filtering is used. | A separate isolated ground domain |
| `analog VCC` | Analog front-end supply candidate, currently 3.3 V pending headroom validation. | `analog VREF` |
| `AGND` | Analog return naming/placement intent on the common ground system. | A blindly split ground island |
| `DGND` | Digital return naming/placement intent on the common ground system. | A floating separate ground |

## 5. DSTK22807 Status

- Measured footprint was created.
- The footprint was paper-tested for mechanical planning.
- Silkscreen cleanup was completed.
- Footprint is geometry-approved for mechanical planning.
- Electrical pinout remains provisional.
- USB side still needs physical confirmation.
- Antenna side still needs physical confirmation.
- Right-row reversal still needs confirmation:
  - Top-right footprint pad 10 must be `5V`.
  - Bottom-right footprint pad 18 must be `GPIO9`.
- Footprint pads remain numeric-only; signal labels should not be embedded into the geometry-only footprint.
- Do not connect the DSTK22807 footprint to schematic nets until physical pinout is confirmed again.
- DSTK22807 power pin measurement is documented in `V5/DSTK22807_POWER_PIN_MEASUREMENT.md`.
- USB powered measurement observed 5V = 5.126 V and 3V3 = 3.291 V.
- Unpowered measurement observed 5V = 0 V and 3V3 rail discharge over time: 0.590 V after 10 s, 0.400 V after 30 s, and 0.250 V after 60 s.
- The 3V3 decay is consistent with residual capacitor discharge / leakage path.
- 3V3 external powering remains **NOT APPROVED YET**.
- USB plus external 3.3 V simultaneous connection remains **NOT APPROVED**.
- 5V pin as analog/ADC supply remains **NOT APPROVED**.

Current observed pad mapping from `V5/DSTK22807_PHYSICAL_PINOUT_OBSERVATION.md`:

| Footprint Pad | Observed Label | Status |
| --- | --- | --- |
| 1 | TX | Observed |
| 2 | RX | Observed |
| 3 | GPIO0 | Observed |
| 4 | GPIO1 | Observed |
| 5 | GPIO2 | Observed |
| 6 | GPIO3 | Observed |
| 7 | GPIO4 | Observed |
| 8 | GPIO5 | Observed |
| 9 | GPIO8 | Observed |
| 10 | 5V | Provisional from right-row reversal |
| 11 | GND | Provisional from right-row reversal |
| 12 | 3V3 | Provisional from right-row reversal |
| 13 | GPIO14 | Provisional from right-row reversal |
| 14 | GPIO13 | Provisional from right-row reversal |
| 15 | GPIO12 | Provisional from right-row reversal |
| 16 | GPIO11 | Provisional from right-row reversal |
| 17 | GPIO10 | Provisional from right-row reversal |
| 18 | GPIO9 | Provisional from right-row reversal |

DSTK SPI-only integration decision:

- Current status: **DSTK_SPI_CONNECTED_U501_UNUSED_PINS_MARKED_NOT_FINAL_HW_APPROVAL**.
- Checkpoint `79730dd connect dstk spi and mark unused pins` connected DSTK SPI-only logic and marked intentional U501 unused pins.
- Checkpoint `3494631 sync kicad metadata and analog label placement` synced KiCad sheet metadata and analog reference/value placement.
- Decision: connect MCP3208 SPI to U501 as follows:

| SPI Net | U501 GPIO | U501 Pin |
| --- | --- | --- |
| `ADC_CS` | GPIO14 | Pin 13 |
| `ADC_SCLK` | GPIO13 | Pin 14 |
| `ADC_MOSI` | GPIO12 | Pin 15 |
| `ADC_MISO` | GPIO11 | Pin 16 |

- Reason: lab verification confirmed the right-side pin order with USB up as 5V, GND, 3V3, GPIO14, GPIO13, GPIO12, GPIO11, GPIO10, GPIO9.
- Reason: measured USB-powered voltages were 3V3 = 3.295 V and 5V = 5.125 V.
- Reason: same-pin top pad to side castellated edge continuity was confirmed.
- This verification is sufficient for SPI pin mapping.
- This verification is not approval to power `3V3_ADC` from DSTK 3V3.
- Decision: connect U501 GND / pin 11 to root/system GND.
- Reason: SPI logic requires a common reference.
- Boundary: this is not DSTK power-source approval.
- Boundary: DSTK 3V3 and 5V/VBUS remain unconnected to analog/ADC rails.
- Decision: use explicit KiCad no_connect markers for intentionally unused U501 pins.
- no_connect pins: TX, RX, GPIO0, GPIO1, GPIO2, GPIO3, GPIO4, GPIO5, GPIO8, 5V_VBUS, 3V3, GPIO10, and GPIO9.
- Reason: these pins are intentionally unused in this schematic checkpoint, and no_connect documents intentional non-use.
- Boundary: No ERC markers were used.
- Boundary: no_connect is not placed on GND or SPI-connected pins.
- Boundary: 3V3 and 5V_VBUS no_connect means they are intentionally not used in this power strategy checkpoint; it does not approve any power architecture.
- Decision: keep `DSTK_3V3_CANDIDATE` unresolved until a dedicated power-source strategy review.
- Reason: DSTK 3V3 presence was measured, but USB/backfeed/load/noise and `3V3_ADC` source strategy are not approved.
- Boundary: do not connect DSTK 3V3 to `3V3_ADC` until a separate power review approves it.
- Boundary: do not connect DSTK 5V/VBUS to any analog/ADC rail.
- Boundary: no 5V analog/ADC.
- Current root ERC decision after DSTK SPI/U501 cleanup: acceptable for pre-power-strategy continuation with 0 errors and 2 known warnings.
- Known warnings: U501 footprint library warning from CLI config, and `DSTK_3V3_CANDIDATE` isolated pin label.
- This is not final hardware approval.
- PCB has not been started.

DSTK power-source strategy placeholder decision:

- Current status: **DSTK_POWER_PLAN_KEEP_PLACEHOLDER_DOCS_ONLY**.
- Decision: keep `DSTK_3V3_CANDIDATE` as a docs-tracked planning placeholder for now.
- Decision: do not connect U501 3V3 to `3V3_ADC` at this checkpoint.
- Decision: do not connect U501 5V_VBUS to analog/ADC rails.
- Decision: do not suppress the `DSTK_3V3_CANDIDATE` warning with a No ERC marker.
- Reason: U501 3V3 voltage was measured as 3.295 V under USB power, but this does not prove safe use as an analog/ADC rail source.
- Reason: U501 5V was measured as 5.125 V under USB power, but 5V analog/ADC is no-go.
- Reason: current capacity, BLE/RF load effect, regulator noise, and USB/external-source backfeed behavior are not confirmed.
- Reason: human-contact EMG testing must remain battery-isolated.
- Option review: DSTK 3V3 -> `3V3_ADC` is not approved now and requires regulator/current/noise/backfeed confirmation.
- Option review: local/separate `3V3_ADC` regulator is the safer controlled candidate and needs a separate plan.
- Option review: selectable/jumper source remains a possible later approach, but only one source may be populated or closed.
- Option review: keeping the placeholder is the approved current checkpoint decision.
- Measurement needs before changing this decision:
  - identify the DSTK onboard regulator or power-path model,
  - dummy-load test at 5 mA, 10 mA, 20 mA, 30 mA, and optionally 50 mA,
  - observe 3V3 droop/noise with BLE/RF active,
  - evaluate USB/external 3.3 V backfeed behavior,
  - define a USB-disconnected battery-only human-test procedure.
- Boundary: no schematic power edit is approved by this decision.
- Boundary: PCB has not been started.
- Boundary: this is not final hardware approval.

Local/separate 3V3_ADC regulator pre-schematic decision:

- Current status: **LOCAL_3V3_ADC_REGULATOR_PRE_SCHEMATIC_PLAN_REVIEWED_NOT_IMPLEMENTED**.
- Decision: plan a local/separate battery-side `3V3_ADC` regulator path before PCB work.
- Decision: do not approve DSTK 3V3 as the analog/ADC rail source at this checkpoint.
- Decision: do not approve DSTK 5V/VBUS for analog/ADC rails.
- Decision: keep `DSTK_3V3_CANDIDATE` as a planning placeholder only.
- Decision: keep `ADC_REF` and `analog VREF` separate.
- Decision: keep human-contact testing battery-isolated only.
- Reason: DSTK 3V3 voltage presence alone does not prove current capacity, low noise, BLE/RF load behavior, or safe backfeed behavior.
- Reason: EMG/ADC measurements are sensitive to supply ripple, reference ripple, and analog bias noise.
- Reason: for a 12-bit ADC at 3.3 V, 1 LSB is approximately 0.8 mV, so rail/reference noise must be evaluated at that scale. This is a review scale, not a final spec.
- Reason: `ADC_REF` ripple affects ADC full-scale behavior directly.
- Reason: `analog VREF` noise affects RAW/RECT/ENV bias behavior.
- Reason: `BAT_MON` divider must be checked against battery maximum voltage and ADC input/source impedance constraints.
- Battery/topology note: 1S Li-ion/LiPo LDO-only cannot guarantee 3.3 V regulation across the full discharge range.
- Battery/topology note: 1S Li-ion/LiPo buck-only also cannot regulate once battery voltage falls near/below 3.3 V.
- Battery/topology note: if 1S Li-ion/LiPo is selected, evaluate buck-boost or buck-boost plus low-noise LDO.
- Battery/topology note: 2xAA may be below 3.3 V depending on chemistry/state of discharge, so boost or buck-boost may be required.
- Battery/topology note: 3xAA may allow an LDO depending on chemistry and dropout, but maximum voltage, power loss, and dropout must be checked.
- Battery/topology note: buck-only is efficient but has higher switching-noise risk, so it is less attractive as the only first analog validation source.
- Battery/topology note: buck plus low-noise LDO can improve noise but increases BOM/layout complexity.
- Battery/topology note: final topology cannot be selected before battery chemistry and load current are defined.
- Regulator selection criteria: input voltage range, 3.3 V output, output current margin, dropout or buck-boost operating range, quiescent current, PSRR, output noise, reverse current/backfeed behavior, enable pin need, startup/soft-start behavior, stable output capacitor range and ESR, package and hand-solderability, availability, and datasheet reference design requirements.
- Source selection policy: local regulator output must not be tied directly to DSTK 3V3.
- Source selection policy: `DSTK_3V3_CANDIDATE` and local regulator output must not be shorted together.
- Source selection policy: if jumper/0R selectable source is later used, population policy must allow only one source path at a time.
- Source selection policy: do not implement source selection in this docs-only checkpoint.
- Source selection policy: prefer clearer future source naming such as `LOCAL_3V3_ADC_SOURCE` or `LOCAL_3V3_ADC_REG`, but do not rename schematic nets in this task.
- Measurement checklist before schematic implementation: battery chemistry, battery min/nom/max voltage, estimated analog/ADC load current, `3V3_ADC` no-load voltage, `3V3_ADC` loaded voltage, `3V3_ADC` ripple/noise, `ADC_REF` DC level and ripple/noise, `analog VREF` DC level and ripple/noise, MCP3208 VDD/VREF relationship, `BAT_MON` divider output at maximum battery voltage, `BAT_MON` source impedance / ADC sampling effect, BLE idle/active comparison, SPI inactive/active comparison, USB connected/disconnected backfeed check, power-up/power-down sequencing, reverse current path check, and thermal check under expected load.
- Human safety boundary: USB-powered human EMG testing is not approved.
- Human safety boundary: USB must be disconnected when electrodes are attached to a human.
- Human safety boundary: battery-powered operation is required for human-contact testing, but battery-powered alone is not final safety approval.
- Human safety boundary: `REF_ELECTRODE` nets must not be connected to GND/chassis/USB.
- Human safety boundary: 5V analog/ADC is not approved.
- Human safety boundary: this is a student/prototype sensor development record, not medical device approval.
- Human safety boundary: PCB/enclosure/isolation safety must be reviewed separately before any final human test approval.

Battery decision for first analog validation:

- Current status: **BATTERY_DECISION_FIRST_ANALOG_VALIDATION_3XAA_NOT_FINAL_PRODUCT**.
- Decision: use 3xAA alkaline battery pack as the first analog validation / bring-up battery candidate.
- Decision: do not treat 3xAA as the final product battery decision.
- Decision: keep 1S protected Li-ion/LiPo as the later final product battery candidate.
- Decision: defer Li-ion/LiPo implementation until analog chain behavior, regulator topology, `3V3_ADC` current budget, charger/backfeed behavior, and human-contact safety procedure are reviewed.
- Decision: do not approve any schematic battery/regulator implementation in this checkpoint.
- Decision: do not approve DSTK 3V3 as the `3V3_ADC` source.
- Decision: do not approve DSTK 5V/VBUS for analog/ADC rails.
- Reason: 3xAA alkaline gives a simple battery-isolated source for first analog validation.
- Reason: 3xAA avoids USB charger/load-sharing/backfeed variables during early analog bring-up.
- Reason: 3xAA allows low-noise 3.3 V LDO evaluation without immediately introducing boost/buck-boost switching noise.
- Reason: 3xAA is easier to debug and measure during first bench validation.
- Reason: 1S Li-ion/LiPo is better for final product form factor and rechargeability, but it needs buck-boost or buck-boost plus low-noise LDO review for stable 3.3 V operation across the discharge range.
- Reason: Li-ion/LiPo also requires charger/protection/load-sharing and human-contact safety policy review.
- Constraint: 3xAA path still requires regulator selection before schematic implementation.
- Constraint: battery chemistry for final product remains unlocked.
- Constraint: regulator MPN/package/footprint remains unlocked.
- Constraint: MCP3208 VDD remains planned from selected `3V3_ADC`.
- Constraint: MCP3208 VREF remains `ADC_REF` and must stay within VDD limits.
- Constraint: `ADC_REF` and `analog VREF` must remain separate.
- Constraint: `REF_ELECTRODE` nets must not be connected to GND/chassis/USB.
- Constraint: USB-powered human EMG testing is not approved.
- Constraint: USB must be disconnected when electrodes are attached to a human.
- Constraint: battery-powered operation is required for human-contact testing, but battery-powered alone is not final safety approval.
- Constraint: PCB/enclosure/isolation safety must be reviewed separately before any final human test approval.
- Future architecture note: first analog validation candidate is 3xAA alkaline -> switch/protection -> low-noise 3.3 V regulator -> `3V3_ADC`.
- Future architecture note: later final product candidate is 1S protected Li-ion/LiPo -> charger/protection policy -> buck-boost or buck-boost plus low-noise LDO -> `3V3_ADC`.
- Boundary: do not implement either architecture in schematic in this docs-only checkpoint.

3xAA low-noise 3V3_ADC LDO requirements decision:

- Current status: **3XAA_LOW_NOISE_3V3_ADC_LDO_REQUIREMENTS_REVIEWED_NOT_IMPLEMENTED**.
- Decision: use 3xAA alkaline -> switch/protection -> low-noise 3.3 V LDO -> `3V3_ADC` as the first analog validation path candidate.
- Decision: treat this as alkaline-specific first validation planning, not as final product battery selection.
- Decision: do not include 3xAA NiMH in this decision.
- Decision: keep 1S protected Li-ion/LiPo deferred as the later final product battery candidate.
- Decision: do not approve any schematic implementation in this checkpoint.
- Decision: do not select regulator MPN/package/footprint in this checkpoint.
- Decision: do not approve DSTK 3V3 or 5V/VBUS as analog/ADC rail sources.
- Reason: 3xAA alkaline gives a simple battery-isolated source for early analog bring-up.
- Reason: it avoids charger/load-sharing/USB backfeed variables.
- Reason: it avoids boost/buck-boost switching noise during first analog validation.
- Reason: it allows low-noise LDO evaluation before introducing switching regulators.
- Reason: 3xAA fresh/open-circuit can be around 4.5 V to 4.8 V, so LDO input rating, `BAT_MON` maximum voltage, and power dissipation must be checked with margin.
- Reason: LDO regulation will be lost when pack voltage under load falls below 3.3 V plus selected LDO dropout.
- Reason: for 3xAA NiMH, nominal pack voltage is about 3.6 V, so dropout margin is much narrower and this must not be assumed equivalent to alkaline.
- Constraint: minimum usable battery voltage must be measured with actual load.
- Constraint: load current must be estimated from datasheets and later measured.
- Constraint: LDO selection must check dropout, PSRR, output noise, output current margin, quiescent current, reverse current/backfeed behavior, capacitor stability, thermal behavior, package/hand-solderability, and datasheet reference design.
- Constraint: `3V3_ADC` ripple/noise must be measured.
- Constraint: `ADC_REF` DC level and ripple/noise must be measured.
- Constraint: `analog VREF` DC level and ripple/noise must be measured.
- Constraint: MCP3208 VDD/VREF relationship must be verified.
- Constraint: `BAT_MON` divider output at maximum 3xAA voltage and source impedance / ADC sampling effect must be checked.
- Constraint: BLE idle/active comparison is a measurement item only; it does not approve human-electrode testing with USB or unresolved DSTK power.
- Constraint: human-contact testing remains battery-isolated only.
- Constraint: USB must be disconnected when electrodes are attached to a human.
- Constraint: battery-powered operation alone is not final safety approval.
- Constraint: PCB/enclosure/isolation safety must be reviewed separately before final human test approval.
- Pre-schematic implementation gate: confirm 3xAA alkaline-only scope.
- Pre-schematic implementation gate: define LDO current budget target.
- Pre-schematic implementation gate: define LDO dropout target.
- Pre-schematic implementation gate: define LDO noise/PSRR target.
- Pre-schematic implementation gate: define protection strategy.
- Pre-schematic implementation gate: define switch strategy.
- Pre-schematic implementation gate: define `BAT_MON` divider ratio.
- Pre-schematic implementation gate: define test points `BAT_PACK`, `3V3_ADC`, `ADC_REF`, `analog VREF`, and GND.
- Pre-schematic implementation gate: define `ADC_REF` generation/decoupling strategy.
- Pre-schematic implementation gate: confirm `analog VREF` buffer loading.
- Pre-schematic implementation gate: select exact regulator MPN/package/footprint.
- Pre-schematic implementation gate: define no-USB human-contact procedure.

3xAA LDO current/dropout target decision:

- Current status: **3XAA_LDO_CURRENT_DROPOUT_TARGETS_REVIEWED_NOT_IMPLEMENTED**.
- Decision: use preliminary first-validation LDO output current capability target of at least 50 mA for `3V3_ADC`, pending datasheet lookup and measured analog/ADC load.
- Decision: allow 100 mA or higher regulator class only if noise, dropout, quiescent current, package thermal behavior, reverse-current behavior, and capacitor stability remain suitable.
- Decision: do not treat output current rating alone as a selection criterion.
- Decision: use preliminary dropout target of <=300 mV at the chosen current budget target.
- Decision: prefer <=200 mV dropout at the expected measured analog/ADC load.
- Decision: use preliminary LDO Vin max target of >=5.5 V preferred minimum and >=6.0 V more comfortable.
- Decision: do not select regulator MPN/package/footprint in this checkpoint.
- Decision: do not approve schematic implementation in this checkpoint.
- Decision: do not approve DSTK 3V3 or 5V/VBUS as analog/ADC rail sources.
- Reason: the analog/ADC load is expected to be modest, but exact current must come from datasheet lookup and bench measurement.
- Reason: a 50 mA class target gives a practical first-validation margin without blindly oversizing.
- Reason: 100 mA or higher may be easy to source, but oversizing can affect package, quiescent current, noise, dropout, and stability tradeoffs.
- Reason: lower dropout increases usable 3xAA alkaline battery range.
- Reason: fresh 3xAA alkaline pack voltage may be around 4.5 V to 4.8 V, so LDO input rating and `BAT_MON` maximum-voltage review need margin.
- Reason: 3xAA NiMH is not equivalent because nominal pack voltage is about 3.6 V and dropout margin is much narrower.
- Reason: `3V3_ADC` ripple/noise affects ADC readings.
- Reason: `ADC_REF` ripple affects ADC full-scale behavior directly.
- Reason: `analog VREF` noise affects RAW/RECT/ENV bias behavior.
- Current budget framework: U201 MCP3208 requires datasheet lookup, bench measurement, and margin.
- Current budget framework: U202 MCP6002 analog VREF buffer and safe unused follower require datasheet lookup and bench measurement.
- Current budget framework: U302 MCP6004 Channel 1 analog/RECT/ENV requires datasheet lookup and bench measurement.
- Current budget framework: U402 MCP6004 Channel 2 analog/RECT/ENV requires datasheet lookup and bench measurement.
- Current budget framework: U301 INA333 Channel 1 requires datasheet lookup.
- Current budget framework: U401 INA333 Channel 2 requires datasheet lookup.
- Current budget framework: analog VREF divider/buffer loading requires estimate and bench measurement.
- Current budget framework: `ADC_REF` feed/decoupling loading requires datasheet and bench review.
- Current budget framework: `BAT_MON` divider loading and ADC source impedance require review.
- Current budget framework: `VREF_MON` sampling path requires review.
- Current budget framework: test points, leakage, and margin must be included in the current budget margin.
- Schematic implementation gate: LDO current target selected.
- Schematic implementation gate: LDO dropout target selected.
- Schematic implementation gate: LDO Vin max target selected.
- Schematic implementation gate: LDO noise/PSRR target language accepted.
- Schematic implementation gate: protection strategy selected.
- Schematic implementation gate: switch strategy selected.
- Schematic implementation gate: `BAT_MON` divider maximum voltage and ratio selected.
- Schematic implementation gate: test points confirmed: `BAT_PACK`, `3V3_ADC`, `ADC_REF`, `analog VREF`, and GND.
- Schematic implementation gate: `ADC_REF` generation/decoupling strategy selected.
- Schematic implementation gate: analog VREF buffer loading checked.
- Schematic implementation gate: exact regulator MPN/package/footprint selected later.
- Schematic implementation gate: no-USB human-contact procedure documented.
- Constraint: 3xAA alkaline remains first analog validation only.
- Constraint: 3xAA is not final product battery selection.
- Constraint: 1S protected Li-ion/LiPo remains deferred final product battery candidate.
- Constraint: `ADC_REF` and `analog VREF` remain separate.
- Constraint: human-contact testing remains battery-isolated only.
- Constraint: USB must be disconnected when electrodes are attached to a human.
- Constraint: battery-powered operation alone is not final safety approval.
- Constraint: PCB/enclosure/isolation safety must be reviewed separately before final human test approval.

3V3_ADC current-budget and TPS7A2033 preferred-candidate decision:

- Current status: **3V3_ADC_CURRENT_BUDGET_SUPPORTS_TPS7A2033_PREFERRED_WITH_BACKFEED_CAVEAT_NOT_FINAL_HW_APPROVAL**.
- Decision: keep TI `TPS7A2033PDBVR` as the preferred LDO candidate for the first-validation `3V3_ADC` path.
- Decision: record the current first-validation architecture direction as 3xAA alkaline -> switch/protection -> `TPS7A2033PDBVR` candidate -> `3V3_ADC`.
- Decision: treat `TPS7A2033PDBVR` as a preferred candidate only, not final hardware approval, not schematic implementation approval, not final production MPN lock, not footprint lock, not BOM lock, and not PCB approval.
- Decision: do not reopen LDO selection from this review because the reviewed datasheet-based current budget does not contradict the previous preference for `TPS7A2033PDBVR`.
- Decision: keep the 10 mA practical first-validation design budget as a design budget, not as expected measured current.
- Decision: keep the at-least-50 mA regulator-capability planning floor as a conservative capability floor, not as expected operating load.
- Decision: actual `3V3_ADC` rail current still requires bench measurement before final hardware approval.
- Reviewed component count: 1x U201 MCP3208.
- Reviewed component count: 1x U202 MCP6002 package = 2 op-amp channels.
- Reviewed component count: 2x MCP6004 packages, U302 and U402 = 8 op-amp channels total.
- Reviewed component count: 2x INA333 devices, U301 and U401.
- Datasheet-backed MCP3208 supply current:
  - Typical supply current: 320 uA.
  - Maximum supply current: 400 uA.
  - Reviewed condition: `VDD = VREF = 5 V`, `TA = -40 C to +85 C`, `fSAMPLE = 100 ksps`, DOUT unloaded.
  - Exact MCP3208 current at the project 3.3 V operating point is not confirmed as a separate guaranteed datasheet table value.
- Datasheet-backed MCP3208 reference input drain:
  - Typical reference input drain: 100 uA.
  - Maximum reference input drain: 150 uA.
  - Reviewed at 5 V.
  - This current is included because `ADC_REF` is fed from `3V3_ADC` through the current R206 0R/10R candidate direction.
- Datasheet-backed MCP6002/MCP6004 current basis:
  - Quiescent current is specified per amplifier, not per package.
  - Typical quiescent current: 100 uA per amplifier.
  - Maximum quiescent current under the reviewed datasheet conditions: 170 uA per amplifier.
  - The project uses both amplifiers in one MCP6002 package.
  - The project uses two MCP6004 packages, four amplifiers per package, eight amplifiers total.
- Datasheet-backed INA333 current:
  - Typical supply current: 50 uA per device.
  - Maximum supply current: 75 uA at 25 C under the reviewed conditions.
  - Maximum supply current: 80 uA over the wider reviewed temperature range.
  - The project uses two INA333 devices.
- Calculated analog VREF divider current:
  - R201 = 47k.
  - R202 = 47k.
  - Current is approximately 35.1 uA at 3.3 V.
- Calculated BAT_MON divider current:
  - R204 = 330k.
  - R205 = 1M.
  - Current is approximately 3.61 uA at 4.8 V battery input.
  - This is battery-input drain, not direct `3V3_ADC` rail load.
- `VREF_MON` static load is expected to be very small, but exact dynamic sampling disturbance remains unconfirmed.
- Reviewed current-budget result:
  - Typical expected `3V3_ADC` load: approximately 1.6 mA.
  - Conservative verified static/active subtotal: approximately 2.5 mA.
  - Practical first-validation design budget: 10 mA.
  - Existing regulator-capability planning floor: at least 50 mA.
- The approximately 1.6 mA and approximately 2.5 mA totals are datasheet-based engineering estimates, not measured values.
- Reason: `TPS7A2033PDBVR` remains preferred because the expected analog/ADC load is far below its 300 mA output capability.
- Reason: `TPS7A2033PDBVR` has a suitable project-specific balance of 1.6 V to 6.0 V recommended input range, 6.5 V absolute maximum input, low output noise, strong PSRR, low quiescent current, low dropout, and SOT-23-5 / DBV package availability.
- Constraint: the 300 mA output capability is not the expected operating current and is not the only reason the part remains preferred.
- Material caveat: reverse-current/backfeed behavior remains open.
- TI documentation warns that reverse current can flow from output to input after input collapse and may damage the regulator.
- Constraint: no other source may intentionally hold `3V3_ADC` high while the TPS7A20 input is collapsed, unless a future reviewed protection topology explicitly makes this safe.
- Constraint: no DSTK 3V3 -> `3V3_ADC`.
- Constraint: no DSTK 5V/VBUS -> analog or ADC rail.
- Constraint: no local LDO output tied directly to DSTK 3V3.
- Constraint: `ADC_REF` and `analog VREF` remain separate.
- Constraint: `analog VREF` remains approximately 1.65 V analog midscale bias/reference.
- Constraint: `ADC_REF` remains the MCP3208 full-scale reference.
- Constraint: `REF_ELECTRODE` nets must not be tied to GND, chassis, or USB.
- Constraint: human-contact EMG testing remains battery-isolated only.
- Constraint: USB must be disconnected whenever electrodes are attached to a human.
- Thermal sanity result at `Vin = 4.8 V`, `Vout = 3.3 V`, using `P_LDO = (Vin - Vout) * Iload`:
  - 10 mA -> approximately 15 mW.
  - 50 mA -> approximately 75 mW.
- Constraint: these thermal values are first-pass dissipation estimates only; actual junction behavior depends on PCB copper, layout, ambient temperature, package mounting, and datasheet thermal test conditions.
- Constraint: the full-load 300 mA dropout number must not be treated as the expected project operating point.
- Constraint: at the actual low expected load, dropout should be lower than the full-load condition, but no guaranteed low-current dropout value is invented here without an explicit reviewed datasheet table value.
- Schematic implementation status: not implemented.
- PCB status: not started.

PMOS / BAT_MON / SPI off-power pre-implementation gate decision:

- Current status: **3XAA_TPS7A2033_PMOS_BATMON_SPI_OFFPOWER_REVIEWED_READY_WITH_CAVEATS_NO_SCHEMATIC_EDIT_NO_PCB_NOT_FINAL_HW_APPROVAL**.
- Decision marker: `3XAA_TPS7A2033_PMOS_BATMON_SPI_OFFPOWER_REVIEWED_READY_WITH_CAVEATS_NO_SCHEMATIC_EDIT_NO_PCB_NOT_FINAL_HW_APPROVAL`.
- Decision scope: 3xAA alkaline first analog validation only.
- Decision scope: no schematic implementation in this checkpoint.
- Decision scope: no PCB work in this checkpoint.
- Decision scope: not final hardware approval.
- Decision scope: not human-test approval.
- Decision: preserve the reviewed first-validation power architecture:
  - 3xAA alkaline.
  - Main high-side switch.
  - PMOS reverse-polarity protection.
  - Protected `LDO_IN`.
  - BAT_MON divider/filter from protected `LDO_IN`.
  - `TPS7A2033PDBVR` IN from protected `LDO_IN`.
  - `TPS7A2033PDBVR` OUT through a 0R/current-measure link to `3V3_ADC`.
- Decision: keep TI `TPS7A2033PDBVR` as the preferred LDO candidate.
- Decision: do not reopen TPS7A2033 selection from this checkpoint.
- Preserved current-budget basis:
  - Typical expected `3V3_ADC` load: approximately 1.6 mA.
  - Conservative verified static/active subtotal: approximately 2.5 mA.
  - Practical first-validation design budget: 10 mA.
  - Regulator-capability planning floor: at least 50 mA.
  - These are datasheet-based engineering estimates, not measured current.
- Material caveat: TPS7A2033 reverse-current/backfeed behavior remains a constraint.
- Constraint: no other source may intentionally hold `3V3_ADC` high while the TPS7A20 input is collapsed unless a future reviewed protection topology explicitly makes this safe.
- PMOS decision status: **PMV48XP_SELECTED_AS_PREFERRED_EXACT_FIRST_VALIDATION_CANDIDATE**.
- Decision: use Nexperia `PMV48XP` as the preferred exact first-validation PMOS candidate.
- Reviewed PMV48XP properties:
  - Type: P-channel MOSFET.
  - Package: SOT-23 / TO-236AB.
  - `VDS`: -20 V.
  - `VGS` absolute maximum: +/-12 V.
  - `RDS(on)` maximum: 55 mOhm at `VGS = -4.5 V`.
  - `RDS(on)` maximum: 81 mOhm at `VGS = -2.5 V`.
  - Pin 1 = Gate.
  - Pin 2 = Source.
  - Pin 3 = Drain.
- Decision: connect the PMV48XP for first-validation planning as:
  - Switched battery positive -> Drain / physical pin 3.
  - Protected `LDO_IN` -> Source / physical pin 2.
  - Gate -> system GND / physical pin 1.
- Reviewed PMOS operation:
  - Under correct pack polarity, initial conduction occurs through the intrinsic body diode from drain toward source.
  - The source node rises.
  - `VGS` becomes negative.
  - The PMOS enhances and provides a low-drop conduction path.
  - Under reversed pack polarity, the body diode is reverse-biased and the PMOS remains off.
- PMOS caveat: pack-level PMOS reverse-polarity protection does not by itself guarantee protection against one individual AA cell being inserted backwards while the total series-pack output polarity remains positive.
- PMOS caveat: PMV48XP is not final production MPN lock.
- PMOS caveat: PMV48XP lifecycle/prototype availability was not confirmed from reviewed primary manufacturer documentation.
- PMOS implementation caveat: KiCad symbol pin mapping and physical package pin mapping must be verified before implementation.
- PMOS implementation caveat: no footprint is assigned or locked by this checkpoint.
- BAT_MON decision: old 330k top / 1M bottom candidate is not suitable for the current 3xAA first-validation direction.
- BAT_MON reason: at 4.8 V, the old divider produces approximately 3.61 V, which exceeds a 3.3 V `ADC_REF` normal operating range.
- BAT_MON rule: absolute maximum ADC input limits must not be used as normal operating targets.
- BAT_MON decision: lock the following values for schematic implementation planning:
  - Top resistor: 68k.
  - Bottom resistor: 100k.
  - Capacitor: 10 nF from BAT_MON to GND.
  - Sense node: protected `LDO_IN`.
- BAT_MON reviewed calculations:
  - Divider ratio: approximately 0.595.
  - BAT_MON at 4.5 V: approximately 2.679 V.
  - BAT_MON at 4.8 V: approximately 2.857 V.
  - BAT_MON at 5.5 V review point: approximately 3.274 V.
  - Divider current at 4.8 V: approximately 28.6 uA.
  - Thevenin resistance: approximately 40.5 kOhm.
  - RC time constant with 10 nF: approximately 0.405 ms.
  - Five time constants: approximately 2.0 ms.
- BAT_MON firmware policy:
  - BAT_MON is a slow housekeeping measurement.
  - After switching to MCP3208 CH7, discard the first conversion.
  - Wait at least 5 ms.
  - Acquire several samples.
  - Average the samples.
- BAT_MON caveat: actual BAT_MON accuracy and settling still require bench measurement.
- BAT_MON caveat: exact resistor and capacitor MPNs are not locked.
- SPI off-power reviewed problematic condition:
  - DSTK22807 / ESP32-H2 powered.
  - `3V3_ADC` off.
  - MCP3208 VDD at 0 V or collapsing.
  - SPI remains physically connected through `ADC_CS`, `ADC_SCLK`, `ADC_MOSI`, and `ADC_MISO`.
- MCP3208 datasheet-backed absolute maximum limit for all inputs/outputs: `VSS - 0.6 V` to `VDD + 0.6 V`.
- SPI off-power conclusion: applying approximately 3.3 V to MCP3208 digital inputs while VDD = 0 V is not an approved operating condition.
- SPI off-power caveat: internal clamp/injection or partial-powering behavior may occur, but exact behavior is not approved as a normal operating mode.
- SPI off-power caveat: small series resistors alone do not make the powered-DSTK / unpowered-MCP3208 condition safe or datasheet-approved.
- First-validation hard rule: USB-powered DSTK + `3V3_ADC` off + SPI physically connected is FORBIDDEN for first validation.
- Firmware caveat: firmware-only high-Z policy is not sufficient from the instant of power-up because firmware cannot guarantee GPIO state before reset/boot completes.
- DSTK caveat: actual DSTK22807 carrier pull-up/pull-down circuitry on GPIO11, GPIO12, GPIO13, and GPIO14 remains unconfirmed from current project evidence.
- SPI series-resistor direction:
  - Useful candidate purpose: edge-rate damping.
  - Useful candidate purpose: ringing reduction.
  - Useful candidate purpose: noise control.
  - Useful candidate purpose: partial fault-current reduction.
  - Candidate range: 100R to 330R.
  - Reasonable first schematic candidate if one common value is desired: 220R.
  - Exact value is not locked by this documentation-sync checkpoint.
  - SPI series resistors are not the off-power safety solution.
- Decision: the main remaining uncontrolled backfeed/off-power risk after the PMOS and BAT_MON reviews is through the SPI interface, not through the selected PMOS or BAT_MON divider.
- Preserved constraints:
  - No DSTK 3V3 -> `3V3_ADC`.
  - No DSTK 5V/VBUS -> analog or ADC rails.
  - No local LDO output directly tied to DSTK 3V3.
  - No other source may intentionally hold `3V3_ADC` high while the TPS7A20 input is collapsed.
  - `ADC_REF` and `analog VREF` remain separate.
  - `analog VREF` remains approximately 1.65 V analog midscale bias/reference.
  - `ADC_REF` remains the MCP3208 full-scale reference.
  - `EMG1_REF_ELECTRODE` and `EMG2_REF_ELECTRODE` must not be tied to GND, chassis, or USB.
  - Human-contact EMG testing remains battery-isolated only.
  - USB must be disconnected whenever electrodes are attached to a human.
  - Battery-powered operation alone is not final human-test approval.
  - No No ERC markers.
  - PCB has not started.

## 6. MCP3208 Status

- `MCP3208-CI/P` PDIP-16 is recommended for the socketed first prototype.
- `MCP3208-CI/SL` SOIC-16 is a valid compact alternative after DIP bring-up.
- KiCad symbol `Analog_ADC:MCP3208` was reviewed against the official MCP3208 pinout.
- KiCad DIP footprint `Package_DIP:DIP-16_W7.62mm` was reviewed for the PDIP first prototype.
- MCP3208 should preferably be powered at 3.3 V for direct ESP32-H2 SPI compatibility.
- If MCP3208 is powered at 5 V, `DOUT` level shifting is required before ESP32-H2.
- MCP3208 `VREF` is the ADC full-scale reference and must not be merged with the 1.65 V analog midscale bias without an explicit architecture decision.
- MCP3208-CI/P is the C-grade accuracy version. It is acceptable for first EMG prototype bring-up, but B-grade alternatives such as `MCP3208-BI/P` or `MCP3208-BI/SL` can be considered if accuracy margin becomes critical.

## 7. Power / Reference Status

- Battery-powered first prototype is preferred.
- First schematic power-source strategy documented in `V5/FIRST_SCHEMATIC_POWER_SOURCE_STRATEGY.md`.
- First schematic power-source strategy is **SELECTABLE / JUMPER SOURCE OPTION**.
- Limited power/reference schematic block added in commit `1f759a6`.
- Root schematic `V5/EMG_v5.kicad_sch` now includes hierarchical sheet `POWER_REFERENCE_BLOCK`.
- Child schematic file added: `V5/POWER_REFERENCE_BLOCK.kicad_sch`.
- `POWER_REFERENCE_BLOCK` now receives `EMG1_RAW`, `EMG1_RECT`, `EMG1_ENV`, `EMG2_RAW`, `EMG2_RECT`, and `EMG2_ENV` as hierarchical inputs from the Channel 1 and Channel 2 analog sheets.
- Project metadata updated in `V5/EMG_v5.kicad_pro`.
- This is the first limited KiCad schematic edit for power/reference support.
- This schematic block is review/prototype planning support, not final production schematic.
- This is not full board schematic completion.
- This is not PCB layout.
- This is not analog EMG chain schematic.
- Candidate source A: `DSTK_3V3_CANDIDATE`.
- Candidate source B: `LDO_3V3_FALLBACK`.
- Selected analog/ADC rail: `3V3_ADC`.
- Power-source strategy uses selectable / jumper source option.
- `DSTK_3V3_CANDIDATE` and `LDO_3V3_FALLBACK` must be mutually exclusive.
- Do not populate/close both source paths at the same time.
- Do not directly tie the two source outputs together.
- Do not backfeed DSTK `3V3`.
- Do not power DSTK22807 from the external LDO in the first schematic block.
- Do not connect the DSTK22807 power path to the external LDO in this block.
- Do not use `5V` as analog/ADC supply.
- External analog/ADC 3.3 V regulator is now a conservative fallback candidate, not a mandatory preferred requirement.
- DSTK22807 onboard `3V3` rail is a new candidate source for analog/ADC power, pending current budget and load/noise test.
- External LDO remains a fallback if DSTK `3V3` current, noise, thermal margin, or ADC stability is unacceptable.
- DSTK22807 3V3 rail current budget review documented in `V5/DSTK22807_3V3_RAIL_CURRENT_BUDGET_REVIEW.md`.
- DSTK22807 onboard regulator / power path exact model remains UNKNOWN.
- DSTK22807 onboard 3V3 current capability remains unverified.
- DSTK22807 3V3 power direction remains unresolved.
- Do not assume DSTK22807 `3V3` pin can power the whole analog/ADC system until current and noise testing pass.
- Do not back-feed DSTK22807 until board behavior is verified.
- Do not back-feed DSTK22807 3V3 until power path is verified.
- For human-connected EMG testing, USB Type-C must be disconnected and the system must be battery-powered/isolated.
- MCP3208 `VDD` should be powered from selected `3V3_ADC`.
- MCP3208 `VREF` remains `ADC_REF`.
- `ADC_REF` and `analog VREF` are separate.
- MCP3208 powered at 3.3 V is preferred.
- If MCP3208 is powered at 5 V later, `DOUT` level shifting is required.
- If DSTK `3V3` is used for MCP3208 `VDD` / `ADC_REF`, local decoupling/filtering and ADC_REF quality review remain required.
- `analog VREF` candidate remains 47k/47k divider plus buffer.
- `VREF_MON` and `BAT_MON` are represented in the child schematic.
- ADC CH0-CH5 are represented by Channel 1 and Channel 2 analog outputs; SPI inputs are now connected to DSTK22807 U501 through the root schematic.
- SPI labels `ADC_CS`, `ADC_SCLK`, `ADC_MOSI`, and `ADC_MISO` are exposed from `POWER_REFERENCE_BLOCK` to root as hierarchical labels/sheet pins.
- Safety notes are present in the child schematic.
- `BAT_MON` is represented, but final divider/scaling remains review item.
- ADC input protection is unresolved.
- Analog headroom/gain at 3.3 V is unresolved.
- ADC input range protection must ensure CH0-CH7 never exceed VSS to VREF in single-ended MCP3208 use.
- Proper MCP3208 `VDD` decoupling, `VREF` decoupling, and AGND/DGND handling are required in the later schematic/layout.
- Power/reference value-level design review documented in `V5/POWER_REFERENCE_VALUE_LEVEL_DESIGN_REVIEW.md`.
- Value-level candidates exist for regulator, analog VREF divider/buffer, ADC_REF decoupling, ADC input protection, BAT_MON divider, unused ADC bias, and test points.
- These candidates are not final schematic implementation and not final BOM lock.
- Small power/reference schematic block is **ADDED / NEEDS REVIEW**.
- Further changes to this block require review.
- Full board schematic remains **NO**.
- Analog EMG chain schematic remains **NO**.
- Power/reference schematic block proposal documented in `V5/POWER_REFERENCE_SCHEMATIC_BLOCK_PROPOSAL.md`.
- Proposal defines maximum scope for a future small schematic-only power/reference block.
- Proposal has now been followed by the limited implementation in commit `1f759a6`.
- Proposal does not approve full board schematic, analog EMG chain schematic, PCB layout, or DSTK22807 external 3.3 V powering.
- Small power/reference schematic block proposal was **READY_FOR_REVIEW**.
- Limited power/reference schematic block implementation is **ADDED FOR REVIEW**.


## 8. Channel 1 Analog Status

- Checkpoint `8428e5e add channel 1 analog schematic candidate` adds `V5/EMG_CHANNEL_1_ANALOG.kicad_sch`.
- Checkpoint `adea50a fix channel 1 analog polarity and block rectifier placeholder` fixed Channel 1 analog polarity/topology issues and blocked the unfinished RECT topology.
- Checkpoint `3ecdacb implement channel 1 rectifier candidate` implements the reviewable Channel 1 VREF-centered absolute-value rectifier candidate.
- Checkpoint `3825181 cleanup channel 1 rectifier schematic readability` applied Channel 1 RECT schematic readability cleanup.
- Checkpoint `eca2cc3 assign channel 1 rectifier footprints` assigned Channel 1 RECT/ENV-related schematic footprint fields.
- Checkpoint `8f2bf9e clarify channel 1 rectifier diode values` clarified D331/D332 visible value text.
- Pre-duplication review after checkpoint `7170bbe sync docs with rectifier diode value cleanup` returned verdict **CHANNEL_1_READY_FOR_CHANNEL_2_DUPLICATION**.
- Root schematic `V5/EMG_v5.kicad_sch` includes hierarchical sheet `EMG_CHANNEL_1_ANALOG`.
- Channel 1 analog schematic candidate exists and uses real KiCad symbols, not placeholder boxes.
- `EMG1_RAW`, `EMG1_RECT`, and `EMG1_ENV` are connected to MCP3208 CH0, CH1, and CH2 through `POWER_REFERENCE_BLOCK`.
- `ADC_REF` is not used inside the analog child sheet.
- `analog VREF` remains the 1.65 V midbias reference.
- `ADC_REF` and `analog VREF` remain separate.
- `EMG1_REF_ELECTRODE` / sleeve is not tied to GND.
- J301 TRS mapping is fixed: Tip/T = `EMG1_IN_P`, Ring/R = `EMG1_IN_N`, Sleeve/S = `EMG1_REF_ELECTRODE`.
- U301 INA polarity is fixed: IN+ = `EMG1_IN_P_PROT`, IN- = `EMG1_IN_N_PROT`.
- U302A RAW gain topology is corrected to non-inverting feedback.
- U302B and U302C were repurposed from reserved followers into the two-op-amp RECT candidate.
- U302D remains the ENV buffer.
- RECT topology is no longer only a blocked placeholder in the KiCad schematic.
- Channel 1 RECT KiCad candidate: **IMPLEMENTED / REVIEWED / CLEANED READABILITY / FOOTPRINT FIELDS ASSIGNED / DIODE VALUES CLARIFIED / NOT FINAL HARDWARE APPROVAL**.
- Cleanup target was `V5/EMG_CHANNEL_1_ANALOG.kicad_sch` only.
- Footprint assignment target was `V5/EMG_CHANNEL_1_ANALOG.kicad_sch` only.
- Diode value-text clarification target was `V5/EMG_CHANNEL_1_ANALOG.kicad_sch` only.
- RECT block readability was improved without intentional topology/function change.
- Footprint assignment diff was limited to footprint property fields; no value, net, or topology changes were intended.
- Diode value-text clarification changed D331/D332 visible value properties only; no net, topology, symbol, footprint, or orientation change was made.
- R332 / the old RECT placeholder marker was fully removed.
- `EMG1_RECT_PLACEHOLDER_REMOVED` is no longer present.
- Stale/forbidden strings were checked and absent from the cleaned child sheet: `ADC_REF`, `No ERC`, `R_RECT_PLACEHOLDER`, `EMG1_RECT_PLACEHOLDER_REMOVED`, `RECT BLOCKED PLACEHOLDER`, `ENV placeholder`, `not functional until RECT`, and `TP_analog_VREF`.
- `EMG1_RECT_DRV` is no longer passively biased to `analog VREF` through the old 1M placeholder as the active implementation.
- D331/D332 Schottky candidate diodes were added.
- R333-R338 were added for the 10k/20k rectifier resistor network.
- New internal nets include `EMG1_RECT_SUM1`, `EMG1_RECT_A1_OUT`, `EMG1_RECT_U302B_DRV`, and `EMG1_RECT_SUM2`.
- Output remains `EMG1_RECT_DRV -> R331 470R -> EMG1_RECT`, with `C331 1nF` to GND.
- ENV path follows `EMG1_RECT_DRV` through `R341 33k`, `C341 1uF` to `analog VREF`, U302D buffer, and `EMG1_ENV_DRV`.
- `EMG1_RECT` and `EMG1_ENV` are no longer merely placeholder-only in KiCad, but still need PCB/hardware review before layout.
- No No ERC markers were added.
- No 5V analog/ADC use was introduced.
- KiCad GUI ERC after diode pin fixes returned to 6 expected placeholder errors only from U201 CH3/CH4/CH5/CLK/Din/`~CS/SHDN`.
- The temporary D331/D332 pin-not-connected errors were fixed.
- Real new ERC errors after RECT implementation: 0.
- Earlier warnings included symbol/library mismatch and planning placeholders; the MCP6004 and D_Schottky metadata mismatch warnings have since been cleaned. Remaining warnings are DSTK power/SPI planning placeholders only.
- Channel 2 has since been implemented as a controlled schematic duplicate/adaptation of Channel 1 at checkpoint `60d5881`.
- PCB remains **NOT STARTED**.
- DSTK SPI/power remains **NOT CONNECTED**.
- Exact MCP600x vendor model and exact diode model limitations still remain.
- Channel 1 RECT KiCad implementation received post-review verdict: **POST_REVIEW_PASS_WITH_CLEANUP_RECOMMENDED**.
- The implemented topology matches the planned VREF-centered rectifier candidate at schematic-connectivity level.
- This remains reviewable and is not final hardware approval.
- Final pre-duplication review verdict: **CHANNEL_1_READY_FOR_CHANNEL_2_DUPLICATION**.
- Channel 1 ready status: **CHANNEL_1_READY_FOR_CHANNEL_2_DUPLICATION_NOT_FINAL_HW_APPROVAL**.
- Issues requiring fix before Channel 2: none found in the reviewed gates.
- Channel 2 may start as a controlled schematic duplicate/adaptation of Channel 1.
- This is still not PCB approval and not final hardware approval.
- Confirmed pass item: RAW path still reaches `EMG1_RAW_DRV`, then `R321 -> EMG1_RAW` with `C321` to GND.
- Confirmed pass item: RECT path is present around U302B/U302C, D331/D332, and R333-R338.
- Confirmed pass item: D331/D332 values are `BAS70ZFILM`, symbol `Device:D_Schottky`, footprint `Diode_SMD:D_SOD-123`.
- Confirmed pass item: ENV path remains `EMG1_RECT_DRV -> R341 -> ENV_LPF_NODE`, `C341` to `analog VREF`, U302D -> `EMG1_ENV_DRV`.
- Confirmed pass item: output paths remain `EMG1_RECT_DRV -> R331 -> EMG1_RECT` and `EMG1_ENV_DRV -> R351 -> EMG1_ENV`.
- Confirmed pass item: `ADC_REF` is absent from the child sheet.
- Confirmed pass item: no No ERC markers were added.
- Confirmed pass item: old RECT placeholders and `BAT54/BAS70` text are absent.
- Confirmed pass item: `EMG1_REF_ELECTRODE` remains isolated from GND.
- Confirmed pass item: docs match schematic fields for D331/D332, U302, R333-R338, and C321/C331/C351/C341/C360.
- U302B role was confirmed: `+` input -> `analog VREF`, `-` input -> `EMG1_RECT_SUM1`, output -> `EMG1_RECT_U302B_DRV`.
- U302C role was confirmed: `+` input -> `analog VREF`, `-` input -> `EMG1_RECT_SUM2`, output -> `EMG1_RECT_DRV`.
- D331/D332 orientation matches the implementation plan using the local KiCad `Device:D_Schottky` pin convention.
- R333-R338 match the planned 10k/20k network.
- R332 / old RECT placeholder marker has now been fully removed and no longer biases `EMG1_RECT_DRV`.
- `EMG1_RECT_DRV` is driven by U302C output.
- `EMG1_RECT_DRV -> R331 470R -> EMG1_RECT` remains intact.
- `C331 1nF` to GND remains intact.
- ENV path remains `EMG1_RECT_DRV -> R341 33k -> ENV_LPF_NODE`, `C341 1uF` to `analog VREF`, U302D buffer -> `EMG1_ENV_DRV`.
- `ADC_REF` remains absent from the analog child sheet.
- `EMG1_REF_ELECTRODE` remains isolated from GND.
- No No ERC markers were added.
- No new 5V analog/ADC use was introduced.
- GUI ERC after diode fix showed 6 errors only: the expected U201 placeholder input errors CH3, CH4, CH5, CLK, Din, and `~CS/SHDN`.
- D331/D332 pin-not-connected errors were resolved.
- New real RECT ERC errors: 0.
- RECT block readability cleanup was applied at checkpoint `3825181`.
- D331/D332 and MCP6004 symbol metadata mismatch warnings have since been reviewed and cleaned; exact ordering/package/orientation review still remains before PCB.
- D331/D332 diode MPN/footprint direction is recorded for first-prototype planning, and schematic footprint fields were assigned at checkpoint `eca2cc3`.
- D331/D332 visible schematic value text was clarified at checkpoint `8f2bf9e`.
- D331 value is now `BAS70ZFILM`.
- D332 value is now `BAS70ZFILM`.
- The previous visible `BAT54/BAS70` ambiguity is removed from the Channel 1 analog schematic.
- KiCad `Device:D_Schottky` convention used by D331/D332: pin 1 = K / cathode and pin 2 = A / anode.
- D331/D332 still use KiCad symbol `Device:D_Schottky` and footprint `Diode_SMD:D_SOD-123`.
- Current D331/D332 orientation matches the previous Channel 1 RECT review:
  - D331 K -> `EMG1_RECT_U302B_DRV`, A -> `EMG1_RECT_A1_OUT`.
  - D332 K -> `EMG1_RECT_SUM1`, A -> `EMG1_RECT_U302B_DRV`.
- Do not use `BAS70FILM` with a plain 2-pin `Device:D_Schottky` symbol plus generic 3-pad SOT-23 footprint for the first prototype.
- `BAS70FILM` is deferred because the ST SOT-23 single diode uses two active package pins plus a third unused/NC-style pad and does not map cleanly without explicit custom mapping or a project-local 3-pin symbol.
- Recommended D331/D332 first-prototype direction:
  - MPN: `BAS70ZFILM`
  - Package: SOD-123
  - KiCad symbol: `Device:D_Schottky`
  - KiCad footprint: `Diode_SMD:D_SOD-123`
- `BAS70ZFILM` is preferred because the two-terminal SOD-123 package avoids SOT-23 third-pad / NC ambiguity and maps cleanly to the 2-pin diode symbol.
- Avoid `BAS70-04`, `BAS70-05`, `BAS70-06`, and BAT54A/C/S dual/common variants for this rectifier pass.
- Before PCB/layout, visually verify D331/D332 cathode-band orientation against the KiCad footprint and rectifier schematic.
- U302 MCP6004 MPN/package/footprint verification verdict: **MCP6004_PDIP14_SOCKETED_FIRST_PROTO_READY**.
- U302 uses KiCad symbol `Amplifier_Operational:MCP6004`.
- U302 units are present as one package: U302A, U302B, U302C, U302D, plus power unit.
- U302 pin mapping matches MCP6004/LM324-style quad op-amp pinout: U302A 1=OUT, 2=-, 3=+; U302B 7=OUT, 6=-, 5=+; U302C 8=OUT, 9=-, 10=+; U302D 14=OUT, 13=-, 12=+; power 4=V+, 11=V-.
- U302 power unit is present with V+ -> `3V3_ADC` and V- -> GND.
- C360 100nF local decoupling is present.
- MCP6004 caveat: the schematic previously had an MCP6004 symbol/library mismatch or stale metadata warning, but functional pin numbering matched the expected MCP6004 pinout. The embedded MCP6004 metadata mismatch has since been cleaned without pin/unit/net/topology/refdes/value/footprint change.
- Recommended U302 first-prototype direction: MPN `MCP6004-I/P`, package PDIP-14, KiCad symbol `Amplifier_Operational:MCP6004`, KiCad footprint `Package_DIP:DIP-14_W7.62mm`.
- Use socketed PDIP for first prototype bring-up/debug.
- Recommended compact U302 alternative: MPN `MCP6004-I/SL`, package SOIC-14, KiCad footprint `Package_SO:SOIC-14_3.9x8.7mm_P1.27mm`.
- MCP6004-I/ST / TSSOP-14 is only a later compact option with KiCad footprint `Package_SO:TSSOP-14_4.4x5mm_P0.65mm`; it is not recommended for the first prototype because hand bring-up/debug and inspection are worse than PDIP/SOIC.
- Before PCB, keep the MCP6004 package/MPN/ordering review open even though the MCP6004 metadata mismatch warning has been cleaned.
- Before PCB, confirm MCP6004 input common-mode range and output swing at `3V3_ADC = 3.3 V`.
- Before PCB, confirm rectifier crossover behavior and output stability with ADC RC caps and ENV/storage load.
- Exact MCP6004 vendor LTspice model is still not locally locked.
- Final MCP6004 purchase-page/datasheet package check is still required before ordering.
- Channel 1 RECT resistor matching verification verdict: **RECT_RESISTOR_MATCHING_DECISION_READY**.
- Confirmed RECT resistor roles: `R333 = 10k` RIN1, `R334 = 10k` RF1, `R335 = 1M` A1 bias / off-state return toward `analog VREF`, `R336 = 20k` RRAW2, `R337 = 10k` RA12, and `R338 = 20k` RF2.
- Ratio-critical resistors: R333/R334 as the 10k/10k first-stage pair, and R336/R337/R338 for the weighted summer.
- Key weighted-summer ratios: `R336 = R338 = 20k` and `R338/R337 = 2`.
- `R335 = 1M` is not ratio-critical; it is mainly bias/leakage/off-state related.
- First-prototype resistor decision: use 0.1% thin-film, 0805 for R333, R334, R336, R337, and R338.
- Use the same manufacturer/series/package/tempco for the ratio-critical resistors where possible.
- Reason: the first prototype should separate rectifier topology error from resistor mismatch.
- `R335 = 1M` may be 1% thin-film 0805; 0.1% is optional only for BOM simplicity or later sensitivity findings.
- Compact / cost-down alternatives: 0603 thin-film can be used later if layout density matters, and 1% thin-film may be considered after tolerance sensitivity or bench validation.
- 1% thick-film is probably functional but is not preferred for first validation of the rectifier candidate.
- Remaining risk: run tolerance sensitivity or bench check before relaxing ratio-critical parts to 1%.
- Exact resistor MPN/series/tempco is still not locked.
- MCP6004 offset/swing/crossover and BAS70 leakage may dominate small-signal error.
- Layout should keep the ratio network close and thermally similar.
- Channel 1 RAW/RECT/ENV capacitor dielectric/package verification verdict: **RECT_ENV_CAPACITOR_DECISION_READY**.
- Confirmed capacitor roles: `C321 = 1nF` RAW ADC/output cap to GND, `C331 = 1nF` RECT ADC/output cap to GND, `C351 = 4.7nF` ENV ADC/output cap to GND, `C341 = 1uF` ENV LPF storage cap from `ENV_LPF_NODE` to `analog VREF`, and `C360 = 100nF` U302 local decoupling cap.
- First-prototype capacitor decision for C321/C331: 1nF C0G/NP0, 0805, 5% or better preferred.
- First-prototype capacitor decision for C351: 4.7nF C0G/NP0, 0805 preferred; X7R is an acceptable fallback if C0G/NP0 availability becomes difficult.
- First-prototype capacitor decision for C341: 1uF X7R, 0805, 10% acceptable, with 16V or 25V rating to reduce DC-bias derating.
- First-prototype capacitor decision for C360: 100nF X7R, 0805, 16V or 25V local decoupling.
- Avoid Y5V/Z5U for these analog/output/decoupling capacitors.
- Compact alternative: 0603 can be considered later if layout density matters; keep C321/C331/C351 as C0G/NP0 where practical and keep C341/C360 as X7R.
- Remaining capacitor risk: C341 tolerance and DC-bias derating shift the ENV cutoff.
- Remaining capacitor risk: C341 returns to `analog VREF`, so VREF buffer stability/loading should be reviewed.
- Remaining capacitor risk: MCP6004 output stability with ADC/output caps should remain protected by 470R series resistors, but still needs review.
- Exact capacitor MPN/voltage/tolerance is not locked yet.
- Channel 1 RECT/ENV footprint field assignment status: **CHANNEL_1_RECT_FOOTPRINT_FIELDS_ASSIGNED_NOT_FINAL_HW_APPROVAL**.
- Assigned schematic footprint fields at checkpoint `eca2cc3`:
  - D331 = `Diode_SMD:D_SOD-123`
  - D332 = `Diode_SMD:D_SOD-123`
  - U302 = `Package_DIP:DIP-14_W7.62mm`
  - R333 = `Resistor_SMD:R_0805_2012Metric`
  - R334 = `Resistor_SMD:R_0805_2012Metric`
  - R335 = `Resistor_SMD:R_0805_2012Metric`
  - R336 = `Resistor_SMD:R_0805_2012Metric`
  - R337 = `Resistor_SMD:R_0805_2012Metric`
  - R338 = `Resistor_SMD:R_0805_2012Metric`
  - C321 = `Capacitor_SMD:C_0805_2012Metric`
  - C331 = `Capacitor_SMD:C_0805_2012Metric`
  - C351 = `Capacitor_SMD:C_0805_2012Metric`
  - C341 = `Capacitor_SMD:C_0805_2012Metric`
  - C360 = `Capacitor_SMD:C_0805_2012Metric`
- Root GUI ERC after footprint assignment remained 6 errors and 21 warnings.
- The 6 errors are still the expected U201 placeholder input errors: CH3, CH4, CH5, CLK, Din, and `~CS/SHDN`.
- No new D331/D332 pin-not-connected errors appeared after footprint assignment.
- No new real RECT/ENV ERC errors appeared after footprint assignment.
- D331/D332 schematic value text now says `BAS70ZFILM`; the previous visible `BAT54/BAS70` ambiguity is removed.
- D331/D332 still use symbol `Device:D_Schottky` and footprint `Diode_SMD:D_SOD-123`.
- No net, topology, footprint, symbol, or diode orientation change was made by the value-text clarification.
- Exact ordering MPNs for passives are not locked yet.
- D331/D332 cathode-band footprint orientation must be visually checked before PCB.
- MCP6004 package/MPN/ordering review still remains before PCB, although the MCP6004 metadata mismatch warning has since been cleaned.
- Final Channel 1 RECT status: **CHANNEL_1_READY_FOR_CHANNEL_2_DUPLICATION_NOT_FINAL_HW_APPROVAL**.
- D331/D332 diode MPN/footprint decision status: **CHANNEL_1_RECT_DIODE_MPN_FOOTPRINT_DECISION_READY_NOT_FINAL_HW_APPROVAL**.
- U302 MCP6004 MPN/footprint decision status: **CHANNEL_1_RECT_MCP6004_MPN_FOOTPRINT_DECISION_READY_NOT_FINAL_HW_APPROVAL**.
- Channel 1 RECT resistor matching decision status: **CHANNEL_1_RECT_RESISTOR_MATCHING_DECISION_READY_NOT_FINAL_HW_APPROVAL**.
- Channel 1 RAW/RECT/ENV capacitor decision status: **CHANNEL_1_RECT_ENV_CAPACITOR_DECISION_READY_NOT_FINAL_HW_APPROVAL**.
- Channel 1 RECT/ENV footprint field assignment status: **CHANNEL_1_RECT_FOOTPRINT_FIELDS_ASSIGNED_NOT_FINAL_HW_APPROVAL**.
- Channel 1 RECT diode value-text status: **CHANNEL_1_RECT_DIODE_VALUES_CLARIFIED_NOT_FINAL_HW_APPROVAL**.
- Channel 1 pre-duplication review status: **CHANNEL_1_READY_FOR_CHANNEL_2_DUPLICATION_NOT_FINAL_HW_APPROVAL**.
- Channel 2 may start as a controlled schematic duplicate/adaptation of Channel 1.
- Do not start PCB.

Channel 2 implementation and legacy root cleanup status:

- Current status: **CHANNEL_2_IMPLEMENTED_LEGACY_ROOT_REMOVED_NOT_FINAL_HW_APPROVAL**.
- Commit `60d5881 implement channel 2 analog schematic` created `V5/EMG_CHANNEL_2_ANALOG.kicad_sch`.
- Commit `60d5881` updated root sheet `V5/EMG_v5.kicad_sch`.
- Commit `60d5881` updated `V5/POWER_REFERENCE_BLOCK.kicad_sch`.
- Channel 2 was created as a controlled copy/adaptation of Channel 1.
- Channel 2 uses +100 refdes mapping: J401, U401, U402, R4xx, C4xx, and D431/D432.
- Channel 2 uses `EMG2_*` nets.
- Shared rails remain `3V3_ADC`, `analog VREF`, and GND.
- `ADC_REF` was not introduced.
- `EMG2_REF_ELECTRODE` remains isolated from GND/chassis/USB.
- No No ERC markers were added.
- PCB was not started.
- DSTK SPI/power was not connected.
- D431/D432 inherited the Channel 1 diode decision: `BAS70ZFILM`, `Device:D_Schottky`, and `Diode_SMD:D_SOD-123`.
- U402 inherited the Channel 1 op-amp package direction: MCP6004 and `Package_DIP:DIP-14_W7.62mm`.
- Channel 2 RECT resistors inherit the 0805 resistor footprint strategy.
- Channel 2 relevant capacitors inherit the 0805 capacitor footprint strategy.
- `EMG2_RAW` is connected to U201 CH3.
- `EMG2_RECT` is connected to U201 CH4.
- `EMG2_ENV` is connected to U201 CH5.
- Previous U201 CH3/CH4/CH5 placeholder errors disappeared.
- Commit `080eef1 remove legacy root emg schematic block` removed the old standalone/legacy root EMG schematic block from `V5/EMG_v5.kicad_sch`.
- The removed legacy flat block used the old J1/U1/U2/J2-style EMG circuit.
- Root now keeps the hierarchical `EMG_CHANNEL_1_ANALOG`, `EMG_CHANNEL_2_ANALOG`, and `POWER_REFERENCE_BLOCK` structure.
- Root GND PWR_FLAG/source declaration was restored cleanly after legacy block deletion.
- The old legacy block was not restored.
- GUI/ERC target after Channel 2 implementation and legacy cleanup reached 3 remaining errors.
- Remaining errors are expected SPI placeholders only: U201 CLK input not driven, U201 Din input not driven, and U201 `~CS/SHDN` input not driven.
- U201 CH3/CH4/CH5 errors are gone.
- U201 DGND error was fixed.
- GND label-not-connected error was fixed.
- #FLG power-output conflict was fixed.
- `3V3_ADC` / `analog VREF` label-not-connected errors were fixed.
- No D431/D432 pin-not-connected errors appeared.
- No new real Channel 1/Channel 2 errors remain.
- Remaining caveats: this is not final hardware approval; D331/D332/D431/D432 cathode-band orientation must be visually reviewed before PCB; exact passive ordering MPNs remain unlocked; DSTK power remains unresolved; DSTK SPI-only logic wiring is now connected; PCB remains not started. U302/U402 MCP6004 and D331/D332/D431/D432 D_Schottky metadata mismatch warnings have since been cleaned.

Channel TRS and RAW gain fix decision:

- Current status: **CHANNEL_TRS_AND_RAW_GAIN_FIX_PUSHED_NOT_FINAL_HW_APPROVAL**.
- Commit `1e5d3b0 fix channel trs mapping and raw gain input polarity` records the accepted schematic fix.
- Decision: TRS physical mapping and schematic net mapping fixed for both channels.
- J301 mapping is now Tip/T -> `EMG1_IN_P`, Ring/R -> `EMG1_IN_N`, Sleeve/S -> `EMG1_REF_ELECTRODE`.
- J401 mapping is now Tip/T -> `EMG2_IN_P`, Ring/R -> `EMG2_IN_N`, Sleeve/S -> `EMG2_REF_ELECTRODE`.
- Physical TRS electrode cable measurement note: sari/yellow tip = Tip/T, yesil/green middle body = Ring/R, kirmizi/red rear body = Sleeve/S.
- Resulting project mapping: sari/yellow Tip/T -> `EMGx_IN_P`, yesil/green Ring/R -> `EMGx_IN_N`, kirmizi/red Sleeve/S -> `EMGx_REF_ELECTRODE`.
- Decision: RAW gain op-amp input polarity fixed for both channels.
- U302A now has `RAW_HPF_NODE` on the non-inverting input / + and `RAW_GAIN_FB` on the inverting input / -; U302A output downstream RAW path is preserved.
- U402A now has `EMG2_RAW_HPF_NODE` on the non-inverting input / + and `EMG2_RAW_GAIN_FB` on the inverting input / -; U402A output downstream RAW path is preserved.
- Reason: post-implementation review found T/S mapping inversion and RAW positive-feedback risk.
- Constraints preserved: no PCB, no DSTK power connection, no `ADC_REF` in analog child sheets, `REF_ELECTRODE` isolated from GND/chassis/USB, no No ERC markers.
- Additional preserved boundaries: U301/U401 INA polarity unchanged, U302D/U402D ENV buffer unchanged, RECT path unchanged, ENV path unchanged, MCP3208/U201 mapping unchanged, root schematic unchanged, and `POWER_REFERENCE_BLOCK` unchanged.
- Root ERC after the earlier TRS/RAW post-fix review reported 29 total messages: 3 errors and 26 warnings; this historical result was superseded by the final cleanup review result of 8 total messages, and then by the DSTK SPI/U501 result of 2 total messages: 0 errors and 2 warnings.
- Former U201 SPI placeholder errors are now closed by the DSTK SPI-only integration.
- No unexpected post-fix errors were reported: no new Channel 1/Channel 2 error, no U201 CH0-CH5 input error, no DGND error, no GND / `3V3_ADC` / `analog VREF` label-not-connected error, no #FLG conflict, and no D331/D332/D431/D432 pin-not-connected error.
- This is not final hardware approval; the commit is now part of the pushed branch history.

Historical cleanup / remaining ERC warning decisions before DSTK SPI integration:

- Historical status: **FINAL_WARNINGS_DOCS_SYNC_READY_NOT_FINAL_HW_APPROVAL**.
- Historical local HEAD at that time: `ddb646e place unused U202B op amp unit safely`.
- Recent cleanup commits:
  - `0e23acf cleanup MCP6004 symbol metadata mismatch`
  - `39ff7c9 cleanup Schottky diode symbol metadata mismatch`
  - `ddb646e place unused U202B op amp unit safely`
- Decision: clean local embedded MCP6004 metadata only.
- Reason: the MCP6004 mismatch was metadata/library-cache level, not pin/topology.
- Scope: U302/U402 in the Channel 1 and Channel 2 analog sheets.
- Result: no pin, unit, net, topology, refdes, value, or footprint change; MCP6004 mismatch warnings are no longer present.
- Decision: clean local embedded `Device:D_Schottky` metadata only.
- Reason: the D_Schottky mismatch was `ki_fp_filters` metadata only.
- Scope: D331/D332/D431/D432.
- Result: `BAS70ZFILM` and `Diode_SMD:D_SOD-123` were retained; `BAT54`, `BAS70FILM`, SOT-23, or 3-pin diode ambiguity was not introduced; rectifier orientation was preserved; D_Schottky mismatch warnings are no longer present.
- Decision: place U202B unused op amp unit as a safe unity follower.
- Reason: unused op amp unit should not be left floating before PCB/final schematic review.
- Implementation: U202B non-inverting input pin 5 to `analog VREF`; inverting input pin 6 tied to output pin 7; output pin 7 only local feedback.
- U202B output is not connected to `ADC_REF`, `3V3_ADC`, GND, `VREF_MON`, `BAT_MON`, or any other signal path.
- U202A remains the analog VREF buffer.
- U202 power unit remains V+ -> `3V3_ADC` and V- -> GND.
- No No ERC marker, no `no_connect`, no `global_label`, and no `PWR_FLAG` were added.
- `ADC_REF` and `analog VREF` were not merged.
- Historical decision superseded by `79730dd`: U201 SPI input errors and `ADC_SCLK` / `ADC_MISO` / `ADC_MOSI` / `ADC_CS` isolated labels were kept until DSTK SPI integration; they are now resolved by the DSTK SPI-only connection.
- Decision: keep `DSTK_3V3_CANDIDATE` isolated label until the DSTK power-source strategy is finalized.
- Do not add No ERC markers for these placeholders unless explicitly decided later.
- These placeholders were blockers before PCB/final integration, but acceptable for that schematic cleanup checkpoint.
- Historical root ERC review after the cleanup series: 3 errors / 5 warnings / 8 total messages.
- Former U201 SPI placeholder errors were expected at that time and are now closed by the DSTK SPI-only integration.
- Remaining warning after DSTK SPI integration is the expected power-strategy placeholder `DSTK_3V3_CANDIDATE` isolated pin label; the `ADC_SCLK`, `ADC_MISO`, `ADC_MOSI`, and `ADC_CS` isolated pin label warnings are resolved.
- This is not final hardware approval.
- PCB has not been started.
- DSTK power remains unresolved; DSTK SPI-only logic wiring is now connected.

Channel 1 RECT LTspice simulation status:

- Simulation planning/prototype files now exist:
  - `V5/sim/rectifier/RECTIFIER_LTSPICE_PLAN.md`
  - `V5/sim/rectifier/emg_vref_abs_rectifier_candidate.cir`
  - `V5/sim/rectifier/emg_vref_abs_rectifier_real_topology_candidate.cir`
  - `V5/sim/rectifier/RECTIFIER_MODEL_CONFIRMATION.md`
  - `V5/sim/rectifier/emg_vref_abs_rectifier_conservative_rrio_candidate.cir`
- Behavioral target/load simulation was created first.
- Behavioral simulation validates the intended behavior: `RECT_OUT = VREF + abs(RAW_IN - VREF)`.
- Behavioral `RECT_OUT` tracks `IDEAL_RECT` and `ERR` is near 0.
- `EMG1_RECT_ADC` follows `RECT_OUT` through the 470R/1nF load path.
- `EMG1_ENV_DRV` gives plausible smoothing.
- This behavioral result is target/load validation only, not a real op-amp/diode implementation.
- A real-component LTspice candidate was added.
- The real topology candidate uses explicit op-amp stages, explicit Schottky diode approximations, and explicit resistor networks.
- `abs()` is used only for `IDEAL_RECT` comparison, not inside the rectifier core.
- Initial LTspice review showed the nominal 0.10 V amplitude case tracks `IDEAL_RECT` closely.
- Initial LTspice review showed the 0.50 V amplitude case remains close with only small crossover/spike error.
- Initial LTspice review showed the 1.00 V amplitude stress case reaches about 2.65 V and does not clip at 3.3 V.
- `ERR` is generally small, with mV-level crossover spikes in stress cases.
- The real topology candidate is **REVIEWABLE**, not automatically KiCad-approved.
- Exact MCP6004/MCP6002 LTspice model was not used.
- Exact BAS70/BAT54 LTspice model was not used.
- Generic RRIO op-amp and generic Schottky approximations were used.
- Because of these model limitations, KiCad implementation still requires manual schematic/topology review.
- KiCad schematic has now been updated with the Channel 1 RECT candidate at checkpoint `3ecdacb`.
- KiCad schematic readability cleanup was applied at checkpoint `3825181`.
- KiCad schematic footprint fields for Channel 1 RECT/ENV-related parts were assigned at checkpoint `eca2cc3`.
- KiCad schematic visible D331/D332 diode value text was clarified at checkpoint `8f2bf9e`.
- `EMG1_RECT_DRV` in KiCad is no longer placeholder-biased to `analog VREF` through the old active `1M R_RECT_PLACEHOLDER`.
- `EMG1_RECT` and `EMG1_ENV` are no longer merely placeholder-only in KiCad, but still need PCB/hardware review before layout.
- Model confirmation pass added `V5/sim/rectifier/RECTIFIER_MODEL_CONFIRMATION.md`.
- A conservative RRIO simulation candidate was added: `V5/sim/rectifier/emg_vref_abs_rectifier_conservative_rrio_candidate.cir`.
- Exact MCP6001/MCP6002/MCP6004/MCP600x local LTspice vendor models were not found.
- Exact BAS70 model was not found.
- Local LTspice Toshiba TBAT54 encrypted models were found under LTspice Contrib, but were not copied into the repo.
- Conservative candidate uses finite gain, finite bandwidth, limited output swing generic RRIO behavior and a generic Schottky approximation.
- Initial conservative RRIO candidate failed.
- Cause was found: op-amp macromodel transconductance source polarity was reversed.
- Broken form: `GIN INT 0 ...`.
- Corrected form: `GIN 0 INT ...`.
- This was a macromodel polarity bug, not a topology change.
- Rectifier core, resistor network, diode orientation, VREF references, RAW input, `IDEAL_RECT`, `ERR`, and load path were preserved.
- After the fix, the conservative nominal case tracks `IDEAL_RECT`.
- The conservative 1.00 V stress case reaches about 2.65 V and does not clip at 3.3 V.
- `ERR` is no longer volt-level and remains near 0 visually, with remaining exact error still model-dependent.
- `EMG1_RECT_ADC` follows `RECT_OUT`.
- `EMG1_ENV_DRV` gives plausible smoothing.
- Conservative RRIO simulation is now **PASS / REVIEWABLE**.
- Exact MCP600x vendor model is still not available locally.
- Exact BAS70/BAT54 vendor model is still not included in the repo.
- Final rectifier simulation/model status: **RECTIFIER_SIMULATION_MODEL_CONFIRMATION_REVIEWABLE_NOT_KICAD_APPROVED**.
- KiCad implementation now exists and passed the final Channel 1 pre-duplication review for controlled Channel 2 schematic duplication/adaptation.
- Any next schematic prompt should be limited to controlled Channel 2 schematic duplication/adaptation from Channel 1.
- Do not start PCB.
- Do not start PCB.
- Do not treat LTspice candidates as final hardware approval.


## 9. Open Blockers Before Further Schematic/PCB Work

The following blockers must be closed before further schematic or PCB work:

- Physical DSTK22807 pinout second confirmation.
- Right-row reversal confirmation.
- USB side and antenna side confirmation.
- DSTK22807 `3V3` / `5V` power behavior verification.
- DSTK22807 3V3 external input safety.
- DSTK22807 3V3 current capability.
- USB and external 3.3 V coexistence / backfeed behavior.
- Identify DSTK22807 onboard regulator / power path.
- Estimate or measure ESP32-H2 board current under expected firmware/BLE activity.
- Estimate minimal analog/ADC current.
- Estimate later full 2-channel EMG analog/ADC current.
- Perform DSTK `3V3` dummy-load test.
- Check `3V3` rail voltage under load.
- Check regulator temperature under load.
- Check ESP reset/stability behavior under load.
- Check ADC stability/noise under BLE/radio activity if wireless sampling is used.
- Review and accept `V5/FIRST_SCHEMATIC_POWER_SOURCE_STRATEGY.md`.
- Review `V5/EMG_CHANNEL_1_ANALOG.kicad_sch` visually and electrically.
- Channel 1 pre-duplication review is accepted for controlled Channel 2 schematic duplication/adaptation.
- Keep Channel 2 work limited to controlled schematic duplication/adaptation from Channel 1.
- Verify source selection is mutually exclusive in schematic.
- Confirm source selection mutual exclusion.
- Ensure `DSTK_3V3_CANDIDATE` is only a candidate input, not final approval.
- Ensure `LDO_3V3_FALLBACK` does not backfeed DSTK `3V3`.
- Confirm no backfeed path from LDO to DSTK `3V3`.
- Decide default population state for source jumpers/solder bridges.
- Mark source population as TBD until DSTK 3V3 load/noise testing.
- Battery regulator architecture.
- Battery chemistry and maximum voltage.
- 3.3 V regulator part/current/noise selection, if separate LDO fallback is selected.
- Decide whether the first schematic block should use the selectable/jumper source option with `DSTK_3V3_CANDIDATE`, `LDO_3V3_FALLBACK`, and selected `3V3_ADC`.
- INA333/op-amp headroom at 3.3 V.
- `analog VREF` buffer topology/part.
- MCP3208 ADC input protection and source impedance/acquisition requirements.
- `BAT_MON` divider/scaling.
- `AGND` / `DGND` grounding strategy.
- Battery choice narrowed enough for regulator and BAT_MON.
- Exact regulator candidate/suffix/package chosen if separate LDO fallback is selected.
- Current budget for analog/ADC estimated.
- Decision whether DSTK22807 is excluded from first power block or included later.
- DSTK22807 external 3.3 V powering test result if Option C is considered.
- analog VREF buffer candidate verified at 3.3 V.
- ADC_REF decoupling/filter values accepted.
- Confirm `ADC_REF` and analog `VREF` are separate.
- ADC input source impedance/acquisition calculation accepted.
- BAT_MON divider values accepted.
- Unused ADC bias values accepted.
- Human-test safety procedure accepted.
- Review and accept `V5/POWER_REFERENCE_SCHEMATIC_BLOCK_PROPOSAL.md`.
- Confirm MCP3208 channel labels match locked CH0-CH7 mapping.
- Confirm DSTK pinout before connecting SPI/power to DSTK symbol.
- Historical blocker superseded by `79730dd`: DSTK SPI is now connected to U501 GPIO14/13/12/11; DSTK power remains unresolved until power behavior is verified.
- Confirm DSTK 3V3 current/load/noise tests before choosing population path.
- Verify exact KiCad symbols/footprints for regulator/op-amp/connector/test points.
- Verify analog VREF buffer stability at 3.3 V.
- Accept ADC_REF decoupling strategy.
- Accept BAT_MON divider candidate.
- Accept ADC input R/C/source impedance strategy.
- Decide unused ADC bias approach.
- Keep DSTK22807 power path unresolved unless separately tested.
- Keep DSTK 3V3 current/load/noise test blocker.
- Keep human-test safety procedure blocker.
- Keep ADC_REF/analog VREF separation blocker.
- Human-test safety procedure remains required.


## 10. Should schematic implementation proceed?

Full board schematic: **NO**.

Full two-channel analog EMG chain schematic: **CANDIDATES IMPLEMENTED / NOT FINAL HARDWARE APPROVAL**.

Channel 1 RECT KiCad candidate: **IMPLEMENTED / REVIEWED / NOT FINAL HARDWARE APPROVAL**.

Channel 2 analog schematic: **IMPLEMENTED / NOT FINAL HARDWARE APPROVAL**.

Channel TRS / RAW gain fix: **PUSHED / NOT FINAL HARDWARE APPROVAL**.

PCB layout: **NO**.

Small power/reference schematic block proposal: **READY_FOR_REVIEW**.

Small power/reference schematic block: **ADDED FOR REVIEW**.

Further changes to this block require review.

Small power/reference schematic block implementation: **ADDED FOR REVIEW** with selectable/jumper power-source strategy.

First schematic power-source strategy: **SELECTABLE / JUMPER SOURCE OPTION**.

DSTK onboard `3V3` as analog/ADC source: **CANDIDATE ONLY**, pending current budget and load/noise test.

External analog/ADC LDO: **CONSERVATIVE FALLBACK CANDIDATE**.

`3V3_ADC` source selection: **MUTUALLY EXCLUSIVE**.

DSTK22807 external 3.3 V powering: **NOT APPROVED YET**.

USB-powered human EMG testing: **FORBIDDEN**.

5V pin as analog/ADC supply: **NOT APPROVED**.

## 11. Recommended Next Action

- Continue with review/planning only; do not treat the TRS/RAW gain fix, DSTK SPI integration, or DSTK power placeholder decision as final hardware approval.
- Review D331/D332 `BAS70ZFILM` / `Diode_SMD:D_SOD-123` cathode-band orientation visually against the schematic before PCB.
- Review D431/D432 `BAS70ZFILM` / `Diode_SMD:D_SOD-123` cathode-band orientation visually against the schematic before PCB.
- Keep U302 `MCP6004-I/P` / `Package_DIP:DIP-14_W7.62mm` and U402 `MCP6004` / `Package_DIP:DIP-14_W7.62mm` package/orderability review open before PCB; the MCP6004 metadata mismatch warning itself has been cleaned.
- Lock exact ordering MPNs for R333/R334/R336/R337/R338 as 0.1% thin-film 0805 from the same series where possible; keep R335 as 1% thin-film 0805 unless BOM simplification or sensitivity results justify 0.1%.
- Lock exact ordering MPNs for C321/C331 as 1nF C0G/NP0 0805, C351 as 4.7nF C0G/NP0 0805 preferred, and C341/C360 as X7R 0805 with suitable voltage ratings before Channel 2 or PCB.
- Confirm ENV behavior and analog VREF buffer loading after RECT/capacitor decisions.
- Plan DSTK power-source strategy separately; DSTK SPI-only logic wiring is now connected.
- Current root ERC review result is 0 errors plus 2 known warnings; do not add No ERC markers for the remaining planning/library warnings.
- Current DSTK power-source review verdict is `DSTK_POWER_PLAN_KEEP_PLACEHOLDER_DOCS_ONLY`.
- Do not start PCB.
- Do not treat the KiCad/LTspice RECT candidates as final hardware approval.
- Do not connect DSTK22807 3V3 or 5V/VBUS to analog/ADC rails until power behavior is verified and explicitly approved.
- Do not begin full board schematic.
- Do not use 5V as analog/ADC supply.

## Source Documents Used

- `V5/DSTK22807_PHYSICAL_PINOUT_OBSERVATION.md`
- `V5/DSTK22807_SPI_PIN_REVIEW.md`
- `V5/MCP3208_MPN_SYMBOL_FOOTPRINT_REVIEW.md`
- `V5/POWER_REFERENCE_ARCHITECTURE_REVIEW.md`
- `V5/BOM_V5_DRAFT.md`
- `V5/STATUS_V5.md`
- `V5/DSTK22807_MEASUREMENT_CHECKLIST.md`
- `V5/DSTK22807_PHOTO_PIN_NOTES.md`
- `V5/DSTK22807_POWER_PIN_MEASUREMENT.md`
- `V5/POWER_REFERENCE_COMPONENT_CANDIDATES.md`
- `V5/POWER_REFERENCE_VALUE_LEVEL_DESIGN_REVIEW.md`
- `V5/POWER_REFERENCE_SCHEMATIC_BLOCK_PROPOSAL.md`
- `V5/DSTK22807_3V3_RAIL_CURRENT_BUDGET_REVIEW.md`
- `V5/FIRST_SCHEMATIC_POWER_SOURCE_STRATEGY.md`
- `V5/POWER_REFERENCE_BLOCK.kicad_sch`
- `V5/sim/rectifier/RECTIFIER_LTSPICE_PLAN.md`
- `V5/sim/rectifier/emg_vref_abs_rectifier_candidate.cir`
- `V5/sim/rectifier/emg_vref_abs_rectifier_real_topology_candidate.cir`
- `V5/sim/rectifier/RECTIFIER_MODEL_CONFIRMATION.md`
- `V5/sim/rectifier/emg_vref_abs_rectifier_conservative_rrio_candidate.cir`
- `V5/EMG_CHANNEL_1_ANALOG.kicad_sch`
- `V5/EMG_CHANNEL_2_ANALOG.kicad_sch`
- `V5/EMG_v5.kicad_sch`
- `V5/EMG_v5.kicad_pro`

Final decision: **3V3_ADC_CURRENT_BUDGET_SUPPORTS_TPS7A2033_PREFERRED_WITH_BACKFEED_CAVEAT_NOT_FINAL_HW_APPROVAL**
