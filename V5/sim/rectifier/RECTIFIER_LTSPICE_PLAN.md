# RECTIFIER LTspice Simulation Plan

## 1. Goal

Create and run an LTspice validation flow for a Channel 1 analog absolute-value rectifier candidate around `analog VREF`.

Target behavior:

```text
EMG1_RECT_DRV = analog VREF + abs(EMG1_RAW_DRV - analog VREF)
```

The rectifier output must feed the existing path:

```text
EMG1_RECT_DRV -> R331 470R -> EMG1_RECT
C331 1nF to GND
ENV LPF 33k + 1uF
```

This plan is simulation planning only. It does not approve a KiCad schematic edit.

## 2. Constraints

- Single-supply operation: `3V3_ADC = 3.3 V`.
- `analog VREF = 1.65 V` midbias.
- Input signal `EMG1_RAW_DRV` is centered around `analog VREF`.
- Output target is `EMG1_RECT_DRV` / `RECT_OUT`.
- Output must stay inside ADC-safe range: GND to `3V3_ADC` / `ADC_REF`.
- `ADC_REF` must not be used as analog bias.
- `ADC_REF` and `analog VREF` must remain separate.
- Do not use 5 V.
- Do not connect `EMG1_REF_ELECTRODE` to GND.
- Candidate op-amp family: MCP6004/MCP6002-class RRIO.
- Candidate diode family: BAS70/BAT54-class Schottky.
- Prefer matched 10k/20k style resistor networks if the final circuit needs gain/summing ratios.

## 3. Candidate Topology

Recommended topology for simulation: two-op-amp precision absolute-value rectifier referenced to `analog VREF`.

Conceptual stages:

1. Stage A: precision negative half-wave generator around `analog VREF`.
   - Computes approximately `max(analog VREF - EMG1_RAW_DRV, 0)`.
   - Implemented with one MCP6004-class RRIO op-amp and Schottky feedback diodes in the final real circuit.

2. Stage B: summing / reconstruction stage.
   - Computes approximately `EMG1_RAW_DRV + 2 * Stage_A`.
   - This equals `analog VREF + abs(EMG1_RAW_DRV - analog VREF)`.
   - Implemented with one MCP6004-class RRIO op-amp and matched resistor ratios in the final real circuit.

This is the only reviewed analog candidate that directly matches the required full-wave absolute-value behavior. Half-wave rectification is easier but does not meet the stated target unless the requirement is relaxed.

## 4. Proposed LTspice Circuit Blocks

Use the optional netlist:

```text
V5/sim/rectifier/emg_vref_abs_rectifier_candidate.cir
```

Planned circuit blocks:

- `V3V3`: 3.3 V supply.
- `VREF`: 1.65 V analog midbias.
- `RAW_IN`: sine input centered on `VREF`.
- `RECT_INT_A`: first-stage negative half-wave intermediate.
- `RECT_INT_B`: second-stage reconstructed full-wave intermediate.
- `RECT_OUT`: rectifier output / `EMG1_RECT_DRV` target.
- `EMG1_RECT_ADC`: ADC-side node after 470R/1nF.
- `EMG1_ENV_DRV`: envelope LPF node after 33k/1uF.

Model availability:

- Exact MCP6004/MCP6002 LTspice model was not found in the local LTspice directory during this planning pass.
- Exact BAS70/BAT54 LTspice model was not found in the local LTspice directory during this planning pass.
- The optional `.cir` therefore uses a behavioral transfer approximation for the rectifier core and a generic Schottky diode model placeholder.
- This is enough to verify target waveform/load cases, but not enough to approve the final op-amp/diode implementation.

## 5. Exact Simulation Cases

Run transient simulations for these cases.

### Case 1: Small sine input

```text
EMG1_RAW_DRV = 1.65 V + 20 mV sine, 100 Hz
```

### Case 2: Medium sine input

```text
EMG1_RAW_DRV = 1.65 V + 100 mV sine, 100 Hz
```

### Case 3: Large sine input

```text
EMG1_RAW_DRV = 1.65 V + 500 mV sine, 100 Hz
```

### Case 4: Worst-case clipping check

```text
EMG1_RAW_DRV = 1.65 V + 1.0 V sine, 100 Hz
```

### Case 5: Frequency sanity

Repeat amplitude cases at:

- 20 Hz.
- 100 Hz.
- 500 Hz.

### Case 6: Offset sensitivity

Repeat cases with input centered at:

- 1.60 V.
- 1.65 V.
- 1.70 V.

The optional `.cir` uses stepped parameters:

```text
.step param AMPL list 0.02 0.10 0.50 1.00
.step param FREQ list 20 100 500
.step param VCM list 1.60 1.65 1.70
```

## 6. Expected Waveforms

Expected ideal behavior:

- When `RAW_IN = VREF`, `RECT_OUT` should sit near `VREF`.
- For `RAW_IN > VREF`, `RECT_OUT` should follow `RAW_IN`.
- For `RAW_IN < VREF`, `RECT_OUT` should rise above `VREF` as `2 * VREF - RAW_IN`.
- `RECT_OUT` should never go below GND.
- `RECT_OUT` should not clip near 3.3 V for expected EMG amplitudes.
- `EMG1_RECT_ADC` should closely follow `RECT_OUT` through the 470R/1nF output RC at EMG frequencies.
- `EMG1_ENV_DRV` should smooth the rectified waveform through the 33k/1uF envelope path.

For the 1.0 V worst-case sine, ideal peak output is approximately 2.65 V, still below 3.3 V. Real op-amp output swing must still be checked with a real MCP6004/MCP6002 model or conservative RRIO output swing limits.

## 7. Pass / Fail Criteria

Pass criteria:

- `RECT_OUT` rests near 1.65 V when `RAW_IN = VREF`.
- `RECT_OUT` rises above 1.65 V for both positive and negative input excursions.
- `RECT_OUT` approximately matches `VREF + abs(RAW_IN - VREF)`.
- No clipping for 20 mV, 100 mV, and 500 mV input cases.
- 1.0 V input case remains inside GND to 3.3 V in the behavioral testbench.
- `EMG1_RECT_ADC` is not excessively attenuated by 470R/1nF at 20 Hz to 500 Hz.
- ENV LPF produces a plausible smoothed envelope from `RECT_OUT`.

Warn criteria:

- Noticeable crossover error near `VREF`.
- Output approaches op-amp swing limits.
- Envelope response is too slow or too fast for intended feature extraction.
- `RECT_OUT` load from ADC RC and ENV path is too heavy for MCP6004-class drive.

Fail criteria:

- Output rectifies only one polarity.
- Output goes below GND or above 3.3 V.
- Output clips in normal expected signal range.
- `ADC_REF` is used as analog bias in the proposed implementation.
- `analog VREF` and `ADC_REF` are merged.

## 8. Risks

- Generic behavioral models can hide MCP6004/MCP6002 output swing, input common-mode, slew, and recovery limitations.
- Generic Schottky approximation can hide diode leakage and near-zero crossover behavior.
- Two-op-amp precision rectifiers are sensitive to resistor matching and diode orientation.
- MCP6004-class GBW and slew rate are likely adequate for 20 Hz to 500 Hz EMG, but this still needs confirmation with a realistic model.
- The final KiCad circuit must be reviewable as actual op-amp/diode/resistor topology, not only a behavioral `abs()` block.
- `RECT_OUT` must remain low impedance enough to drive both ADC RC and ENV LPF.

## 9. What Must Be Proven Before KiCad Edit

Before editing `V5/EMG_CHANNEL_1_ANALOG.kicad_sch`:

- Choose the exact two-op-amp precision rectifier topology and diode orientation.
- Confirm the topology produces `VREF + abs(RAW_IN - VREF)` at 3.3 V.
- Confirm no normal-case clipping with expected RAW gain.
- Confirm acceptable crossover behavior near `analog VREF`.
- Confirm output can drive R331/C331 and ENV LPF without instability or major error.
- Confirm MCP6004/MCP6002-class operation at 3.3 V is acceptable.
- Confirm BAS70/BAT54-class diode leakage and forward behavior are acceptable for the signal amplitude.
- Keep `ADC_REF` out of the analog child sheet.
- Keep `EMG1_REF_ELECTRODE` isolated from GND.

## 10. Final Recommendation

READY_TO_SIMULATE

The optional `.cir` is ready as a first behavioral target/load simulation. However, KiCad edit should remain blocked until a second simulation pass uses either:

- a real MCP6004/MCP6002 LTspice macromodel and BAS70/BAT54 model, or
- an explicit generic op-amp plus diode-feedback topology that does not rely on a direct behavioral `abs()` expression.
