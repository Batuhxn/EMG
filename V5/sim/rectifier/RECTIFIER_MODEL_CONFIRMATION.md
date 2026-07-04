# Rectifier Model Confirmation

## Scope

This pass tightens model assumptions for the Channel 1 VREF-centered two-op-amp absolute-value rectifier candidate before any KiCad implementation.

Review target:

- `V5/sim/rectifier/emg_vref_abs_rectifier_real_topology_candidate.cir`

No KiCad schematic, PCB, `STATUS_V5.md`, or `DECISIONS_V5.md` files were edited in this pass.

## Local Model Search

Local search roots checked:

- `C:\Users\Batuhan\Desktop\EMG_v4_gelistirilmis_paket`
- `C:\Users\Batuhan\AppData\Local\Programs\ADI\LTspice`
- `C:\Users\Batuhan\Documents\LTspice`
- `C:\Users\Batuhan\AppData\Local\LTspice`

Search terms:

- `MCP6001`
- `MCP6002`
- `MCP6004`
- `MCP600x`
- `BAS70`
- `BAT54`

## Exact Models Found

### MCP600x op-amp models

No exact local LTspice `.subckt` / model file was found for:

- `MCP6001`
- `MCP6002`
- `MCP6004`
- `MCP600x`

The repository contains KiCad symbols, BOM notes, and review-document references to MCP6002/MCP6004-class parts, but no usable vendor LTspice macromodel was found locally.

Conclusion: exact MCP600x simulation is **not locally confirmed** in this pass.

### BAS70 diode models

No exact local BAS70 LTspice model was found.

Conclusion: BAS70 simulation is **not locally confirmed** in this pass.

### BAT54 diode models

Local LTspice Toshiba Contrib encrypted BAT54-family files were found:

| File | Type | Notes |
| --- | --- | --- |
| `C:\Users\Batuhan\AppData\Local\LTspice\lib\sub\Contrib\Toshiba\DIODE\TBAT54_enc.lib` | Encrypted LTspice model | Permission text says use for simulations, not reverse engineering. |
| `C:\Users\Batuhan\AppData\Local\LTspice\lib\sub\Contrib\Toshiba\DIODE\TBAT54A_enc.lib` | Encrypted LTspice model | Same limitation. |
| `C:\Users\Batuhan\AppData\Local\LTspice\lib\sub\Contrib\Toshiba\DIODE\TBAT54C_enc.lib` | Encrypted LTspice model | Same limitation. |
| `C:\Users\Batuhan\AppData\Local\LTspice\lib\sub\Contrib\Toshiba\DIODE\TBAT54S_enc.lib` | Encrypted LTspice model | Same limitation. |
| `C:\Users\Batuhan\AppData\Local\LTspice\lib\sym\Contrib\Toshiba\diode\TBAT54.asy` | LTspice symbol | `SYMATTR Value TBAT54`, `SYMATTR ModelFile Contrib/Toshiba/DIODE/TBAT54_enc.lib`, `Prefix X`. |

These files can be used locally in LTspice if the same LTspice installation is present. They should not be copied into the repository in this pass. They are encrypted vendor/contrib files, so the internal model/subckt implementation was not inspected.

Conclusion: a local BAT54-class model exists, but the current rectifier `.cir` still uses a generic Schottky approximation unless manually updated to use the local Toshiba encrypted model.

## Include Safety

- Do not copy encrypted Toshiba model files into the project repository.
- A local-only LTspice test can reference the installed Contrib model path, but that makes the simulation environment-dependent.
- Because exact MCP600x op-amp models were not found, a full vendor-model confirmation remains open.

## Datasheet-Limit Checklist Before KiCad Implementation

Before translating the topology into KiCad, check these limits against the chosen exact parts and packages:

- [ ] Op-amp input common-mode range at 3.3 V, especially near `analog VREF = 1.65 V` and summing-node excursions.
- [ ] Op-amp output swing near 0 V and 3.3 V under the expected rectifier load.
- [ ] Output drive current into `RECT_OUT`, including `470R/1nF` ADC RC path and `33k/1uF` ENV path.
- [ ] GBW / phase margin for the precision rectifier stage, weighted summer, ADC RC load, and ENV load.
- [ ] Slew rate at 20 Hz, 100 Hz, and 500 Hz for AMPL = 0.02 V, 0.10 V, 0.50 V, and 1.00 V.
- [ ] Input offset effect near the `VREF` crossover where rectifier error is most visible.
- [ ] Diode forward behavior near very small signals around `VREF`.
- [ ] Diode leakage over expected temperature range.
- [ ] Diode capacitance and its effect on crossover spikes/stability.
- [ ] Resistor ratio tolerance, especially the matched `10k/20k` weighted summer network.

## Conservative Simulation Option

Because exact MCP600x op-amp models were not found locally, an additional conservative generic simulation file was created:

- `V5/sim/rectifier/emg_vref_abs_rectifier_conservative_rrio_candidate.cir`

This file keeps the same rectifier topology as the real candidate and still avoids behavioral `abs()` inside the rectifier core. It uses a finite-gain, finite-bandwidth, slew-current-limited, limited-output-swing generic RRIO approximation.

The conservative file preserves the same simulation cases:

- `AMPL`: 0.02, 0.10, 0.50, 1.00
- `FREQ`: 20, 100, 500
- `VCM`: 1.60, 1.65, 1.70

The conservative file preserves the same load:

- `RECT_OUT -> 470R -> EMG1_RECT_ADC`
- `EMG1_RECT_ADC -> 1nF to GND`
- `RECT_OUT -> 33k -> EMG1_ENV_DRV`
- `EMG1_ENV_DRV -> 1uF to VREF`

## Interpretation

The previous manual review remains useful: the topology is structurally plausible and reviewable. This model pass improves caution but does not close the exact-device-model gap.

The strongest remaining uncertainty is the MCP600x op-amp model. The rectifier depends on op-amp recovery, output swing, input offset, and crossover behavior. These are exactly the details that generic models can understate.

A local BAT54-family encrypted model is available in LTspice, but exact BAS70 is not. If BAT54 is selected for the schematic, a local-only LTspice variant can be made later that explicitly uses `TBAT54`. If BAS70 is selected, a local BAS70 model or datasheet-based conservative approximation is still needed.

## Conservative RRIO Rerun Result

Latest conservative RRIO rerun result:

- Initial conservative candidate failed because the op-amp macromodel transconductance source polarity was reversed.
- The model bug was fixed by changing `GIN INT 0 ...` to `GIN 0 INT ...`.
- This was a macromodel polarity bug, not a topology change.
- After rerun, the conservative nominal case tracks `IDEAL_RECT`.
- The conservative 1.00 V stress case reaches about 2.65 V and does not clip at 3.3 V.
- `ERR` is no longer volt-level and remains near 0 visually, with remaining exact error still model-dependent.
- `EMG1_RECT_ADC` follows `RECT_OUT`.
- `EMG1_ENV_DRV` gives plausible smoothing.
- Exact MCP600x vendor model is still not available locally.
- Exact BAS70/BAT54 vendor model is still not copied into the repo.
- Therefore this is **REVIEWABLE**, not KiCad-approved.
## Final Verdict

MODEL_CONFIRMATION_PASS_BUT_NEEDS_EXACT_VENDOR_MODEL
