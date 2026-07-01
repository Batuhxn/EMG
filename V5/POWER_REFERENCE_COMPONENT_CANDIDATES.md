# V5 Power / Reference Component Candidate Review

## 1. Current repo state

| Item | State |
| --- | --- |
| Project path | `C:\Users\Batuhan\Desktop\EMG_v4_gelistirilmis_paket` |
| Branch | `v5/measured-dstk22807-footprint` |
| Git status before writing | Clean |
| Scope | Documentation/review only. No schematic, PCB, footprint, firmware, gerber, or production file changes. |

Last 8 commits at review start:

```text
83768c0 restore V5 decisions log
cdc9f74 document V5 power reference architecture review
2924677 document MCP3208 MPN symbol footprint review
8a252b4 document DSTK22807 SPI pin candidates
f6e2b14 document observed DSTK22807 pinout mapping
3824d20 clean up DSTK22807 carrier footprint silkscreen
b4ba0e9 add measured provisional DSTK22807 carrier footprint
b7384a1 emergency baseline from transferred EMG project
```

Official evidence used in this review:

- Microchip MCP3204/3208 datasheet: `https://ww1.microchip.com/downloads/aemDocuments/documents/APID/ProductDocuments/DataSheets/21298e.pdf`
- Microchip MCP1700 product page / datasheet link: `https://www.microchip.com/en-us/product/MCP1700`
- Microchip MCP1702 product page / datasheet link: `https://www.microchip.com/en-us/product/MCP1702`
- Microchip MCP6002 product page / datasheet link: `https://www.microchip.com/en-us/product/MCP6002`
- TI INA333 datasheet: `https://www.ti.com/lit/ds/symlink/ina333.pdf`
- TI TLV755P product page / datasheet link: `https://www.ti.com/product/TLV755P`

Internet access was available. Only official manufacturer sources were used for component evidence. Exact BOM MPNs are not locked by this document.

## 2. First prototype power-source assumptions

The first V5 digital prototype should remain battery-first and isolated for any human-electrode work.

| Assumption | Review | Status |
| --- | --- | --- |
| Battery input chemistry | Still TBD. 1S LiPo is possible because the draft BOM mentions external 1S LiPo charging only, but this is not locked here. | Open |
| Regulator input maximum | Depends on selected battery chemistry and whether charger/protection is on-board or external. | Open |
| Charger | Out of scope unless explicitly added later. | Not included |
| Human-connected tests | Must be battery-only and isolated. No USB, 5 V bench supply, mains-connected tools, or oscilloscope earth while electrodes are attached to a human. | Locked safety rule |
| DSTK22807 power direction | Do not assume the module `3V3` pin can power analog/ADC. Do not back-feed DSTK22807 until board behavior is verified. | Open blocker |

## 3. 3.3 V regulator candidate requirements

| Requirement | Target / Candidate | Reason | Status |
| --- | --- | --- | --- |
| Input voltage range | Compatible with selected battery, likely 1S LiPo range if that remains the battery choice | The regulator must survive full battery voltage and remain in regulation near end-of-discharge. | Open |
| Output voltage | 3.3 V | Matches MCP3208 3.3 V operation and ESP32-H2 SPI logic. | Provisional |
| Current capability | Enough for ADC + analog + possible DSTK22807 interface/module load | Regulator sizing changes if it powers only analog/ADC versus analog/ADC + module. | Open |
| Low noise | Preferred | EMG front-end and ADC reference are noise-sensitive. | Open |
| Low dropout | Preferred if using 1S LiPo | A 1S battery falls close to 3.3 V near discharge. High dropout loses usable battery range. | Open |
| Output capacitor stability | Must match regulator datasheet | LDO instability can look like sensor noise or rail oscillation. | Open |
| Reverse/backfeed behavior | Must be understood | Prevents accidental back-powering through DSTK22807 or IO rails. | Open |
| Thermal check | Required | LDO dissipation depends on battery voltage, load current, package, and copper area. | Open |
| Package / assembly | Prefer hand-assembly-friendly; through-hole if performance/current allow | First prototype should be reworkable, but electrical performance matters more than DIP/THT convenience. | Preference only |

Candidate regulator families, not final BOM locks:

| Candidate Regulator | Package | Output Current | Dropout / Noise Notes | Pros | Risks | Status |
| --- | --- | --- | --- | --- | --- | --- |
| Microchip MCP1702 3.3 V family | TO-92 / SOT packages depending variant | 250 mA family per Microchip product page | Low-IQ LDO class; dropout/noise/cap requirements must be checked in datasheet for exact suffix | Hand-assembly-friendly TO-92 options may exist; simple battery prototype candidate | 250 mA may be marginal if powering DSTK22807 module; not a final low-noise reference source; exact suffix/package not locked | Candidate, not locked |
| Microchip MCP1700 3.3 V family | TO-92 / SOT packages depending variant | Verify exact suffix in datasheet | Low quiescent current LDO class per Microchip product page | Simple, low-current analog/ADC rail candidate | Current may be too low for module-powered architecture; exact dropout/noise/cap stability must be checked | Candidate, not locked |
| TI TLV755P 3.3 V family | SOT23-5 / WSON / X2SON class packages | 500 mA per TI product page | Low-IQ LDO with ceramic-cap stability per TI product page | More current margin; good if one regulator powers more than ADC/analog | SMD, not through-hole; exact package may be harder to hand assemble; backfeed and thermal still need review | Candidate, not locked |
| Existing module/DSTK 3V3 pin | DSTK22807 board pin | Unknown | Unknown | Could reduce BOM if proven safe | Explicitly not approved as whole-system source until module behavior is verified | Blocked |

Important: this review does not decide whether the regulator powers DSTK22807. That decision is blocked by DSTK22807 `3V3`/`5V` behavior verification.

## 4. 3V3 rail split/filtering strategy

The preferred strategy is one main regulated `3V3` with careful local decoupling and optional quiet branches. Do not create confusing split grounds or long return paths.

| Rail | Candidate Source | Loads | Filtering | Risk | Status |
| --- | --- | --- | --- | --- | --- |
| `3V3` | External low-noise 3.3 V regulator | System rail candidate | Bulk cap near regulator, local caps at loads | Ownership unclear if DSTK22807 is separately powered or back-fed | Provisional |
| `3V3_ADC` | `3V3` branch near MCP3208 | MCP3208 `VDD`; possibly ADC reference filtering source | Local 100 nF plus 1 uF class decoupling; ferrite/RC only after impedance review | Ferrite/RC can create rail droop or return ambiguity if misused | Provisional |
| `analog VCC` | `3V3` or filtered analog branch | INA333, MCP6002/MCP6004, analog filters | Local op-amp/INA decoupling; optional small isolation resistor/ferrite only if stable | 3.3 V headroom not yet proven for old V4 values | Open |
| `3V3_MCU` / DSTK interface | DSTK pin or shared `3V3`, TBD | SPI logic reference / module power depending architecture | Avoid backfeed; series resistors on SPI may help edges/noise | Power direction unknown | Blocked |
| `ADC_REF` | Clean/filtered 3.3 V candidate | MCP3208 `VREF` pin | Local VREF decoupling, possible RC if acquisition/reference-drive requirements permit | Too much source impedance or noisy reference degrades conversion accuracy | Open |

## 5. MCP3208 decoupling and ADC_REF strategy

Official Microchip MCP3208 evidence relevant to this block: the ADC operates from 2.7 V to 5.5 V, uses SPI, has CH0-CH7 single-ended input range from VSS to VREF, and the sample/hold input includes switch resistance and sampling capacitance that make source impedance/acquisition time important.

| MCP3208 Pin/Net | Candidate Components | Requirement | Risk | Status |
| --- | --- | --- | --- | --- |
| `VDD` | 100 nF ceramic close to pin 16, plus 1 uF local bulk near ADC | Stable 3.3 V supply with short return to ground | Poor decoupling couples SPI and sampling current into analog readings | Candidate |
| `VREF` / `ADC_REF` | Clean 3.3 V reference feed, 100 nF close to pin 15, optional 1 uF after datasheet/source check | Defines ADC full-scale; must stay clean and within MCP3208 limits | Do not connect to 1.65 V analog VREF accidentally; RC source impedance can affect dynamic reference behavior | Candidate |
| `AGND` | Short return to common ground near analog/ADC area | Low-noise analog return | Blind ground split can break return paths | Candidate |
| `DGND` | Short return to common ground near SPI side | Digital return for SPI and ADC core | SPI currents should not cross high-impedance input area | Candidate |
| CH0-CH7 | Series resistor + small RC/protection per channel, values TBD | Inputs must remain from VSS to VREF in single-ended mode | Too high source impedance can cause acquisition error; too much capacitance can slow response | Open |
| SPI pins | Short routing, optional small series damping on SCLK/MOSI/CS if needed | Reliable 3.3 V SPI | Fast edges near EMG input can inject noise | Open |

`ADC_REF` is not `analog VREF`. `ADC_REF` is the MCP3208 full-scale reference, currently a clean/filtered 3.3 V candidate.

## 6. analog VREF buffer candidate requirements

`analog VREF` is the analog front-end midscale bias. For a 3.3 V analog supply, the provisional target is 1.65 V.

Passive divider alone is not preferred because INA REF pins, op-amp bias nodes, ADC monitoring, leakage, probing, and filter networks can move the midpoint. A buffer is strongly preferred.

| Topology | Example Components / Values | Pros | Risks | Status |
| --- | --- | --- | --- | --- |
| Divider + op-amp buffer | Two equal resistors from 3.3 V to GND, buffered by an existing spare MCP6002/MCP6004-class RRIO op-amp; add local decoupling | Simple, cheap, uses existing BOM direction, adjustable current/noise tradeoff | Exact resistor values and capacitor load stability need calculation; buffer must tolerate capacitive load and startup behavior | Preferred candidate |
| Precision/reference IC + buffer | 1.65 V reference if available, or 2.048 V/1.65 V-derived scheme with buffer | Better absolute accuracy if needed | Accuracy is probably less important than stable midpoint for EMG; adds BOM/cost; 1.65 V references may be less common | Possible, not preferred now |
| Rail-splitter IC | Dedicated virtual-ground / rail-splitter component | Purpose-built midpoint generator | Extra BOM; current/noise/stability must be verified; may be unnecessary at low analog load | Possible, not locked |
| Passive divider only | Two equal resistors plus capacitor | Cheapest | Output impedance too high and load-dependent; not preferred for shared analog bias | Not recommended |

Buffer requirements:

- Operates correctly from 3.3 V.
- Input/output common-mode compatible around 1.65 V.
- Low output impedance for INA REF/op-amp bias/VREF_MON/probing.
- Low enough noise for EMG baseline stability.
- Stable with selected decoupling and any capacitive load.
- Does not overload during startup or when ADC/probe/test points are attached.

## 7. Analog front-end supply compatibility

The analog front-end may run from 3.3 V, but the existing values must not be assumed valid if V4 or earlier circuits were tuned around 5 V.

| Device / Stage | 3.3 V Compatibility Question | Required Evidence | Status |
| --- | --- | --- | --- |
| INA333 | Does supply range, input common-mode, REF behavior, gain, and output swing support the intended EMG signal and electrode offset at 3.3 V? | TI datasheet operating limits plus gain/headroom calculation and/or simulation | Open blocker |
| MCP6002/MCP6004-class op-amp | Does the op-amp operate at 3.3 V and swing enough around 1.65 V for filter/rectifier/envelope stages? | Microchip datasheet limits plus stage simulation | Open blocker |
| High-pass / low-pass filters | Do cutoff frequencies and bias paths remain correct with 1.65 V midscale? | RC calculations and SPICE/TINA/LTspice check | Open |
| Rectifier stage | Does rectifier behavior work at small EMG amplitudes and 3.3 V supply? | Op-amp output swing and diode/active rectifier analysis | Open |
| Envelope stage | Does envelope output fit 0..3.3 V and settle correctly? | RC/gain calculation and transient simulation | Open |
| ADC drive | Can each analog output drive MCP3208 sampling input through protection/filter network? | MCP3208 acquisition/source-impedance calculation | Open |

## 8. MCP3208 input protection / source impedance

Every MCP3208 channel must be defined and protected. Single-ended ADC inputs must remain between VSS and `ADC_REF`.

| ADC Channel | Net | Candidate Protection / Bias | Risk | Status |
| --- | --- | --- | --- | --- |
| CH0 | `EMG1_RAW` | Small series resistor plus optional RC to ground near ADC; clamp/protection only after leakage review | Excess source impedance can cause sampling error; clamp leakage can corrupt EMG signal | Open |
| CH1 | `EMG1_RECT` | Same as CH0 if populated; safe bias if not populated | Floating channel if RECT is omitted | Open |
| CH2 | `EMG1_ENV` | Same as CH0 if populated; safe bias if not populated | Envelope source impedance may be high | Open |
| CH3 | `EMG2_RAW` | Protected input if second channel populated; defined bias/test jumper if future-only | Future channel can float or inject noise | Open |
| CH4 | `EMG2_RECT` | Protected input if populated; defined bias if future-only | Same as CH3 | Open |
| CH5 | `EMG2_ENV` | Protected input if populated; defined bias if future-only | Same as CH3 | Open |
| CH6 | `VREF_MON` | High-value tap or buffer output through small series resistor | Must not load analog VREF or add noise to the bias node | Recommended, values TBD |
| CH7 | `BAT_MON` | Divider from battery, series/RC/protection, ratio TBD | Battery/charger voltage can exceed ADC_REF if divider is wrong | Open blocker |

## 9. BAT_MON strategy

`BAT_MON` cannot be finalized until battery chemistry and maximum voltage are known.

| BAT_MON Option | Pros | Risks | Required Decision Before Schematic |
| --- | --- | --- | --- |
| Always-on high-value divider | Simple, low component count, continuously measurable | Wastes battery current; high impedance may increase ADC acquisition error | Battery max voltage, target divider current, ADC settling calculation |
| Switched divider controlled by MCU GPIO/FET | Saves battery current | Adds GPIO/control complexity and leakage/error paths; firmware dependency | Available GPIO, switch topology, startup default state |
| Divider plus RC filter | Reduces noise and sampling kickback | RC time constant can slow measurements; source impedance still matters | Measurement rate and ADC acquisition calculation |
| No BAT_MON in first schematic | Simplifies first bring-up | Loses battery visibility; CH7 still needs safe bias | Whether battery monitoring is required for V5 first prototype |

BAT_MON rule: at maximum battery and charger/protection condition, ADC channel voltage must stay <= `ADC_REF`.

## 10. Safe unused ADC channel strategy

If `RECT`, `ENV`, or second EMG channel outputs are not populated in the first prototype, their MCP3208 inputs must not float.

| Unused Channel Strategy | Pros | Risks | Recommendation |
| --- | --- | --- | --- |
| Tie unused channel to `analog VREF` through resistor | Lets firmware read midscale on unused channels; good diagnostic baseline | Too many low-value ties can load VREF buffer | Good candidate if resistor values are high enough and buffer load is checked |
| Tie unused channel to GND through resistor | Simple and unambiguous zero reading | Firmware must distinguish unused-zero from real signal; not centered like EMG nodes | Acceptable for truly unused channels |
| Test jumper / solder bridge to choose bias | Flexible for future population | More PCB area and documentation burden | Good for prototype if space allows |
| Leave no-load but with optional bias footprint | Future-friendly | Channel can float if bias DNI is forgotten | Only acceptable if assembly notes are strict |
| Direct tie to VREF or GND without resistor | Simple | Can fight future populated circuits or overload bias during mistakes | Not recommended |

Recommended prototype direction: use resistor-defined bias or jumper-selectable bias for future channels, with values selected after MCP3208 source impedance/acquisition review.

## 11. Human safety implications

- No human-connected electrodes with USB power.
- No human-connected electrodes with 5 V bench supply power.
- No human-connected electrodes with mains-connected tools.
- No oscilloscope earth connection to a human-connected circuit unless a complete isolation/safety setup is defined.
- Battery-only validation first.
- Use dummy load, resistor network, signal generator, and simulation before human contact.
- This is not a certified medical device.

## 12. Recommended provisional component architecture

This is not a final schematic, but the best provisional architecture is:

| Block | Provisional Architecture |
| --- | --- |
| Battery input | External battery input, chemistry TBD; 1S LiPo possible but not locked. Charger out of scope. |
| 3.3 V regulator | External 3.3 V low-noise/low-dropout LDO candidate; MCP1702/MCP1700/TLV755P families are candidates pending exact MPN/package/current/noise/thermal review. |
| `3V3_ADC` filtering | Main `3V3` branch with local MCP3208 decoupling; optional ferrite/RC only after return-path and impedance review. |
| MCP3208 `VDD` | 3.3 V preferred for ESP32-H2 SPI compatibility. |
| `ADC_REF` | Clean/filtered 3.3 V candidate with local VREF decoupling. Must remain separate from analog VREF. |
| `analog VREF` generation | 3.3 V divider plus op-amp buffer is the preferred low-cost candidate. Exact resistor/cap/buffer choice TBD. |
| `analog VCC` | 3.3 V candidate, pending INA333/op-amp headroom and gain validation. |
| ADC input protection | Per-channel series resistor/RC/protection footprints, values TBD after acquisition/source-impedance calculation. |
| `BAT_MON` | Divider/scaling/protection TBD after battery chemistry and maximum voltage are known. |
| Unused ADC channels | Defined resistor or jumper-selectable bias; no floating MCP3208 inputs. |
| Safety status | Planning only; no human test on USB/5 V/mains-connected equipment. |

## 13. Blockers before schematic

- Battery chemistry and maximum voltage.
- Regulator exact MPN and package.
- DSTK22807 `3V3` / `5V` behavior.
- INA333/op-amp 3.3 V headroom calculation.
- `analog VREF` buffer exact topology/part.
- MCP3208 source impedance/acquisition calculation.
- `BAT_MON` divider ratio.
- ADC input protection values.
- Safe unused ADC bias values.
- Physical DSTK22807 pinout second confirmation.
- Human-test safety procedure.

## 14. Should schematic implementation proceed?

Full schematic implementation: **NO**.

Small schematic block proposal: **NO** until component candidates and values are reviewed.

Documentation/planning: **YES**.

Final decision: **POWER_REFERENCE_COMPONENT_CANDIDATES_READY_FOR_REVIEW**
