# V5 Power / Reference Schematic Block Proposal

> Historical proposal notice: this document records a pre-implementation design proposal and is not the current source of truth. Its provisional carrier-pin and direct-SPI statements are superseded by the implemented architecture at `bbe257e2c9dc28f7537942b5f15370770654ba19`, `STATUS_V5.md`, and `DECISIONS_V5.md`.

## 1. Current repo state

| Item | State |
| --- | --- |
| Project path | `C:\Users\Batuhan\Desktop\EMG_v4_gelistirilmis_paket` |
| Branch | `v5/measured-dstk22807-footprint` |
| Git status before writing | Clean |
| Remote tracking | `github-emg/v5/measured-dstk22807-footprint` |
| Scope | Schematic block proposal only. No schematic implementation, netlist, PCB layout, routing, KiCad edit, commit, or push. |

Last 14 commits at review start:

```text
e5ef7c8 update decisions with value level power review
0f11ceb document power reference value level review
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

## 2. Purpose and boundary

This proposal defines the maximum scope of the first small power/reference schematic block if approved later.

This proposal does not modify KiCad schematic files.

This proposal does not approve the full board schematic.

This proposal does not approve the analog EMG chain schematic.

This proposal does not approve PCB layout.

This proposal does not approve DSTK22807 external 3.3 V powering.

This proposal does not approve USB Type-C plus external 3.3 V simultaneous use.

This proposal does not approve 5V as the analog/ADC supply.

## 3. Proposed schematic block scope

The future schematic block, if approved later, may include only:

- Battery input placeholder / connector.
- Optional input protection placeholder.
- 3.3 V regulator for analog/ADC.
- `3V3` rail.
- `3V3_ADC` / `ADC_REF` local decoupling.
- MCP3208 `VDD` and `VREF` decoupling.
- `analog VREF = 1.65 V` divider + buffer.
- `VREF_MON` route to MCP3208 CH6.
- `BAT_MON` divider route to MCP3208 CH7.
- Safe bias or placeholder strategy for unused/future ADC channels.
- Test points for `BAT+`, `3V3`, `ADC_REF`, `analog VREF`, `GND`, `VREF_MON`, `BAT_MON`.
- Safety notes.

Explicitly excluded:

- Full EMG analog chain.
- INA333 gain/filter stages.
- Rectifier/envelope active circuits.
- DSTK22807 external 3.3 V powering implementation.
- USB power-path protection implementation.
- PCB layout.
- Routing.
- Gerbers.

## 4. Proposed net names

| Net | Meaning | Proposed use | Risk / Note |
| --- | --- | --- | --- |
| `BAT+` | Battery/input source positive | Placeholder input to regulator and BAT_MON divider | Battery chemistry not locked. |
| `GND` | Common ground | Shared return for power/reference/ADC block | AGND/DGND are placement/return intent, not blind split. |
| `3V3` | Main 3.3 V regulated rail | Output of analog/ADC regulator | Do not assume DSTK22807 3V3 ownership. |
| `3V3_ADC` | Quiet/local ADC branch | MCP3208 VDD/near-ADC decoupling branch | Filtering must not create confusing returns. |
| `ADC_REF` | MCP3208 VREF pin / ADC full-scale reference | Clean 3.3 V local reference node | Must not be merged with analog VREF. |
| `analog VREF` | 1.65 V analog midscale bias | Buffered divider output for analog bias/VREF_MON | Must not be used as MCP3208 VREF. |
| `VREF_MON` | ADC monitor of analog VREF | MCP3208 CH6 | Must not load analog VREF. |
| `BAT_MON` | Scaled battery monitor | MCP3208 CH7 | Divider values not final. |
| `EMG1_RAW` | Future first-channel raw analog output | MCP3208 CH0 placeholder | Full analog chain excluded here. |
| `EMG1_RECT` | Future first-channel rectified analog output | MCP3208 CH1 placeholder/bias | Full analog chain excluded here. |
| `EMG1_ENV` | Future first-channel envelope output | MCP3208 CH2 placeholder/bias | Full analog chain excluded here. |
| `EMG2_RAW` | Future second-channel raw analog output | MCP3208 CH3 placeholder/bias | Future channel only. |
| `EMG2_RECT` | Future second-channel rectified output | MCP3208 CH4 placeholder/bias | Future channel only. |
| `EMG2_ENV` | Future second-channel envelope output | MCP3208 CH5 placeholder/bias | Future channel only. |
| `ADC_CS` | SPI chip select | MCP3208 CS/SHDN to provisional DSTK pin | Pinout remains provisional until physical confirmation. |
| `ADC_SCLK` | SPI clock | MCP3208 CLK | Route/noise handled later, no PCB here. |
| `ADC_MOSI` | SPI data to ADC | MCP3208 DIN | 3.3 V logic expected. |
| `ADC_MISO` | SPI data from ADC | MCP3208 DOUT | If MCP3208 ever powered at 5 V, level shifting is required. |

`ADC_REF` and `analog VREF` must not be merged. `AGND`/`DGND` should be treated as placement/return intent on common ground, not a blind split. 5V must not be used as the analog/ADC supply.

## 5. Proposed component groups

| Group | Proposed components | Candidate values | KiCad symbol/footprint notes | Status |
| --- | --- | --- | --- | --- |
| Battery connector / input placeholder | Battery connector or placeholder pins | TBD | Symbol/footprint to verify | Proposed |
| 3.3 V regulator | MCP1702-class LDO for analog/ADC-only | 3.3 V output, exact suffix TBD | Symbol/footprint to verify | Proposed, not final |
| Regulator input/output capacitors | Ceramic capacitors | 1 uF class input/output candidate, plus bulk if needed | Footprints to verify | Proposed |
| MCP3208 ADC | MCP3208-CI/P | PDIP-16 | `Analog_ADC:MCP3208`, `Package_DIP:DIP-16_W7.62mm` reviewed | Approved for first prototype ADC |
| MCP3208 VDD/VREF decoupling | Local capacitors | VDD 100 nF + 1 uF/4.7 uF candidate; VREF 100 nF + optional 1 uF | Footprints to verify | Proposed |
| analog VREF divider | Two resistors | 47k / 47k candidate | Resistor footprints to verify | Proposed, not final |
| analog VREF buffer | Verified 3.3 V RRIO op-amp, MCP6002/MCP6004-class candidate | Unity buffer, optional output isolation resistor TBD | Symbol/footprint to verify | Proposed, not final |
| VREF_MON input protection | Series resistor / optional C | 330 ohm to 1k, 100 pF to 1 nF optional | Footprints to verify | Proposed |
| BAT_MON divider | Divider resistors | Compare 330k/1M and 100k/330k | Footprints to verify | Proposed, not final |
| ADC channel input series/RC placeholders | R/C footprints | R 330 ohm to 1k, C optional by channel | Footprints to verify | Proposed |
| Unused ADC bias jumpers/resistors | Bias resistors/jumpers | 100k class or jumper-selectable | Footprints to verify | Proposed |
| Test points | Test pads/loops | BAT+, 3V3, ADC_REF, analog VREF, GND, VREF_MON, BAT_MON | Footprints to verify | Proposed |

Do not claim exact KiCad symbol/footprint is final unless already reviewed. The MCP3208 first prototype symbol/footprint/MPN are already reviewed.

## 6. Regulator block proposal

Use Option A as the initial small block:

`Battery / external supply -> 3.3 V regulator -> analog/ADC only`

DSTK22807 is not powered by this block. DSTK22807 3V3 external powering remains **NOT APPROVED YET**. Option B, regulator powering analog/ADC + DSTK22807, is not implemented in the first schematic block.

| Item | Proposed candidate | Value / note | Status |
| --- | --- | --- | --- |
| Battery connector placeholder | Connector or input pins | Exact connector TBD | Proposed |
| Optional power switch / jumper | Jumper or switch placeholder | Optional bring-up control | Proposed |
| Optional input protection placeholder | Fuse/polyfuse/diode/TVS placeholder | Value/type TBD | Proposed |
| LDO candidate family | MCP1702-class | 3.3 V, analog/ADC-only, not final lock | Proposed |
| Alternate higher-current LDO | TLV755P-class | Only if DSTK later shares rail; not implemented now | Not implemented now |
| Input capacitor | Ceramic | 1 uF class candidate | Proposed |
| Output capacitor | Ceramic | 1 uF class candidate, exact value per regulator | Proposed |
| 3V3 test point | TP_3V3 | Bring-up measurement | Proposed |
| GND test point | TP_GND | Measurement reference | Proposed |

## 7. MCP3208 power/reference block proposal

MCP3208 is proposed at 3.3 V. MCP3208 `VREF` pin is `ADC_REF`. `ADC_REF` is a clean 3.3 V local reference node, not `analog VREF`.

| MCP3208 Pin / Net | Proposed connection | Candidate value | Status |
| --- | --- | --- | --- |
| VDD | `3V3_ADC` or local `3V3` branch | 100 nF close + 1 uF/4.7 uF nearby | Proposed |
| VREF | `ADC_REF` | 100 nF close, optional 1 uF candidate | Proposed |
| AGND/DGND | Common GND with controlled return intent | No blind split | Proposed |
| CH0 | `EMG1_RAW` placeholder through optional R/C | R 330 ohm-1k, C 100 pF-1 nF optional | Proposed placeholder |
| CH1 | `EMG1_RECT` placeholder / unused bias | Bias/jumper if unpopulated | Proposed placeholder |
| CH2 | `EMG1_ENV` placeholder / unused bias | Bias/jumper if unpopulated | Proposed placeholder |
| CH3 | `EMG2_RAW` placeholder / unused bias | Bias/jumper if unpopulated | Proposed placeholder |
| CH4 | `EMG2_RECT` placeholder / unused bias | Bias/jumper if unpopulated | Proposed placeholder |
| CH5 | `EMG2_ENV` placeholder / unused bias | Bias/jumper if unpopulated | Proposed placeholder |
| CH6 | `VREF_MON` | Buffered analog VREF through small series resistor | Proposed |
| CH7 | `BAT_MON` | Divider plus optional RC/protection | Proposed |
| DIN/DOUT/CLK/CS | Existing provisional SPI nets | `ADC_MOSI`, `ADC_MISO`, `ADC_SCLK`, `ADC_CS` | Provisional |

This proposal does not implement the full ADC signal block unless later approved. If MCP3208 is ever powered at 5 V, DOUT level shifting is required before ESP32-H2. Current proposal keeps MCP3208 at 3.3 V.

## 8. analog VREF block proposal

Use 47k/47k divider from 3V3 to GND as the value-level candidate. Buffer with verified 3.3 V RRIO op-amp, MCP6002/MCP6004-class candidate, not final lock. Output net is `analog VREF`. Target is 1.65 V. `analog VREF` is not `ADC_REF`.

| Item | Proposed value/component | Reason | Risk | Status |
| --- | --- | --- | --- | --- |
| Rtop | 47k | Current/power/noise compromise | Exact tolerance/noise not reviewed | Candidate |
| Rbottom | 47k | Produces 1.65 V from 3.3 V | Exact tolerance/noise not reviewed | Candidate |
| Divider current | Approx 35.1 uA | Lower drain than 10k/10k | More impedance than 10k/10k | Calculated candidate |
| Thevenin resistance | Approx 23.5 kOhm before buffer | Acceptable into high-Z buffer | Not suitable as shared bias without buffer | Calculated candidate |
| Buffer op-amp | Verified 3.3 V RRIO op-amp | Low output impedance for bias loads | Stability/output swing must be checked | Proposed |
| Optional output isolation resistor | TBD, possible 100 ohm only if stability review accepts | Helps capacitive load stability | Adds output impedance | TBD |
| Local capacitor / decoupling | TBD | Noise control | Can destabilize op-amp if direct capacitive load | TBD |
| analog VREF test point | TP_ANALOG_VREF | Bring-up measurement | Probe policy required | Proposed |
| VREF_MON route | To MCP3208 CH6 | Digital offset monitoring | Must not load analog VREF | Proposed |

Passive divider alone is not approved as shared bias. Buffer stability must be checked before schematic implementation. Do not tie many low-value unused ADC biases to analog VREF.

## 9. BAT_MON block proposal

Calculation assumption only: 1S LiPo max = 4.2 V and `ADC_REF = 3.3 V`. Battery choice is not locked.

| Rtop | Rbottom | Vout at 4.2 V | Divider current | Source impedance | Proposed status |
| --- | --- | --- | --- | --- | --- |
| 330k | 1M | 3.158 V | 3.158 uA | 248 kOhm | Lower-drain candidate, likely needs acquisition/RC strategy |
| 100k | 330k | 3.223 V | 9.767 uA | 76.7 kOhm | Easier ADC acquisition candidate, higher drain |

Recommended candidate for future schematic proposal: 330k/1M as a low-drain starting point, with explicit note that ADC acquisition/source impedance must be reviewed. If easier acquisition is prioritized over always-on drain, 100k/330k may be selected instead.

BAT_MON final value requires acquisition/source impedance review. If charger/protection changes maximum battery voltage, divider must be recalculated. Optional RC/protection footprint may be proposed with values TBD.

## 10. ADC input R/C and unused channel proposal

| Channel | Net | Proposed R/C / bias | Implementation status |
| --- | --- | --- | --- |
| CH0 | `EMG1_RAW` | 330 ohm to 1k series footprint, optional 100 pF to 1 nF C footprint | Placeholder only |
| CH1 | `EMG1_RECT` | Same if populated, otherwise high-value bias/jumper | Placeholder only |
| CH2 | `EMG1_ENV` | 330 ohm to 1k, optional 1 nF to 10 nF depending envelope source | Placeholder only |
| CH3 | `EMG2_RAW` | Future channel bias or jumper if not populated | Placeholder only |
| CH4 | `EMG2_RECT` | Future channel bias or jumper if not populated | Placeholder only |
| CH5 | `EMG2_ENV` | Future channel bias or jumper if not populated | Placeholder only |
| CH6 | `VREF_MON` | Buffered analog VREF through small series resistor candidate | Proposed |
| CH7 | `BAT_MON` | Divider plus optional RC/protection | Proposed |

Exact values are not final until acquisition/source impedance calculation is accepted. No ADC channel should float. Direct tie without resistor is not recommended.

## 11. Test point proposal

| Test point | Net | Purpose | Human-test warning |
| --- | --- | --- | --- |
| TP_BAT | `BAT+` | Battery/regulator input bring-up | Do not use earth-referenced equipment while electrodes are attached. |
| TP_3V3 | `3V3` | Regulator output check | No USB during human EMG tests. |
| TP_ADC_REF | `ADC_REF` | ADC reference validation | Do not confuse with analog VREF. |
| TP_ANALOG_VREF | `analog VREF` | Midscale bias validation | Probe only with safe setup. |
| TP_GND | `GND` | Measurement reference | Common ground, no blind split. |
| TP_VREF_MON | `VREF_MON` | ADC monitor path check | Avoid loading analog VREF. |
| TP_BAT_MON | `BAT_MON` | Battery divider output check | Confirm scaling before ADC use. |
| Optional TP_ADC_SPI pins | `ADC_CS/SCLK/MOSI/MISO` | SPI bring-up | Digital probing can inject noise. |

Test points are for bring-up. Do not use earth-referenced equipment while electrodes are attached to a human. USB must not be connected during human EMG tests.

## 12. Proposed KiCad implementation boundary for next step

| Allowed later if approved | Not allowed |
| --- | --- |
| One small sheet/section for power/reference/ADC support | Full board schematic |
| Battery input placeholder | Full EMG analog chain |
| 3.3 V analog/ADC regulator block | INA333 gain/filter stages |
| MCP3208 power/reference decoupling | Rectifier/envelope active circuits |
| analog VREF divider + buffer | DSTK external 3.3 V power implementation |
| BAT_MON/VREF_MON support | USB simultaneous power implementation |
| ADC placeholder/bias support | PCB layout / routing / gerbers |
| Test points and safety notes | Production output |

## 13. Open blockers before actual schematic edit

- Review this proposal.
- Decide if small schematic block is allowed.
- Verify exact KiCad symbols/footprints for regulator/op-amp/connector/test points.
- Verify analog VREF buffer stability at 3.3 V.
- Accept ADC_REF decoupling strategy.
- Accept BAT_MON divider candidate.
- Accept ADC input R/C/source impedance strategy.
- Decide unused ADC bias approach.
- Keep DSTK22807 power path unresolved unless separately tested.
- Human-test safety procedure remains required.

## 14. Decision

- Full board schematic: **NO**.
- Analog EMG chain schematic: **NO**.
- PCB layout: **NO**.
- Small power/reference schematic block implementation: **NOT IN THIS TASK**.
- Small power/reference schematic block proposal: **READY_FOR_REVIEW**.
- 3V3 external powering of DSTK22807: **NOT APPROVED YET**.
- USB-powered human EMG testing: **FORBIDDEN**.
- 5V pin as analog/ADC supply: **NOT APPROVED**.

## 15. Recommended next action

- Review and commit this proposal document.
- Update `DECISIONS_V5.md` in a separate commit to reference this proposal.
- Only after that, optionally allow Codex to perform a strictly limited KiCad schematic edit for the small power/reference block.
- That implementation task must explicitly name allowed schematic file(s), allowed sheet/section, allowed symbols/nets, and forbidden files.

Final decision: **POWER_REFERENCE_SCHEMATIC_BLOCK_PROPOSAL_READY_FOR_REVIEW**
