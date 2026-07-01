# EMG V5 Draft BOM

This BOM is a draft until the V5 schematic is fully captured.

| Function | Reference Prefix | Candidate Part | Notes |
| --- | --- | --- | --- |
| Instrumentation amp | U_INA1, U_INA2 | INA333xxDGK | One per EMG channel |
| Quad op-amp | U_OP1..U_OP3 | MCP6004 | Minimum 3x; unused amps terminated safely |
| ADC | U_ADC1 | MCP3208 | 8-channel 12-bit SPI ADC |
| MCU dev board | MOD1 | DSTK22807 ESP32-H2 Super Mini | Mounted board, custom carrier footprint after measurement |
| TRS jack | J1, J3 | SJ1-3523N or verified equivalent | Tip IN+, Ring IN-, Sleeve BODY_REF |
| Battery input | J_BAT | JST-PH 2-pin or verified equivalent | External 1S LiPo charging only |
| Regulator | U_REG1 | 3.3 V LDO, low-noise preferred | Sized for analog + DSTK22807 current |
| ESD input protection | D_ESD* | Low-leakage biopotential-suitable ESD | Must not load high-impedance inputs |
| USB ESD | On DSTK board | Onboard / not on EMG PCB | No separate USB-C on first prototype |
| Test points | TP* | 1 mm pads or loops | RAW/RECT/ENV, SPI, power, input nodes |

## Required Analog Nodes

- EMG1_RAW
- EMG1_RECT
- EMG1_ENV
- EMG2_RAW
- EMG2_RECT
- EMG2_ENV
- VREF
- BAT_MON

