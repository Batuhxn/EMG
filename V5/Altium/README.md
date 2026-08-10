# EMG V5 Altium release

This directory contains the Altium source used for the current EMG V5 fabrication release. The release PCB is `EMG_v5_FAB_READY.PcbDoc`; intermediate routing/safety copies and Altium `History` files are intentionally excluded.

## Project contents

- `EMG_v5.PrjPcb`: portable project file with stale log, CAM and external-script document entries removed. Embedded Gerber/drill output paths were changed from the original absolute Desktop path to the relative `EMG_v5_FAB_READY.PcbDoc` path.
- `EMG_v5.SchDoc`, `POWER_REFERENCE_BLOCK.SchDoc`, `EMG_CHANNEL_1_ANALOG.SchDoc`, `EMG_CHANNEL_2_ANALOG.SchDoc`: hierarchical schematics.
- `EMG_v5.SchLib`: project schematic library.
- `EMG_v5_FAB_READY.PcbDoc`: routed two-layer fabrication PCB.
- `Project Outputs for EMG_v5/`: Gerber X2, Excellon drill, DRC and generation reports, plus the factory ZIP.

## BOM cross-check

Binary Altium schematic and PCB component records were compared against `../EMG_v5.kicad_pcb` and the purchasing BOM in `../BOM_V5_FINAL.md`.

- 134 annotated PCB RefDes match between Altium and KiCad with no missing or extra electrical components.
- All matched values and footprint names agree between the Altium schematics, Altium FAB_READY PCB and KiCad V5 PCB.
- The five schematic-only `#FLG...` power flags correctly have no PCB footprint.
- Nineteen `TP...` test pads and four no-RefDes mounting holes are excluded from purchasing.
- 115 purchasing positions remain: 113 populated components and two DNP components.
- `C304` and `C404` are `100pF DNP` and must remain unpopulated.
- `J301` and `J401` are `SJ1-3523N` with the CUI/Same Sky horizontal jack footprint.
- `C213` is 47 uF in the Murata GRM32 1210 footprint; the approved purchasing MPN remains `GRM32ER71A476KE15L`.

Distributor selection, prices, alternatives and purchasing verification gates remain in `../BOM_V5_FINAL.md`. The Altium conversion does not change those sourcing decisions.

## Fabrication output validation

The supplied Altium DRC for `EMG_v5_FAB_READY.PcbDoc` reports zero violations, including zero clearance, short-circuit and unrouted-net violations.

The Gerber X2 set is metric and contains:

- top and bottom copper (`Copper,L1,Top,Signal` and `Copper,L2,Bot,Signal`);
- top and bottom solder mask;
- top legend;
- a closed non-plated board profile.

The bottom legend file is a valid but empty Gerber and is therefore omitted from `EMG_Gerber.zip`. The raw empty file is retained with the generation report for auditability.

The Excellon set is metric and contains:

- plated round holes: 5 tools and 201 coordinate hits;
- non-plated round holes: 2 tools and 14 coordinate hits;
- plated slots: 1 tool and 6 routed slots.

Every file in `EMG_Gerber.zip` was SHA-256 compared with its corresponding raw output and matched exactly. All Gerber files end with `M02`; all drill files end with `M30`.

## Release boundary

This is a complete bare-PCB fabrication package. Solder-paste/stencil layers, centroid/pick-and-place data and an assembly drawing are not present, so contract assembly requires a separate Altium assembly-output pass. A fresh schematic compile/ERC should also be archived before a production release; the checked-in report is PCB DRC only.
