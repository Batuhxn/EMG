# V5 Power / Reference Value-Level Design Review

> Historical review notice: this value-level candidate review predates the implemented V5 power and SPI architecture and is not the current source of truth. Current component connections and boundaries are defined by `STATUS_V5.md`, `DECISIONS_V5.md`, and checkpoint `bbe257e2c9dc28f7537942b5f15370770654ba19`.

## 1. Current repo state

| Item | State |
| --- | --- |
| Project path | `C:\Users\Batuhan\Desktop\EMG_v4_gelistirilmis_paket` |
| Branch | `v5/measured-dstk22807-footprint` |
| Git status before writing | Clean |
| Remote | `github-emg https://github.com/Batuhxn/EMG.git` |
| Scope | Value-level review only. No schematic implementation, PCB change, footprint edit, firmware change, gerber, production output, commit, or push. |

Last 12 commits at review start:

```text
c3f29e9 document battery regulator power path plan
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

Official manufacturer evidence used:

- Microchip MCP3204/3208 datasheet / product page.
- Microchip MCP1700 product page / datasheet source.
- Microchip MCP1702 datasheet / product page.
- Microchip MCP6002 product page / datasheet source.
- TI INA333 datasheet source.
- TI TLV755P datasheet / product page.

## 2. Purpose and boundary

This document reviews value-level candidates for the first V5 power/reference block.

It does not implement a schematic.

It does not approve the full board schematic.

It does not approve the analog EMG chain schematic.

It does not approve DSTK22807 external 3.3 V powering.

It does not approve USB Type-C plus external 3.3 V simultaneous connection.

It does not approve 5V as the analog/ADC supply.

This is not a final BOM lock. It is a candidate review intended to make the next schematic-only power/reference block safer and smaller.

## 3. Proposed first power/reference block scope

The maximum scope of the first possible schematic block later is:

- Battery input placeholder.
- 3.3 V regulator candidate.
- `3V3` rail.
- `3V3_ADC` / `ADC_REF` decoupling candidate.
- MCP3208 `VDD` and `VREF` decoupling.
- `analog VREF = 1.65 V` divider + buffer candidate.
- `VREF_MON` connection candidate.
- `BAT_MON` divider candidate.
- Safe unused ADC channel bias candidate.
- Test points.
- Safety notes.

This is the maximum scope of the first possible schematic block later. It does not include the full EMG analog chain. It does not include final PCB layout.

## 4. Battery assumption for calculations

Primary calculation assumption: 1S LiPo/Li-ion external protected battery.

- Nominal voltage: around 3.7 V.
- Full-charge voltage: 4.2 V.
- Discharge cutoff / minimum usable voltage: TBD.
- Charger remains out of scope unless explicitly added later.

| Battery assumption | Voltage range | Pros | Risks | Status |
| --- | --- | --- | --- | --- |
| 1S protected LiPo/Li-ion | 4.2 V full, about 3.7 V nominal, cutoff TBD | Compact, common for wearable/prototype battery systems | Needs protection, cutoff, safe charging policy; LDO dropout matters near low battery | Primary calculation assumption, not locked |
| 2xAA / 2xAAA | Chemistry-dependent; can be above or below 3.3 V | Easy sourcing and safer handling than bare Li-ion | May require buck-boost or reduced usable range | Possible alternative |
| 9V battery | About 9 V fresh | Convenient bench source | Inefficient for 3.3 V, poor current suitability for radio/module loads | Not preferred |
| External protected battery pack | Pack-dependent | Can include protection and connector convenience | Voltage and current capability vary; BAT_MON must be recalculated | Possible |

## 5. 3.3 V regulator value-level candidate

Official-source observations:

- MCP1702 is a 250 mA low quiescent-current LDO family with 2.7 V to 13.2 V input range, typical 2.0 uA quiescent current, 1 uF ceramic input/output capacitor application, and TO-92/SOT package options in the datasheet.
- TLV755P is a 500 mA LDO family with 1.45 V to 5.5 V input range, 1 uF minimum input and output capacitor recommendations, and small SMD package options. It gives more current margin if DSTK22807 later shares the 3.3 V rail.
- MCP1700 remains a low-current candidate family, but exact suffix/package/current/noise/stability must be checked before use.

| Regulator Candidate | Example Variant | Package | Current | Dropout / IQ / Noise Notes | Output Cap Requirement | Pros | Risks | Status |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| MCP1702 family | 3.3 V suffix to be selected | TO-92, SOT-23A, SOT-89 class options | 250 mA family | Low IQ class; dropout must be checked against 1S LiPo low-voltage condition | 1 uF ceramic input/output typical application; stability per datasheet | Good hand-assembly candidate if TO-92; enough for analog/ADC-only with margin | May be marginal if powering DSTK22807; dropout can reduce usable 1S battery range | Primary analog/ADC-only candidate, not locked |
| MCP1700 family | 3.3 V suffix to be selected | TO-92/SOT class options depending exact MPN | Exact current TBD by suffix/datasheet | Low-IQ LDO candidate | Exact output cap stability must be checked | Simple low-power analog/ADC rail candidate | Current margin may be insufficient; exact evidence still needed | Secondary candidate, not locked |
| TLV755P family | 3.3 V fixed output variant to be selected | SOT-23-5 / WSON / X2SON class options | 500 mA family | Better current margin; dropout/noise/thermal must be checked for chosen package | Minimum 1 uF input and output caps per datasheet table | Better if analog/ADC + DSTK22807 later share rail | SMD only; hand assembly harder; backfeed behavior still not solved | Higher-current candidate, not locked |

Do not approve one final regulator yet. For analog/ADC-only Option A, MCP1702-class looks practical. If Option B later powers DSTK22807 from the same rail, TLV755P-class current margin may be more appropriate, but DSTK22807 current draw is still TBD.

## 6. Current budget value-level estimate

| Load | Candidate Current | Evidence / Source | Notes | Status |
| --- | --- | --- | --- | --- |
| MCP3208 | Sub-mA class; datasheet active max at 5 V is 400 uA | Microchip MCP3208 datasheet | Depends on sample rate, VDD, clocking, and shutdown behavior | Candidate estimate |
| INA333 front-end | TBD | TI INA333 datasheet and future measurement | One or two channels depending final analog plan | Open |
| MCP6002/MCP6004-class op-amp stages | TBD | Microchip op-amp datasheet and stage count | Includes filters, rectifier/envelope, and maybe VREF buffer | Open |
| analog VREF buffer | TBD | Selected op-amp/reference datasheet | Load depends on REF pins, bias networks, VREF_MON, test point, unused ADC bias | Open |
| BAT_MON divider if always-on | 0.98 uA to 9.77 uA for listed divider candidates | Resistor calculation | Very small but continuous battery drain | Candidate calculation |
| Unused ADC bias resistors | Example: 1.65 V / 100k = 16.5 uA per channel if tied to VREF through 100k | Resistor calculation | Many low-value ties can load analog VREF | Candidate calculation |
| Power LEDs/indicators if any | TBD | Schematic/board observation | Can dominate analog current if not controlled | Open |
| DSTK22807 active/BLE current | Must be measured; do not invent final value | Bench measurement required | Required if the same regulator powers DSTK22807 | Open blocker |

Analog/ADC-only regulator current is likely much smaller than analog/ADC + DSTK22807. Regulator final choice cannot be locked until DSTK22807 current is measured if Option B is desired.

## 7. analog VREF divider + buffer value candidates

Goal: generate `analog VREF` around 1.65 V from 3.3 V. This is not `ADC_REF`. `analog VREF` should be buffered. Passive divider alone is not preferred.

Equal-divider calculations:

| Divider Values | VREF Target | Divider Current | Thevenin Resistance | Pros | Risks | Status |
| --- | --- | --- | --- | --- | --- | --- |
| 10k / 10k | 1.65 V | 165 uA | 5 kOhm | Strong divider, lower sensitivity to leakage before buffer | Higher continuous battery drain | Viable, not preferred for lowest power |
| 47k / 47k | 1.65 V | 35.1 uA | 23.5 kOhm | Good power/noise/loading compromise before buffer | Still needs buffer; cap/load stability must be checked | Provisional preferred candidate |
| 100k / 100k | 1.65 V | 16.5 uA | 50 kOhm | Lower battery drain | More leakage/noise/probe sensitivity before buffer | Viable only with careful buffer/layout |

| VREF Buffer Candidate | Candidate Values / Components | Pros | Risks | Status |
| --- | --- | --- | --- | --- |
| Spare MCP6002/MCP6004-class RRIO op-amp buffer | 47k/47k divider into unity-gain buffer; optional small output isolation resistor, value TBD | Cheap, BOM-aligned, low output impedance for bias loads | Must verify 3.3 V operation, output swing around 1.65 V, and capacitive-load stability | Preferred candidate, not locked |
| Dedicated low-noise op-amp buffer | Same divider, separate buffer part | Better control of noise/stability if spare op-amp is not suitable | Adds BOM/cost and package work | Backup candidate |
| Precision reference + buffer | 1.65 V reference or derived midpoint | Better absolute voltage accuracy | EMG midpoint may not require high absolute accuracy; adds cost | Not preferred now |

Provisional candidate: 47k/47k divider into a verified 3.3 V RRIO op-amp buffer, with local decoupling and load/stability review. If 100k/100k is used, leakage/noise sensitivity rises. If 10k/10k is used, battery drain rises.

## 8. analog VREF load check

| Load | Expected Loading | Risk | Required Check |
| --- | --- | --- | --- |
| INA333 REF pins | High impedance expected, exact value TBD | REF bias movement creates output offset | Datasheet and stage simulation |
| Op-amp bias networks | Depends on filter topology | Low-value bias networks can load VREF | Value-level filter review |
| Filter bias references | Depends on RC values | VREF noise couples into signal path | Noise and impedance review |
| VREF_MON ADC input | Small dynamic load through ADC sampling | Can inject sampling disturbance into VREF | Series resistor/acquisition review |
| Test point / probe | Probe-dependent | 1x probe or DMM loading can disturb high impedance references if unbuffered | Use buffered VREF and label test method |
| Unused ADC bias tied to analog VREF | Example 100k per channel if used | Many low-value ties increase buffer load | Sum load current and choose resistor values |

Do not tie many unused ADC channels to analog VREF with low-value resistors. VREF_MON should not meaningfully load analog VREF. Probe/test point can disturb high impedance references if unbuffered.

## 9. ADC_REF and MCP3208 decoupling/filtering values

Goal: MCP3208 `VDD = 3.3 V` candidate. MCP3208 `VREF` pin is `ADC_REF`, a clean/filtered 3.3 V candidate. `ADC_REF` is not `analog VREF`.

| Net / Pin | Candidate Value | Purpose | Risk | Status |
| --- | --- | --- | --- | --- |
| MCP3208 VDD | 100 nF ceramic close to pin | High-frequency local bypass | Poor placement couples SPI/sampling current into readings | Recommended |
| MCP3208 VDD bulk | 1 uF or 4.7 uF near ADC | Local energy storage | Too large is usually okay on rail but must not destabilize regulator branch | Candidate |
| MCP3208 VREF / ADC_REF | 100 nF close to VREF pin | Local reference bypass | Reference noise directly affects conversion result | Recommended |
| ADC_REF bulk | Optional 1 uF near ADC_REF | Stabilize reference node | Too much source impedance/RC may degrade dynamic conversion behavior | Candidate, not locked |
| ADC_REF series element | Optional small resistor/ferrite only after review | Isolate rail noise | Excess series resistance can hurt reference behavior | Open |
| Ground return | Short controlled return to common GND | Reference/sampling return integrity | Split/long return path creates measurement error | Required layout constraint |

Too much series resistance in the VREF path can degrade conversion behavior. SPI noise and sampling current can couple into readings if decoupling/layout is poor. Final layout must keep ADC input returns and reference returns controlled.

## 10. MCP3208 ADC input protection and source impedance value candidates

Every CH0-CH7 input must stay between VSS and ADC_REF and must not float. Source impedance/acquisition time must be compatible with MCP3208 sampling. MCP3208 has an internal sample/hold path, so high source impedance and large capacitors need calculation.

Candidate series resistor options: 100 ohm, 330 ohm, 1k, 4.7k.

Candidate capacitor-to-ground options near ADC: 100 pF, 1 nF, 4.7 nF, 10 nF.

| ADC Channel | Net | Candidate R | Candidate C | Bias / Protection Strategy | Risk | Status |
| --- | --- | --- | --- | --- | --- | --- |
| CH0 | EMG1_RAW | 330 ohm to 1k footprint | 100 pF to 1 nF optional | Modest series protection, optional anti-kickback/noise C | Too much RC can slow settling | Candidate, not locked |
| CH1 | EMG1_RECT | 330 ohm to 1k footprint | 100 pF to 1 nF optional | Same as CH0 if populated; defined bias if not | Floating if RECT not populated | Open |
| CH2 | EMG1_ENV | 330 ohm to 1k footprint | 1 nF to 10 nF optional depending envelope impedance | Envelope output may already be slow | High source impedance can affect ADC acquisition | Open |
| CH3 | EMG2_RAW | 330 ohm to 1k footprint | 100 pF to 1 nF optional | Future channel; bias if not populated | Future input can float | Open |
| CH4 | EMG2_RECT | 330 ohm to 1k footprint | 100 pF to 1 nF optional | Future channel; bias if not populated | Future input can float | Open |
| CH5 | EMG2_ENV | 330 ohm to 1k footprint | 1 nF to 10 nF optional | Future channel; bias if not populated | Future input can float | Open |
| CH6 | VREF_MON | 330 ohm to 1k footprint | 100 pF to 1 nF optional | Buffered analog VREF through small series resistor | Must not load or disturb analog VREF | Candidate |
| CH7 | BAT_MON | 1k to 4.7k footprint after divider | 1 nF to 10 nF optional | Scaled divider plus RC/protection | Divider source impedance may be high | Open blocker |

Recommendation: use modest series resistor footprints and optional small C footprints near the ADC. Do not lock values until acquisition/source impedance calculation is completed. For VREF_MON, use buffered analog VREF through a small series resistor, not a direct heavy load. For BAT_MON, use a scaled divider and possibly RC.

## 11. BAT_MON divider value candidates

Calculation assumption only: 1S LiPo max = 4.2 V and ADC_REF = 3.3 V. Need BAT_MON <= ADC_REF at max battery. Divider formula: `Vout = Vin * Rbottom / (Rtop + Rbottom)`.

| Rtop | Rbottom | Vout at 4.2 V | Divider Current | Source Impedance | Pros | Risks | Status |
| --- | --- | --- | --- | --- | --- | --- | --- |
| 1M | 3.3M | 3.223 V | 0.977 uA | 767 kOhm | Very low battery drain, uses common-ish ratio | Very high source impedance; ADC acquisition likely needs delay/RC/buffer strategy | Candidate, risky |
| 470k | 1.5M | 3.198 V | 2.132 uA | 358 kOhm | Low drain, good headroom below 3.3 V | Still high source impedance | Candidate |
| 330k | 1M | 3.158 V | 3.158 uA | 248 kOhm | More ADC-friendly than megaohm option | More drain than higher values, still not low source impedance | Candidate preferred starting point |
| 100k | 330k | 3.223 V | 9.767 uA | 76.7 kOhm | Lower source impedance, easier acquisition | Higher always-on battery drain | Candidate if always-on drain acceptable |

Divider ratio must also consider charger/protection maximum voltage if charger is later added. High-value dividers save current but may need acquisition delay, buffer, or RC strategy. A switched divider can save battery but adds GPIO/FET complexity. Do not finalize BAT_MON until battery choice and acquisition strategy are closed.

## 12. Safe unused ADC channel bias values

If RECT/ENV or second EMG channel outputs are not populated in the first prototype, ADC channels must not float.

| Strategy | Candidate Value | Pros | Risks | Recommendation |
| --- | --- | --- | --- | --- |
| Tie unused channel to analog VREF through resistor | 100k | Reads midscale, diagnostic-friendly | Loads VREF buffer; many channels add current | Good if VREF buffer load is checked |
| Tie unused channel to GND through resistor | 100k | Simple, defined zero | Firmware must know channel is unused | Acceptable for future-only inputs |
| Jumper/solder bridge to choose analog VREF or GND | 100k selected path | Flexible during bring-up | More PCB area and assembly documentation | Preferred prototype strategy if space allows |
| Optional DNI bias footprint | 100k placeholder | Future-friendly | Floating if DNI not populated by mistake | Acceptable with clear assembly notes |
| Direct tie without resistor | 0 ohm / direct | Simple | Can fight future populated stages or overload VREF | Not recommended |

Recommendation: for prototype, prefer jumper/solder-bridge or high-value resistor-defined bias. Avoid direct tie without resistor. Avoid loading analog VREF with many low-value resistors.

## 13. Test points and bring-up hooks

| Test Point | Purpose | Risk / Note | Status |
| --- | --- | --- | --- |
| BAT+ | Battery/regulator input measurement | Do not probe on human-connected setup with earth-referenced equipment | Recommended |
| 3V3 | Main regulated rail | Useful for regulator bring-up | Recommended |
| 3V3_ADC | ADC/local branch | Confirms ADC branch/filter drop | Recommended if branch exists |
| ADC_REF | MCP3208 full-scale reference | Do not confuse with analog VREF | Recommended |
| analog VREF | Midscale bias | Probe can disturb if unbuffered | Recommended, buffered only |
| AGND/DGND/common GND | Return reference | Avoid creating split-ground confusion | Recommended |
| EMG1_RAW | Main analog output to ADC | No human + USB/earth probing | Recommended later |
| EMG1_RECT | Rectified diagnostic node | Only if populated | Optional |
| EMG1_ENV | Envelope diagnostic node | Only if populated | Optional |
| VREF_MON | ADC channel monitor | Confirms digital offset reference | Recommended |
| BAT_MON | Battery divider output | Verify divider scaling before ADC | Recommended |
| ADC_CS/SCLK/MOSI/MISO | SPI debug | Digital probing can inject noise near analog | Optional |

Test points are useful for bring-up, but human-connected tests must not use mains/earth-referenced equipment. Measurement equipment policy must be respected.

## 14. 5V and USB policy confirmation

- 5V pin as analog/ADC supply: **NOT APPROVED**.
- USB Type-C with human electrodes: **FORBIDDEN**.
- USB + external 3.3 V simultaneous connection: **NOT APPROVED**.
- External 3.3 V powering of DSTK22807: **NOT APPROVED YET**.
- If later using MCP3208 at 5V, DOUT level shifting is required before ESP32-H2. Current plan is MCP3208 at 3.3 V.

## 15. Value-level provisional recommendation

- Battery assumption: 1S protected LiPo/Li-ion for calculations only, not locked.
- Regulator: MCP1702-class is the primary analog/ADC-only candidate; TLV755P-class is a higher-current candidate if DSTK22807 later shares the rail. Exact suffix/package not locked.
- analog VREF: equal resistor divider + op-amp buffer, with 47k/47k as the current value-level candidate.
- ADC_REF: clean 3.3 V with local decoupling, no merge with analog VREF.
- ADC inputs: modest series resistor plus optional RC footprints, values TBD after acquisition review.
- BAT_MON: 330k/1M or 100k/330k style divider candidates for 4.2 V max, not final.
- Unused ADC channels: high-value resistor or jumper-defined bias.
- Test points: include power/reference/ADC diagnostic points.
- Safety: battery-only isolated operation for human contact.

## 16. Blockers before small schematic power/reference block

- Battery choice narrowed enough for regulator and BAT_MON.
- Exact regulator candidate/suffix/package chosen.
- Current budget for analog/ADC estimated.
- Decision whether DSTK22807 is excluded from first power block or included later.
- DSTK22807 external 3.3 V powering test result, if Option B is considered.
- analog VREF buffer candidate verified at 3.3 V.
- ADC_REF decoupling/filter values accepted.
- ADC input source impedance/acquisition calculation accepted.
- BAT_MON divider values accepted.
- Unused ADC bias values accepted.
- Human-test safety procedure accepted.

## 17. Should schematic work begin after this document?

- Full board schematic: **NO**.
- Analog EMG chain schematic: **NO**.
- Small power/reference schematic block: **MAYBE AFTER REVIEW**, if this document is reviewed, any needed corrections are committed, and value-level blockers are accepted.
- If accepted, the next implementation step should be limited to a small schematic-only power/reference block proposal, not PCB layout.

## 18. Recommended next action

- Review this document.
- If acceptable, commit it.
- Then update `DECISIONS_V5.md` in a separate commit to reference this value-level review.
- Only after that, decide whether to allow a small schematic-only power/reference block proposal.

Final decision: **POWER_REFERENCE_VALUE_LEVEL_REVIEW_READY_FOR_REVIEW**
