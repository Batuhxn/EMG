# V5 Power / Reference Architecture Review

> Historical review notice: this proposed architecture predates the implemented V5 power and SPI architecture and is not the current source of truth. Its provisional rail and carrier assumptions are superseded by `STATUS_V5.md`, `DECISIONS_V5.md`, and checkpoint `bbe257e2c9dc28f7537942b5f15370770654ba19`.

## 1. Current repo state

| Item | State |
| --- | --- |
| Project path | `C:\Users\Batuhan\Desktop\EMG_v4_gelistirilmis_paket` |
| Branch | `v5/measured-dstk22807-footprint` |
| Git status before writing | Clean |
| Last checked commit | `2924677 document MCP3208 MPN symbol footprint review` |
| Scope of this document | Planning/review only; no schematic, PCB, footprint, firmware, gerber, or production file changes. |

Last 6 commits at review start:

```text
2924677 document MCP3208 MPN symbol footprint review
8a252b4 document DSTK22807 SPI pin candidates
f6e2b14 document observed DSTK22807 pinout mapping
3824d20 clean up DSTK22807 carrier footprint silkscreen
b4ba0e9 add measured provisional DSTK22807 carrier footprint
b7384a1 emergency baseline from transferred EMG project
```

Source documents reviewed:

| Source | Status | Notes |
| --- | --- | --- |
| `V5/DSTK22807_PHYSICAL_PINOUT_OBSERVATION.md` | Available | Observed 2x9 carrier pad mapping used as physical reference. |
| `V5/DSTK22807_SPI_PIN_REVIEW.md` | Available | SPI candidates selected from right-row GPIOs, avoiding TX/RX and known risky pins where possible. |
| `V5/MCP3208_MPN_SYMBOL_FOOTPRINT_REVIEW.md` | Available | First prototype ADC recommendation is `MCP3208-CI/P`, KiCad symbol `Analog_ADC:MCP3208`, footprint `Package_DIP:DIP-16_W7.62mm`. |
| `V5/BOM_V5_DRAFT.md` | Available | Draft BOM includes MCP3208, DSTK22807 carrier, battery input, low-noise 3.3 V regulator, and analog support parts. |
| `V5/STATUS_V5.md` | Available | V5 is planning / not fabrication-ready for expansion until schematic and layout updates are completed. |
| `V5/DECISIONS_V5.md` | Missing | Requested as an input, but not present in the repo at review time. This is a documentation gap before final schematic work. |

## 2. Proposed first-prototype power architecture

For the first digital-output EMG prototype, the safest provisional architecture is a battery-powered 3.3 V system with separate naming for noisy/quiet domains, while keeping one controlled ground system. The key decision is to avoid using the DSTK22807 module's `3V3` pin as the assumed source for the whole analog and ADC system until the carrier board power capability is verified.

| Rail / Net | Proposed role | Provisional source | Main loads / use | Review note |
| --- | --- | --- | --- | --- |
| `BAT+` | External battery input | Battery connector, chemistry TBD | 3.3 V regulator input and optional battery monitor divider | Battery chemistry, voltage range, connector, fuse/protection, and charger strategy are not closed. |
| `3V3` | Main regulated 3.3 V system rail | External low-noise battery-powered regulator preferred | ADC digital/analog supply candidate, analog front-end supply candidate, MCU interface rail | Do not assume DSTK22807 `3V3` can power analog + ADC until module documentation and current limit are confirmed. |
| `3V3_ADC` | Quiet local ADC supply label/filter output | Filtered branch from `3V3` if needed | MCP3208 `VDD`, local decoupling | Use ferrite/resistor only if layout/noise plan supports it; avoid creating return-path confusion. |
| `3V3_MCU` / DSTK `3V3` interface | MCU logic reference/interface rail | DSTK22807 module 3.3 V pin or shared regulated 3.3 V, TBD | SPI logic level reference | Must not back-power the module through IO or power pins. Final source direction must be documented before schematic. |
| `analog VCC` | Analog front-end supply | 3.3 V candidate | INA/op-amp/filter stages | Must be checked against INA333/MCP6002 headroom and output swing at intended gain. |
| `ADC_REF` | MCP3208 conversion full-scale reference | Clean/filtered 3.3 V candidate | MCP3208 `VREF` | This is not the analog midscale bias. It defines ADC input range in single-ended mode. |
| `analog VREF` | Analog midscale bias | Buffered 1.65 V candidate from `3V3` | INA/reference bias, op-amp bias, ADC monitor CH6 | Keep separate from `ADC_REF` in naming and function. |
| `AGND` | Quiet analog return naming | Board ground system | Analog front-end, ADC analog pins | Prefer placement/routing discipline and controlled returns, not blind ground splitting. |
| `DGND` | Digital return naming | Board ground system | SPI/header/module digital returns | Join to same solid ground system with deliberate return-current control. |
| `BAT_MON` | Scaled battery measurement | Divider from `BAT+` to ADC channel | MCP3208 CH7 | Divider ratio and impedance TBD; consider high-value or switched divider to save battery. |

## 3. MCP3208 supply/reference review

The selected provisional ADC family is MCP3208. The current review uses the earlier MPN decision: `MCP3208-CI/P` as a low-risk through-hole prototype part. It is acceptable for bring-up, with the caveat already recorded that C-grade INL is wider than B-grade. If accuracy margin becomes critical, consider `MCP3208-BI/P` or a compact `MCP3208-BI/SL` variant.

| MCP3208 pin/function | Recommended connection concept | Notes / risk |
| --- | --- | --- |
| `VDD` | `3V3_ADC` or clean `3V3` | Must be locally decoupled near the DIP-16 pins. Logic output high follows VDD, so this aligns with ESP32-H2 3.3 V IO. |
| `VREF` | `ADC_REF`, clean/filtered 3.3 V candidate | Defines ADC full-scale. Do not connect this to 1.65 V analog midscale unless intentionally reducing ADC input range. |
| `AGND` | Ground system, analog-side return near ADC/front-end | Keep ADC analog input returns short and quiet. Do not create a floating/split ground island. |
| `DGND` | Ground system, digital-side return near SPI/module | Tie into the same ground system with controlled placement. Keep SPI return close to SPI traces. |
| `CH0` | `EMG1_RAW` | First active EMG raw analog output input. Must remain within VSS to VREF in single-ended mode. |
| `CH1` | `EMG1_RECT` | Future/optional rectified signal. If not implemented, bias/tie safely; do not float. |
| `CH2` | `EMG1_ENV` | Future/optional envelope signal. If not implemented, bias/tie safely; do not float. |
| `CH3` | `EMG2_RAW` | Future second-channel raw input. If analog channel not populated, provide safe bias/test access. |
| `CH4` | `EMG2_RECT` | Future second-channel rectified input. Safe unused-state required. |
| `CH5` | `EMG2_ENV` | Future second-channel envelope input. Safe unused-state required. |
| `CH6` | `VREF_MON` | Measures analog midscale bias for digital correction/diagnostics. Input should be high impedance and protected. |
| `CH7` | `BAT_MON` | Measures scaled battery voltage. Divider must guarantee ADC input stays from VSS to VREF. |
| `DOUT` | `ADC_MISO` candidate from SPI review | ESP32-H2 input. 3.3 V compatible if MCP3208 VDD is 3.3 V. |
| `DIN` | `ADC_MOSI` candidate from SPI review | ESP32-H2 output to ADC. |
| `CLK` | `ADC_SCLK` candidate from SPI review | Route away from analog inputs; series damping can be considered if edges/noise become an issue. |
| `CS/SHDN` | `ADC_CS` candidate from SPI review | Keep defined during reset/boot; pull-up/pull-down choice should match firmware boot behavior. |

Important ADC input rule for schematic/layout: in single-ended mode, every ADC channel must be protected so the pin voltage never goes below VSS or above VREF, including startup, disconnected electrodes, battery transients, and unpopulated future channels.

## 4. Analog VREF review

The analog EMG chain needs a midscale bias so single-supply op-amps can represent positive and negative biological signal components. With a 3.3 V analog supply, the provisional analog midscale is `analog VREF = 1.65 V`, buffered before use.

| VREF use | Recommended source | Should it equal `ADC_REF`? | Notes |
| --- | --- | --- | --- |
| INA/reference bias | Buffered `analog VREF` around 1.65 V | No | Keeps INA and downstream signal centered in the analog supply range. |
| Op-amp bias / virtual ground | Buffered `analog VREF` around 1.65 V | No | The buffer must be stable with the attached capacitance and load. |
| ADC negative reference in differential math | Not a physical MCP3208 differential negative input for current plan | No | MCP3208 is used here as single-ended channel ADC; subtract `VREF_MON` digitally if needed. |
| ADC full-scale reference | `ADC_REF`, likely clean 3.3 V | Yes, for ADC only | MCP3208 `VREF` defines 0..VREF conversion span. |
| Diagnostic monitor | `CH6 = VREF_MON` | No | Allows firmware to subtract bias drift from EMG channels. |

This separation is intentional: `ADC_REF` is the ADC conversion scale, while `analog VREF` is the front-end signal center. Naming them separately avoids a common schematic mistake.

## 5. Analog VCC review

A 3.3 V analog supply is attractive because it keeps the MCP3208 input range and ESP32-H2 logic level simple. However, the analog chain must be checked before schematic implementation:

| Check item | Why it matters | Status |
| --- | --- | --- |
| INA333 supply range and output swing | The front-end must not saturate around 1.65 V bias with expected gain and electrode offset. | Open blocker before final schematic. |
| MCP6002 supply range and output swing | Filter/rectifier/envelope stages must fit within 0..3.3 V and preserve Arduino/ADC measurable output. | Open blocker before final schematic. |
| Gain budget | 0.5 mV EMG test signal plus electrode offset/noise must not rail the chain. | Needs calculation/simulation update for 3.3 V. |
| 50 Hz filtering | Analog notch/filter strategy must be explicit if required. | Not closed in this document; can be analog or digital/firmware. |
| Decoupling | Every IC supply pin needs local 100 nF class bypass plus sensible bulk capacitance. | Must be implemented in schematic/layout. |

Recommendation: use `analog VCC = 3.3 V` as the provisional candidate only after INA/op-amp headroom and gain are recalculated. If the existing V5 analog values were tuned for 5 V, the signal swing and VREF center must be revalidated.

## 6. BAT_MON review

`BAT_MON` is useful, but it is not just a wire from battery to ADC. It must be scaled and protected.

| BAT_MON item | Recommendation | Open detail |
| --- | --- | --- |
| Divider ratio | Choose after battery chemistry and maximum voltage are known | Must guarantee ADC input <= `ADC_REF` under full battery and charger conditions. |
| Divider impedance | Prefer high-value divider or switched divider | High impedance saves current but may need ADC acquisition settling support. |
| Protection | Add series resistance and clamp/RC strategy as needed | Avoid injecting battery transients into ADC input. |
| Calibration | Use firmware scaling from raw ADC count to battery voltage | Requires exact resistor values/tolerances. |

Until battery chemistry is defined, `BAT_MON` should be treated as planned but not final.

## 7. Channel input mapping impact

The current 8-channel MCP3208 plan reserves channels for one active EMG chain, future second channel support, diagnostics, and battery monitoring.

| ADC channel | Proposed net | Purpose | First prototype status | Notes |
| --- | --- | --- | --- | --- |
| CH0 | `EMG1_RAW` | Raw EMG analog output | Active | Main bring-up channel. |
| CH1 | `EMG1_RECT` | Rectified EMG signal | Optional/future | If circuit not populated, do not leave floating. |
| CH2 | `EMG1_ENV` | Envelope signal | Optional/future | Useful for easier MCU-side feature extraction. |
| CH3 | `EMG2_RAW` | Future second EMG raw channel | Future | Needs second analog front-end before real use. |
| CH4 | `EMG2_RECT` | Future second rectified signal | Future | Bias/tie safely if unused. |
| CH5 | `EMG2_ENV` | Future second envelope signal | Future | Bias/tie safely if unused. |
| CH6 | `VREF_MON` | Analog midscale monitor | Recommended | Lets firmware compensate VREF drift/offset. |
| CH7 | `BAT_MON` | Battery voltage monitor | Recommended after divider is defined | Requires divider/protection before schematic. |

Impact: using CH6 for `VREF_MON` means firmware can report EMG as `CH0 - CH6` or use CH6 for offset diagnostics. This is often more useful than tying MCP3208 `VREF` to analog midscale.

## 8. Grounding and safety

Grounding recommendation: keep one continuous low-impedance ground system. Use `AGND` and `DGND` as placement/routing intent labels if helpful, but avoid blind ground splits. The analog front-end, ADC input return, and VREF buffer should sit together. SPI traces and module currents should be routed so their return currents do not cross the high-impedance electrode/INA input area.

Safety status:

| Area | Recommendation |
| --- | --- |
| Human electrode testing | Do not connect electrodes to a human while the board is powered from USB, 5 V bench supplies, mains-connected instruments, or non-isolated equipment. |
| Prototype power | Battery-only operation is preferred for any human-contact experiment. |
| Measurement equipment | Use isolation-aware measurement practices; avoid connecting oscilloscope ground to a human-connected circuit unless the safety setup is known. |
| Medical status | This is not a certified medical device. The review is engineering planning only. |

## 9. Recommended provisional architecture

Recommended first-prototype architecture:

- Battery input: external battery input, chemistry TBD.
- Regulator: external battery-powered low-noise 3.3 V regulator preferred.
- `3V3`: system regulated 3.3 V. Do not assume DSTK22807 `3V3` can power the whole analog/ADC section.
- MCP3208 `VDD`: 3.3 V preferred.
- `ADC_REF`: clean/filtered 3.3 V candidate if ADC input range is 0..3.3 V.
- `analog VREF`: buffered 1.65 V midscale candidate, not `ADC_REF`.
- `analog VCC`: 3.3 V candidate, pending INA/op-amp headroom and gain validation.
- `AGND`/`DGND`: controlled return strategy on a common ground system; no blind split.
- `BAT_MON`: divider/scaling TBD; consider high-value or switched divider.
- Safety status: planning only; no human test on USB/5 V/mains-connected equipment.

## 10. Blockers before schematic implementation

| Blocker | Why it matters | Required closure before implementation |
| --- | --- | --- |
| Battery chemistry and maximum voltage TBD | Sets regulator input, BAT_MON divider, protection, and connector safety. | Choose battery type/range and whether charger is in-scope. |
| 3.3 V source ownership TBD | Prevents back-powering or overloading DSTK22807 module 3V3 pin. | Decide whether regulator powers module + analog, or module only interfaces to regulated rail. |
| INA333/MCP6002 headroom at 3.3 V not yet recalculated | Analog chain could saturate if reused from 5 V assumptions. | Recalculate/simulate gain, offsets, VREF, and output swing. |
| MCP3208 input protection not yet designed | ADC pins must never exceed VSS..VREF. | Add input resistors/RC/protection and safe bias for unused channels. |
| `BAT_MON` divider not yet defined | Battery voltage may exceed ADC range. | Define divider ratio, impedance, tolerance, optional switch, and RC. |
| `DECISIONS_V5.md` missing | Project decisions are not centralized. | Create or recover decision log before schematic changes. |
| Human-test safety plan not documented as hardware requirements | Human-connected electrodes create safety risk. | Define battery-only bring-up and measurement safety constraints. |
| Exact DSTK22807 power pin behavior not officially verified | Carrier 5V/3V3 behavior affects power architecture. | Verify physical module documentation or bench-measure before tying rails. |

## 11. Should schematic implementation proceed?

Full schematic implementation should proceed now: **NO**.

Documentation/planning should proceed now: **YES**.

A small schematic-only power block may be proposed later after the blockers above are closed, especially battery choice, 3.3 V source ownership, analog headroom at 3.3 V, and ADC input protection strategy.

Final decision: **POWER_REFERENCE_REVIEW_READY_FOR_REVIEW**
