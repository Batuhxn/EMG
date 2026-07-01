# V5 Battery / Regulator Architecture and DSTK22807 Power-Path Verification Plan

## 1. Current repo state

| Item | State |
| --- | --- |
| Project path | `C:\Users\Batuhan\Desktop\EMG_v4_gelistirilmis_paket` |
| Branch | `v5/measured-dstk22807-footprint` |
| Git status before writing | Clean |
| Scope | Planning/verification only. No schematic implementation, PCB change, footprint edit, firmware change, gerber, or production output. |

Last 11 commits at review start:

```text
e92afa1 update decisions with DSTK power measurements
13b9003 document DSTK22807 power pin measurements
d7b4eb6 document power reference component candidates
83768c0 restore V5 decisions log
cdc9f74 document V5 power reference architecture review
2924677 document MCP3208 MPN symbol footprint review
8a252b4 document DSTK22807 SPI pin candidates
f6e2b14 document observed DSTK22807 pinout mapping
3824d20 clean up DSTK22807 carrier footprint silkscreen
b4ba0e9 add measured provisional DSTK22807 carrier footprint
b7384a1 emergency baseline from transferred EMG project
```

Official manufacturer references used for context:

- Microchip MCP3208 product/datasheet source: `https://www.microchip.com/en-us/product/MCP3208`
- Microchip MCP1700 product/datasheet source: `https://www.microchip.com/en-us/product/MCP1700`
- Microchip MCP1702 product/datasheet source: `https://www.microchip.com/en-us/product/MCP1702`
- TI TLV755P product/datasheet source: `https://www.ti.com/product/TLV755P`

Exact final BOM is not locked by this document.

## 2. Purpose and boundary

This document plans the V5 battery/regulator architecture and the DSTK22807 power-path verification work needed before schematic power implementation.

This document does not approve schematic implementation.

This document does not approve external 3.3 V powering of DSTK22807.

This document does not approve USB Type-C plus external 3.3 V simultaneous use.

This document does not approve the 5V pin as the analog/ADC supply.

## 3. Known DSTK22807 power measurements

| Condition | 5V pin | 3V3 pin | Interpretation |
| --- | --- | --- | --- |
| USB Type-C connected, no external power | 5.126 V | 3.291 V | USB-powered 5V and 3V3 rails are observable on board/header pins. |
| USB disconnected, external power disconnected | 0 V | Not a stable active rail | 5V falls to 0 V when power is removed. |
| 10 s after power removal | Not measured again | 0.590 V | 3V3 residual voltage remains after power removal. |
| 30 s after power removal | Not measured again | 0.400 V | 3V3 rail continues discharging. |
| 60 s after power removal | Not measured again | 0.250 V | Decay is consistent with residual capacitor discharge / leakage path. |

These measurements prove that USB-powered 5V and approximately 3.3 V rails are observable on board pins. The 3V3 decay after power removal is consistent with residual capacitor discharge / leakage.

These measurements do not prove external 3V3 input safety, 3V3 current capability, USB/external 3.3 V coexistence, or safe backfeeding behavior.

## 4. Architecture options

| Option | Power path | Pros | Risks | Human-test suitability | Status |
| --- | --- | --- | --- | --- | --- |
| Option A | Battery -> external 3.3 V regulator -> analog/ADC only. DSTK22807 powered separately only for bench/firmware work. | Lowest immediate risk for analog/ADC bring-up; avoids unverified module backfeed path. | Does not yet provide fully standalone digital output during human testing; later isolated/DSTK power plan still needed. | Human EMG test would still require no USB and a later approved isolated/DSTK power plan. | Preferred for analog/ADC-only electrical bring-up. |
| Option B | Battery -> external 3.3 V regulator -> analog/ADC + DSTK22807 through 3V3 pin. | Clean single 3.3 V battery architecture if proven safe; avoids 5V boost noise and inefficiency. | Only possible after DSTK22807 3V3 external input safety, current draw, and backfeed behavior are verified. | Potentially suitable after verification; not approved now. | NOT APPROVED YET. |
| Option C | Battery -> 5V boost/regulator -> DSTK22807 5V pin -> board 3V3 rail. | May work if module expects 5V input and 3V3 input is unsafe. | Adds boost noise, conversion loss, more power-path complexity, and possible analog contamination. Not preferred for analog/ADC. | Not preferred; requires isolation and noise review. | Not preferred; only consider if 3V3 input is not safe and module requires 5V input. |
| Option D | USB Type-C powers DSTK22807 during firmware/bench-only development. | Convenient for firmware flashing and bench checks. | USB/mains-connected path is forbidden for human-connected EMG. Can create ground/safety hazards. | Not suitable for human-connected testing. | Allowed only for firmware/bench work with no human electrodes. |

## 5. Recommended provisional architecture

- For analog/ADC first bring-up, use an external 3.3 V regulator to power MCP3208 and the analog front-end.
- Do not power analog/ADC from 5V.
- Keep DSTK22807 power path unresolved until verification.
- Prefer Option A for analog/ADC-only electrical bring-up.
- Option B may become the preferred final battery architecture only after a controlled external 3.3 V powering test passes.
- USB Type-C is allowed only for firmware/bench work with no human electrodes.

## 6. DSTK22807 external 3.3 V powering verification plan

This is a future safe test plan. It must not be interpreted as approval to power DSTK22807 through its 3V3 pin.

Required test conditions:

- No human electrodes.
- No analog front-end connected to a human.
- USB Type-C disconnected.
- Use a current-limited bench supply or battery-fed 3.3 V regulator with current measurement.
- Start with a low current limit if using a bench supply.
- Verify polarity before connection.
- Measure current draw.
- Check whether the board boots.
- Check whether 3V3 remains stable.
- Check whether the 5V pin becomes back-fed.
- Check temperature/heating.
- Do not connect USB and external 3.3 V simultaneously.

| Step | Action | Expected safe observation | Stop condition | Status |
| --- | --- | --- | --- | --- |
| 1 | Unpowered resistance/continuity sanity checks between 3V3, 5V, and GND. | No obvious short; resistance not near zero. | Near-short or unexpected continuity suggesting damage/backfeed risk. | Planned |
| 2 | Apply external 3.3 V with USB disconnected and current limit set low. | Current remains below limit; 3V3 rail does not collapse. | Current limit hit, voltage collapse, heating, smell, or unstable behavior. | Planned |
| 3 | Measure input current at idle. | Stable current draw recorded for later regulator sizing. | Current exceeds expected safe limit or rises over time. | Planned |
| 4 | Check 5V pin for backfeed voltage while powered from 3V3. | Ideally no unsafe 5V backfeed; exact acceptable value TBD. | Significant 5V pin voltage or unknown path that may power other equipment. | Planned |
| 5 | Check board boot / LED / serial / BLE only if safe. | Board shows expected signs of life without USB. | Boot failure with high current, unstable 3V3, or heating. | Planned |
| 6 | Remove power and confirm discharge. | 3V3 decays safely after power removal. | Rail remains unexpectedly driven or backfed. | Planned |
| 7 | Document result before approval. | Results captured in a new observation document and decisions updated separately. | Any ambiguity remains unresolved. | Planned |

External 3.3 V powering remains **NOT APPROVED YET** until the test is executed, documented, reviewed, and committed.

## 7. Regulator architecture requirements

| Requirement | Target | Reason | Status |
| --- | --- | --- | --- |
| Battery chemistry / max voltage | Choose or narrow before schematic | Determines regulator input range and BAT_MON divider. | Open |
| 3.3 V output | Regulated 3.3 V | MCP3208 and analog front-end are planned for 3.3 V operation. | Provisional |
| Current budget for analog/ADC only | Sum MCP3208 + INA/op-amp + VREF buffer + support loads | Option A regulator can be smaller if it powers analog/ADC only. | Open |
| Optional current budget for analog/ADC + DSTK22807 | Include module active/BLE current | Required if Option B is selected later. | Open |
| Low dropout if 1S LiPo | Prefer low dropout | 1S LiPo voltage approaches 3.3 V during discharge. | Open |
| Low noise | Preferred | ADC reference and EMG analog front-end are noise-sensitive. | Open |
| Stable with output capacitor | Must follow exact regulator datasheet | LDO instability can mimic sensor noise. | Open |
| Reverse current / backfeed behavior | Must be understood | Prevents unsafe module/USB/rail backfeed. | Open |
| Thermal margin | Required | Depends on input voltage, load current, package, and copper. | Open |
| Hand assembly / package | Prefer hand-assembly-friendly where practical | Prototype should be reworkable. | Preference only |
| Enable pin | Optional | Can support power sequencing or switched loads. | Optional |
| Input protection / fuse / switch | Optional but recommended for battery prototype | Helps protect against wiring mistakes and shorts. | Open |

## 8. Current budget planning

Do not use this table as final current values. It is a planning checklist.

| Load | Current source | Estimated/current TBD | Notes |
| --- | --- | --- | --- |
| MCP3208 | MCP3208 datasheet / measurement | TBD | Depends on sample rate, VDD, and activity. |
| INA333 / instrumentation front-end | TI datasheet / measurement | TBD | One or two channels depending final analog plan. |
| MCP6002/MCP6004 or op-amp stages | Microchip datasheet / measurement | TBD | Includes filter, rectifier, envelope, and VREF buffer if shared. |
| analog VREF buffer | Op-amp/reference datasheet / measurement | TBD | Load includes INA REF, bias networks, VREF_MON, and probes. |
| BAT_MON divider if always-on | Resistor calculation | TBD | Depends on divider values and battery voltage. |
| DSTK22807 active/BLE | Bench measurement required | TBD | Must be measured if the same battery regulator will power it. |
| LEDs or indicators if present | Schematic/board observation | TBD | Include any power LED or user indicator current. |

Exact DSTK22807 current must be measured if the same battery regulator will power it.

Regulator selection cannot be finalized until the current budget is closed.

## 9. Battery choice implications

| Battery option | Pros | Risks | Regulator implication | Status |
| --- | --- | --- | --- | --- |
| 1S LiPo / Li-ion | Compact, common for wearables, high energy density | Charger/protection/safety handling required; voltage varies from full to discharged state | Needs low-dropout 3.3 V regulator or careful discharge cutoff; BAT_MON divider based on max charge voltage | Possible, not locked |
| 2xAA / 2xAAA | Easy to source, safer handling than bare Li-ion | Voltage can be below/near 3.3 V depending chemistry/discharge; larger | May need buck-boost or accept reduced operating range | Possible |
| 9V battery | Easy bench source | Poor capacity/current for digital radio loads; high LDO dissipation | Needs regulator with thermal margin; inefficient for 3.3 V loads | Not preferred |
| External protected battery pack | Can include protection and connector convenience | Pack voltage/current capability varies; still needs safety review | Regulator input range and BAT_MON divider depend on pack | Possible |

Notes:

- 1S LiPo is possible but not locked.
- Battery charger is out of scope unless explicitly added later.
- Human-contact safety still requires isolated battery-only operation.
- BAT_MON divider depends on maximum battery voltage.

## 10. 5V pin policy

- 5V pin is observable as 5.126 V when USB powered.
- 5V pin is not required for intended analog/ADC supply.
- 5V pin as analog/ADC supply: **NOT APPROVED**.
- 5V pin may be useful only as an observation point or possible module input if later proven, but not for the EMG analog chain.
- If any 5V rail is used later, ADC input scaling and ESP32-H2 level safety must be reviewed.

## 11. USB policy

- USB Type-C is allowed only for firmware/bench work with no human electrodes.
- USB Type-C must be disconnected for human-connected EMG tests.
- USB + external 3.3 V simultaneous connection: **NOT APPROVED**.
- If simultaneous connection is ever required, add power-path protection or an explicit isolation strategy first.

## 12. Decision status

- Full schematic implementation: **NO**.
- Small power schematic block implementation: **NO**, pending this plan review and at least one follow-up value-level review.
- Documentation/planning: **YES**.
- 3V3 external powering of DSTK22807: **NOT APPROVED YET**.
- USB-powered human EMG testing: **FORBIDDEN**.
- 5V pin as analog/ADC supply: **NOT APPROVED**.
- Battery-powered isolated operation for human-contact testing: **REQUIRED**.

## 13. Blockers before first schematic power block

- Battery chemistry and max voltage.
- Regulator exact candidate/suffix/package.
- Current budget for analog/ADC.
- Decision whether regulator powers DSTK22807 or not.
- DSTK22807 external 3.3 V input safety test result.
- DSTK22807 current draw under active/BLE operation if regulator powers it.
- USB/external 3.3 V backfeed policy.
- analog VREF buffer value-level design.
- ADC_REF filtering/decoupling value-level design.
- ADC input protection/source impedance values.
- BAT_MON divider ratio.
- Human-test safety procedure.

## 14. When schematic work may begin

- Full board schematic: **NOT YET**.
- First allowed schematic step later: small isolated power/reference schematic block only.
- That small schematic block may begin only after:
  1. This battery/regulator/power-path plan is reviewed and committed.
  2. Battery chemistry is chosen or narrowed enough for regulator/BAT_MON calculations.
  3. Regulator candidate and current budget are reviewed.
  4. analog VREF buffer topology and ADC_REF decoupling are reviewed.
  5. ADC input protection/BAT_MON/unused channel bias strategy has value-level candidates.
- Analog EMG chain schematic should not begin until 3.3 V headroom/gain calculation is complete.

## 15. Recommended next action

- Review and commit this document first.
- Then create a value-level power/reference design review document covering:
  - exact regulator candidate/suffix/package,
  - battery option,
  - VREF divider/buffer values,
  - ADC_REF decoupling/filtering values,
  - ADC input protection values,
  - BAT_MON divider values,
  - unused ADC bias values.

Final decision: **BATTERY_REGULATOR_POWER_PATH_PLAN_READY_FOR_REVIEW**
