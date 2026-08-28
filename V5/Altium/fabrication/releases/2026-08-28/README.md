# EMG V5 fabrication release - 2026-08-28

This directory is the validated bare-PCB upload set generated from `EMG_v5_FAB_READY.PcbDoc`.

## Factory upload

Upload either `EMG_v5_FAB_READY_FINAL_2026-08-28.zip` as-is, or the following nine raw files:

- `EMG_v5_FAB_READY_Copper_Signal_Top.gbr`
- `EMG_v5_FAB_READY_Copper_Signal_Bot.gbr`
- `EMG_v5_FAB_READY_Soldermask_Top.gbr`
- `EMG_v5_FAB_READY_Soldermask_Bot.gbr`
- `EMG_v5_FAB_READY_Legend_Top.gbr`
- `EMG_v5_FAB_READY_Profile.gbr`
- `EMG_v5_FAB_READY-RoundHoles-Plated.TXT`
- `EMG_v5_FAB_READY-RoundHoles-NonPlated.TXT`
- `EMG_v5_FAB_READY-SlotHoles-Plated.TXT`

Do not upload both the ZIP and the unpacked files in the same order.

## Validation summary

- Two copper layers; closed 90 mm x 70 mm profile.
- 201 plated round holes, 6 plated routed slots and 14 non-plated round holes.
- All 221 drill/slot features are inside the board profile.
- Drill-to-copper coordinate agreement is within 0.001 mm.
- The six plated slots use a 0.4 mm tool and a 1.0 mm routed centerline.
- Empty bottom legend, aperture libraries, CAM workspaces and generation reports are intentionally excluded.
- ZIP SHA-256: `9C4FC7E760760C099C7671478158BE8C14589FB19BDB24C5B37D85E946BDE52A`.

The archived Altium PCB DRC dated 2026-08-13 reports 0 warnings and 0 rule violations. Because these fabrication files were generated on 2026-08-28, rerun DRC before ordering if the PCB changed after the archived report.
