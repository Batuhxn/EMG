# EMG V5 assembly release notes

These notes apply to `EMG_v5_FAB_READY.PcbDoc` and take precedence over generic assembler defaults.

## Approved population data

- Purchasing BOM: `../BOM_V5_FINAL.md`.
- `C304` and `C404` are DNP and must remain unpopulated.
- Test pads and the four mounting holes are PCB features, not placement lines.
- `SW501` must use Omron `A6D-4100`. The previous C&K `TDA04H0SB1R` selection is rejected because it is a 1.27 mm SMD part.
- Omron `A6D-4100` matches the Altium footprint: 8-pin through-hole, 2.54 mm pitch, 7.62 mm row spacing and 12.2 mm body length. Its opposing contact pairs 1-8, 2-7, 3-6 and 4-5 match the four configuration nets switching to GND.

## Via-in-pad process gate

The following 0.30 mm vias are located in SMD pads and are not approved as ordinary open vias for stencil assembly:

- `C205.1`
- `C217.1`
- `C360.1`
- `C431.1`
- `C451.1`
- `D331.1`
- `D332.1`
- `D332.2`
- `R206.2`
- `U302.2`
- `U402.5`

Release is permitted only when one of these options is documented on the fabrication order:

1. Resin/epoxy fill, planarize and copper-cap the listed vias (VIPPO), with the top pad surface suitable for stencil printing; or
2. Move the vias out of the pads in Altium, reroute as needed, run DRC, and regenerate every fabrication and assembly output.

Simple solder-mask tenting is not an acceptable substitute for filled-and-capped via-in-pad on these locations.

## Fiducials and panelization

The current PCB has no dedicated board-level fiducial components. For automated SMT assembly, the assembler must add at least three non-collinear global fiducials to the manufacturing panel and confirm optical access after rails/tooling are added. If the assembler requires local fiducials for the 1.5 mm WSON devices, pause the release and add them in Altium before placement.

## Module and substitute holds

- `U501` remains a physical incoming-inspection gate. Accept only the supplier revision whose 2x9 header spacing, board outline, pin order, 3V3/GND and SPI-related pins match the Altium carrier and repository checklist.
- `1N5711W-7-F` for obsolete `BAS70ZFILM` remains subject to analog clamp bench approval.
- `FDN304P` for unavailable `PMV48XP,215` remains subject to power-path/inrush approval.

## Output generation

- `Assembly.OutJob` generates a metric pick-and-place CSV, mirrored bottom/top assembly drawing PDF and STEP model from `EMG_v5_FAB_READY.PcbDoc`.
- The project Gerber X2 configuration includes top and bottom paste layers for stencil generation.
- Before release, regenerate the Altium DRC, Gerber X2, drill and assembly outputs from the same saved PCB revision. Archive the fresh schematic compile/ERC report with the assembly package.
