# EMG V5 Prototype Notes

This V5 revision is a two-channel EMG acquisition board concept based on the V4 analog front-end.

V5 keeps V4 intact. Do not send V5 Gerbers to fabrication until the DSTK22807 carrier footprint, schematic, and PCB layout are completed and checked.

## Signal Definitions

- `EMGx_RAW`: amplified and filtered EMG waveform centered around `VREF = 1.65 V`.
- `EMGx_RECT`: active precision rectified signal, `VREF + abs(EMGx_RAW - VREF)`.
- `EMGx_ENV`: analog envelope, `VREF + low_pass(abs(EMGx_RAW - VREF))`.

RECT must be an active precision full-wave rectifier / absolute value circuit referenced to VREF. Passive diode-only rectification is not acceptable.

## Electrode Connectors

- `J1 = EMG_CH1_TRS`
- `J3 = EMG_CH2_TRS`

TRS mapping:

- Tip: `IN+`
- Ring: `IN-`
- Sleeve: `BODY_REF`

`BODY_REF` is not protective earth. It must not connect directly to USB shield, digital GND, chassis, or earth. It may only connect through the intended protected body-bias/reference network.

## ADC Channel Map

ADC: `MCP3208`, 12-bit SPI.

| MCP3208 Channel | Signal |
| --- | --- |
| CH0 | EMG1_RAW |
| CH1 | EMG1_RECT |
| CH2 | EMG1_ENV |
| CH3 | EMG2_RAW |
| CH4 | EMG2_RECT |
| CH5 | EMG2_ENV |
| CH6 | VREF_MON |
| CH7 | BAT_MON |

RAW ADC input cleanup RC must not reduce RAW bandwidth. Keep its cutoff comfortably above the 450-500 Hz RAW low-pass target.

## DSTK22807 Board

The first prototype uses the `DSTK22807 ESP32-H2 Super Mini Bluetooth Board` as a mounted development board, not a bare Espressif module.

Before creating or placing the custom DSTK22807 carrier footprint, confirm:

- board dimensions
- pin header spacing
- exact pin labels
- antenna location and keepout
- USB connector overhang
- available SPI-capable GPIOs

Do not place `ESP32-H2-MINI-1`, `ESP32-H2-MINI-1U`, or ESP32-S3 SMD module footprints in the first V5 prototype.

## BLE Packet

Binary frame fields:

```text
sample_index
ch1_raw
ch1_rect
ch1_env
ch2_raw
ch2_rect
ch2_env
vref
batt
```

## USB CSV

USB CSV is optional debug output through the DSTK22807 onboard USB connector. It is for non-human testing only.

```text
index,ch1_raw,ch1_rect,ch1_env,ch2_raw,ch2_rect,ch2_env,vref,batt
```

On serial startup firmware must print:

```text
WARNING: Do not attach electrodes to a human while USB is connected.
Human testing is battery-only.
```

## Safety

Human electrode testing is battery-only.

Do not attach electrodes to a person while USB, wall adapter, earth-grounded oscilloscope, or grounded lab supply is connected.

Silkscreen warnings required:

```text
BATTERY ONLY WHEN ELECTRODES ATTACHED
NO USB WHEN ELECTRODES ATTACHED
RESEARCH PROTOTYPE ONLY - NOT A MEDICAL DEVICE
```

