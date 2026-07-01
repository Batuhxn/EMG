# DSTK22807 Power Pin Physical Measurement Observation

## 1. Current repo state

| Item | State |
| --- | --- |
| Project path | `C:\Users\Batuhan\Desktop\EMG_v4_gelistirilmis_paket` |
| Branch | `v5/measured-dstk22807-footprint` |
| Git status before writing | Clean |
| Scope | Measurement observation only. This is not an official schematic/datasheet and does not approve power implementation. |

Last 10 commits at review start:

```text
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

## 2. Measurement setup

| Measurement | USB Type-C | External Power | Reference | Notes |
| --- | --- | --- | --- | --- |
| USB powered measurement | Connected | Not connected | DSTK22807 GND pin | User measured 5V and 3V3 pins while Type-C was connected. |
| Unpowered discharge measurement | Not connected | Not connected | DSTK22807 GND pin | User measured 5V and 3V3 after power removal over time. |

User reports one GND pin available on the board/header and used that GND pin as the measurement reference.

## 3. Observed voltages

| Condition | Pin / Rail | Measured Voltage | Interpretation |
| --- | --- | --- | --- |
| USB Type-C connected, no external battery/regulator | 5V pin | 5.126 V | USB-powered 5V rail is present on the board pin. |
| USB Type-C connected, no external battery/regulator | 3V3 pin | 3.291 V | Board exposes approximately 3.3 V on the 3V3 pin while USB powered. |
| USB disconnected, external power disconnected | 5V pin | 0 V | 5V pin falls to 0 V when USB and external power are disconnected. |
| USB disconnected, external power disconnected, after 10 s | 3V3 pin | 0.590 V | Residual voltage remains after power removal. |
| USB disconnected, external power disconnected, after 30 s | 3V3 pin | 0.400 V | 3V3 rail continues decaying downward. |
| USB disconnected, external power disconnected, after 60 s | 3V3 pin | 0.250 V | Continued decay is consistent with residual capacitor discharge / leakage path, not active power. |

User observation: when Type-C is connected, both 5V and 3.3V are available on their respective pins. With Type-C and external power disconnected, 5V is 0 V and 3V3 decays downward over time.

## 4. What this proves

- USB Type-C supplies the board 5V rail.
- Board exposes approximately 3.3 V rail while USB powered.
- 5V falls to 0 V when USB and external power are disconnected.
- 3V3 decays over time after power removal, consistent with capacitor discharge / leakage path.
- GND reference is available on the header/pad set.

## 5. What this does not prove

- This does not prove that the 3V3 pin is safe as an external power input.
- This does not prove the current capability of the 3V3 pin.
- This does not prove that USB and external 3.3 V can be connected at the same time.
- This does not prove safe backfeeding behavior.
- This does not prove whether the 3V3 pin is directly connected to the onboard regulator output.
- This does not approve the full power architecture.

## 6. Power architecture implication

- Battery-first EMG prototype remains preferred.
- External 3.3 V regulator remains preferred for analog/ADC.
- DSTK22807 3V3 power direction remains unresolved.
- 5V pin is not required for the intended analog/ADC supply.
- Avoid USB during human-connected EMG testing.
- Do not power analog/ADC from 5V.
- Do not back-feed DSTK22807 3V3 until the board power path is verified.
- Do not connect external 3.3 V while USB Type-C is connected unless power-path safety is proven.
- For human-connected EMG tests, USB Type-C must be disconnected and the system must be battery-powered/isolated.
- MCP3208 and analog front-end remain planned for 3.3 V operation.
- If the DSTK22807 module is later powered through its 3V3 pin, that must be a separate explicit decision after power-path verification.

## 7. Remaining measurements / checks

- Verify whether 3V3 pin is connected directly to onboard regulator output.
- Verify whether external 3.3 V can safely power the board through 3V3 pin.
- Verify if any reverse-current path exists when USB is connected.
- Verify current draw of DSTK22807 under BLE/active operation if the same battery regulator will power it.
- Confirm GND continuity to any shield/ground points only when unpowered.
- If testing external 3.3 V powering later, use current-limited supply/regulator, USB disconnected, no human electrodes, and measure current.

## 8. Decision status

- Full power implementation: **NO**
- Documentation/planning: **YES**
- 3V3 external powering of DSTK22807: **NOT APPROVED YET**
- USB-powered human EMG testing: **FORBIDDEN**
- 5V pin as analog/ADC supply: **NOT APPROVED**
- Battery-powered isolated operation for human-contact testing: **REQUIRED**

## 9. Recommended next action

- Update `V5/DECISIONS_V5.md` later, in a separate commit, to reference this measurement.
- Next technical step should be deciding battery/regulator architecture only after DSTK22807 3V3/5V behavior is sufficiently verified.
- Do not start schematic power implementation yet.

Final decision: **DSTK_POWER_MEASUREMENT_READY_FOR_REVIEW**
