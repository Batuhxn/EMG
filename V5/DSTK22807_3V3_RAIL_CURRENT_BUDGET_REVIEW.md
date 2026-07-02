# V5 DSTK22807 3V3 Rail Current Budget Review

## 1. Current repo state

| Item | State |
| --- | --- |
| Branch | `v5/measured-dstk22807-footprint` |
| HEAD | `3abebc8 update decisions with schematic block proposal` |
| Git status | Clean before writing this document |
| Remote tracking | `github-emg/v5/measured-dstk22807-footprint` |
| Scope | Documentation-only current budget review |
| KiCad implementation | No KiCad schematic/PCB implementation in this task |

## 2. Purpose

This review re-evaluates whether a separate external 3.3 V regulator is truly mandatory for the V5 analog/ADC block.

The focus is the DSTK22807 onboard `3V3` rail current capability versus the expected V5 analog/ADC current demand.

This document is not a `DECISIONS_V5.md` update. This document is not schematic implementation. This document does not change human-safety rules.

The electrician comment is technically plausible: if the DSTK22807 onboard `3V3` rail is already regulated and clean enough, the main remaining question is current/noise margin. However, the onboard regulator and power path are still not officially identified, so this is a candidate architecture, not an approved design.

## 3. Existing measured facts

Source: `V5/DSTK22807_POWER_PIN_MEASUREMENT.md`.

USB Type-C connected, no external power:

- `5V` pin = 5.126 V.
- `3V3` pin = 3.291 V.

USB disconnected, external power disconnected:

- `5V` pin = 0 V.
- `3V3` discharge:
  - 10 s = 0.590 V.
  - 30 s = 0.400 V.
  - 60 s = 0.250 V.

These measurements show:

- The onboard `3V3` rail is observable while USB is connected.
- The unpowered `3V3` decay is consistent with residual capacitor discharge / leakage.

These measurements do not prove:

- `3V3` pin current capability.
- `3V3` rail noise or PSRR under analog load.
- Exact `3V3` regulator part or current limit.
- Safe external 3.3 V input into DSTK22807.
- USB plus external 3.3 V coexistence safety.

## 4. Architecture options being compared

| Option | Description | Pros | Risks | Current status |
| --- | --- | --- | --- | --- |
| Option A | Battery/external source -> separate 3.3 V LDO -> analog/ADC only | Conservative, cleaner analog rail, avoids depending on unknown DSTK regulator | More components, more board space, more complexity; DSTK is not powered by this block | Conservative candidate |
| Option B | DSTK22807 onboard `3V3` rail -> analog/ADC rail | Simpler, fewer parts, may remove external LDO | Requires current budget, dummy-load test, and noise verification; onboard LDO exact model is UNKNOWN | Newly considered candidate |
| Option C | Battery/external source -> regulator -> analog/ADC + DSTK22807 via `3V3` | Potential final standalone battery architecture | Requires DSTK external 3.3 V input safety, backfeed, and current verification | NOT APPROVED YET |
| Option D | `5V` pin or USB-derived 5 V as analog/ADC supply | None for current analog/ADC plan | 5 V analog/ADC supply is not approved; human USB testing remains forbidden | NOT APPROVED |

## 5. What "DSTK 3V3 can power analog/ADC" would require

- Exact onboard 3.3 V regulator or power path identified.
- Safe available current estimated from official part data or measured by load test.
- ESP32-H2 board current estimated or measured under expected USB/BLE/activity conditions.
- Analog/ADC current budget estimated.
- Total current plus transient/future margin acceptable.
- `3V3` rail remains within acceptable voltage under load.
- Analog noise is acceptable for EMG ADC measurements.
- Human-test rule remains battery-isolated/no USB.

## 6. Current budget items

| Subsystem | Part / block | Estimated current | Source / confidence | Notes |
| --- | --- | --- | --- | --- |
| ESP32-H2 SoC active CPU/modem-sleep | ESP32-H2 | 5 mA to 17 mA typical depending clock/peripheral clocks in modem-sleep CPU states | Espressif ESP32-H2 datasheet, medium for SoC only | Dev-board current will be higher; board-level measurement still required. |
| ESP32-H2 BLE / 802.15.4 active RF | ESP32-H2 | BLE RX about 24 mA peak; BLE TX about 24 mA to 140 mA peak depending output power; 802.15.4 similar order | Espressif ESP32-H2 datasheet, medium for SoC only | Use BLE-active/noise test if wireless will run during sampling. |
| USB-serial/regulator/LEDs on DSTK board | DSTK22807 board | TBD / needs measurement | Board exact schematic and LDO unknown | Can dominate margin if LEDs or USB bridge are active. |
| MCP3208 ADC | MCP3208-CI/P | About 225 uA at 2.7 V condition; up to 400 uA listed at 5 V condition | Microchip MCP3208 datasheet, high | Use 0.4 mA as simple conservative budget placeholder. |
| INA333 per channel | INA333 | 50 uA typical | TI INA333 datasheet, high for IC typical | Two channels about 100 uA typical before surrounding loads. |
| Op-amp stages per channel | MCP6002/MCP6004-class | 100 uA typical, 170 uA max per amplifier | Microchip MCP6001/2/4 datasheet, high for IC | Multiply by actual amplifier count; MCP6004 quad = about 0.4 mA typ, 0.68 mA max if all amps active. |
| Rectifier/envelope op-amps if later populated | MCP6002/MCP6004-class | 100 uA typical, 170 uA max per amplifier | Microchip datasheet, medium | Stage count is not locked. |
| analog VREF buffer | MCP6002/MCP6004-class one amplifier candidate | 100 uA typical, 170 uA max | Microchip datasheet, medium | Load current from bias networks still needs summing. |
| 47k/47k analog VREF divider | Resistor divider | 35.1 uA at 3.3 V | Calculation, high | `3.3 V / 94k`. |
| BAT_MON divider 330k/1M | Battery monitor divider | 3.16 uA at 4.2 V max battery assumption | Calculation, high | `4.2 V / 1.33M`; battery assumption not locked. |
| ADC input bias resistors | Example 100k to analog VREF | 16.5 uA per channel if tied to 1.65 V through 100k | Calculation, medium | Depends on how many future/unused channels are biased and to which rail. |
| Indicator LEDs, if any | DSTK or EMG board LEDs | TBD / needs measurement | Board/schematic observation required | A single LED can exceed the entire analog budget. |
| Margin/reserve | Regulator/rail capacity | TBD | Engineering rule | Use 50-70% max continuous load rule after verified available current is known. |

## 7. Rough V5 analog/ADC load estimate

These are planning estimates only. They exclude the ESP32-H2/DSTK board itself unless explicitly stated.

### Minimal first block estimate

Includes MCP3208, analog VREF divider, analog VREF buffer, BAT_MON divider, test point/leakage negligible, and no full EMG analog chain.

| Item | Low estimate | High/conservative estimate | Notes |
| --- | --- | --- | --- |
| MCP3208 | 0.225 mA | 0.400 mA | Datasheet conditions vary; use 0.4 mA for budget. |
| analog VREF divider 47k/47k | 0.035 mA | 0.035 mA | Fixed by candidate values. |
| analog VREF buffer | 0.100 mA | 0.170 mA | One MCP600x amplifier. |
| BAT_MON divider 330k/1M | 0.003 mA | 0.004 mA | 1S LiPo 4.2 V assumption. |
| Test points / leakage | Negligible | TBD | Depends on contamination/probing, normally tiny. |
| Total analog/ADC current | About 0.36 mA | About 0.61 mA | Excludes DSTK board current and optional LEDs. |

### Later full 2-channel EMG estimate

Includes MCP3208, INA/op-amp stages, rectifier/envelope op-amps, analog VREF buffer, dividers/bias networks, and any LEDs if used.

| Item | Low estimate | High/conservative estimate | Notes |
| --- | --- | --- | --- |
| MCP3208 | 0.225 mA | 0.400 mA | ADC active budget placeholder. |
| INA333, two channels | 0.100 mA | TBD | 50 uA typical each; max/conditions should be checked for selected operating point. |
| MCP600x op-amp stages | 0.800 mA | 2.040 mA | Example 8 to 12 amplifier sections at 100 uA typ to 170 uA max; actual stage count TBD. |
| analog VREF buffer | 0.100 mA | 0.170 mA | One amplifier section. |
| analog VREF divider | 0.035 mA | 0.035 mA | 47k/47k candidate. |
| BAT_MON divider | 0.003 mA | 0.004 mA | 330k/1M candidate. |
| Unused/future ADC bias | 0.000 mA | 0.100 mA | Example six 100k ties to 1.65 V would be about 99 uA total. |
| Indicator LEDs | TBD | TBD | Do not omit if added; LED current can dominate. |
| Total analog/ADC current | About 1.26 mA plus TBD items | About 2.75 mA plus TBD items | Treat as preliminary until analog stage count is locked. |

Interpretation: the analog/ADC load is likely small compared with ESP32-H2 RF peaks, but that does not automatically approve using DSTK `3V3`; the unknown is the DSTK onboard regulator available margin and rail noise.

## 8. Current margin rule

Do not use the DSTK onboard `3V3` rail for analog/ADC unless measured or official available current comfortably exceeds:

- ESP32-H2 board current.
- Analog/ADC current.
- Transient margin.
- Future expansion margin.

Suggested margin:

- Total expected continuous load should ideally stay below 50-70% of verified available regulator capability.
- If the exact regulator current limit is unknown, require load testing.
- If the analog/ADC rail shares DSTK radio bursts, verify ADC readings during active BLE/802.15.4 operation.

## 9. Measurement plan for DSTK 3V3 rail capability

Safety:

- No human electrodes.
- No USB human testing.
- No oscilloscope earth while a human is connected.
- Use dummy load only.

Measurement steps:

1. Identify onboard 3.3 V regulator marking under magnification if possible.
2. Measure idle current of DSTK board from USB or a safe supply setup.
3. Measure `3V3` voltage no-load.
4. Add dummy loads to the `3V3` pin in steps:
   - 5 mA.
   - 10 mA.
   - 20 mA.
   - 30 mA.
   - 50 mA if safe.
5. For each load record:
   - `3V3` voltage.
   - Board current.
   - Regulator temperature by touch/IR if available.
   - ESP stability/reset behavior.
   - USB/serial/BLE behavior if applicable.
6. Stop the test if:
   - `3V3` drops below acceptable range.
   - Regulator gets hot.
   - ESP resets.
   - USB disconnects.
   - Abnormal smell/heat appears.

Suggested dummy resistor values for 3.3 V:

| Target load | Approx resistor | Power at 3.3 V |
| --- | --- | --- |
| 5 mA | 660 ohm | 0.0165 W |
| 10 mA | 330 ohm | 0.033 W |
| 20 mA | 165 ohm | 0.066 W |
| 30 mA | 110 ohm | 0.099 W |
| 50 mA | 66 ohm | 0.165 W |

Use adequate resistor power rating. Formula: `P = V^2 / R`. Do not burn small resistors.

## 10. Noise / ADC reference considerations

Even if current is enough, ADC reference quality still matters.

If using DSTK `3V3` for MCP3208 `VDD` / `ADC_REF`:

- Local MCP3208 decoupling is required.
- `ADC_REF` filtering/decoupling is required.
- analog VREF buffer is still required.
- `ADC_REF` and analog VREF remain separate.

If digital bursts, USB noise, or radio activity affect ADC readings, separate local filtering or a separate LDO may still be useful.

ESP32-H2 has BLE and IEEE 802.15.4 radio activity. The `3V3` rail noise should be checked during active wireless operation if BLE/802.15.4 will run during acquisition.

## 11. Updated interpretation

External 3.3 V LDO is not automatically required.

DSTK22807 onboard `3V3` rail may be a valid simpler candidate for analog/ADC power if current and noise are acceptable.

The decisive factor is current budget plus load/noise measurement.

External LDO remains a conservative fallback if:

- DSTK `3V3` current is insufficient.
- Rail droop/noise is unacceptable.
- Analog accuracy suffers.
- Thermal margin is poor.
- DSTK regulator model cannot be verified and load test fails.

## 12. Decision recommendation

- External analog/ADC 3.3 V regulator: DOWNGRADED FROM PREFERRED REQUIREMENT TO CONSERVATIVE FALLBACK CANDIDATE, pending review.
- DSTK22807 onboard 3V3 rail for analog/ADC: NEW CANDIDATE, pending current budget and load/noise test.
- DSTK22807 external 3.3 V powering: NOT APPROVED YET.
- USB-powered human EMG testing: FORBIDDEN.
- 5V as analog/ADC supply: NOT APPROVED.
- ADC_REF and analog VREF remain separate.
- Full board schematic: NO.
- Analog EMG chain schematic: NO.
- PCB layout: NO.
- KiCad implementation: NOT IN THIS TASK.

## 13. Open blockers

- Identify DSTK22807 onboard regulator / power path.
- Estimate ESP32-H2 board current under expected firmware/BLE.
- Estimate analog/ADC current for minimal block.
- Estimate full 2-channel EMG current.
- Perform `3V3` dummy-load test.
- Check rail noise or ADC stability under load/BLE if possible.
- Decide whether first schematic block should use:
  - DSTK `3V3` as source, or
  - separate LDO fallback, or
  - selectable/jumper option.
- Update `DECISIONS_V5.md` after review.

## 14. Recommended next action

- Review and commit this current budget review document.
- Then update `DECISIONS_V5.md` in a separate commit.
- Then revise the future KiCad schematic block prompt:
  - either use DSTK `3V3` as candidate source,
  - or include jumper/selectable source,
  - or keep separate LDO fallback.
- Do not proceed to schematic edit until this power-source decision is clear.

## 15. Source documents used

Local source documents used:

- `V5/DECISIONS_V5.md`
- `V5/DSTK22807_POWER_PIN_MEASUREMENT.md`
- `V5/BATTERY_REGULATOR_POWER_PATH_PLAN.md`
- `V5/POWER_REFERENCE_ARCHITECTURE_REVIEW.md`
- `V5/POWER_REFERENCE_COMPONENT_CANDIDATES.md`
- `V5/POWER_REFERENCE_VALUE_LEVEL_DESIGN_REVIEW.md`
- `V5/POWER_REFERENCE_SCHEMATIC_BLOCK_PROPOSAL.md`
- `V5/MCP3208_MPN_SYMBOL_FOOTPRINT_REVIEW.md`
- `V5/DSTK22807_SPI_PIN_REVIEW.md`
- `V5/BOM_V5_DRAFT.md`
- `V5/STATUS_V5.md`

Official manufacturer sources used:

- Espressif ESP32-H2 Series Datasheet v1.2: `https://www.espressif.com/sites/default/files/documentation/esp32-h2_datasheet_en.pdf`
- Microchip MCP3204/3208 datasheet: `https://ww1.microchip.com/downloads/aemDocuments/documents/APID/ProductDocuments/DataSheets/21298e.pdf`
- Texas Instruments INA333 datasheet: `https://www.ti.com/lit/ds/symlink/ina333.pdf`
- Microchip MCP6001/1R/1U/2/4 datasheet: `https://ww1.microchip.com/downloads/aemDocuments/documents/MSLD/ProductDocuments/DataSheets/MCP6001-1R-1U-2-4-1-MHz-Low-Power-Op-Amp-DS20001733L.pdf`

DSTK22807 onboard LDO exact model: UNKNOWN. Official current/rating for the DSTK board rail needs later verification or measurement.

Final decision: **DSTK_3V3_RAIL_CURRENT_BUDGET_REVIEW_READY_FOR_REVIEW**
