# EMG V5 Status

Date: 2026-07-07

## Current State

The V5 folder now contains a clean separate KiCad working base copied from the verified V4 project, plus V5 documentation and ESP32-H2 firmware scaffold.

Current branch / checkpoint:

- Branch: `v5/measured-dstk22807-footprint`
- Current local/remote HEAD: `3494631 sync kicad metadata and analog label placement`
- Remote tracking note: `github-emg/v5/measured-dstk22807-footprint` is at the same HEAD.
- Working tree baseline after push: clean.
- Recent pushed DSTK/SPI commits:
  - `79730dd connect dstk spi and mark unused pins`
  - `3494631 sync kicad metadata and analog label placement`
- Recent cleanup commits before DSTK SPI:
  - `0e23acf cleanup MCP6004 symbol metadata mismatch`
  - `39ff7c9 cleanup Schottky diode symbol metadata mismatch`
  - `ddb646e place unused U202B op amp unit safely`
- Earlier structural checkpoints:
  - `1e5d3b0 fix channel trs mapping and raw gain input polarity`
  - `080eef1 remove legacy root emg schematic block`
  - `60d5881 implement channel 2 analog schematic`
- Root schematic: `V5/EMG_v5.kicad_sch`
- Power/reference child sheet: `V5/POWER_REFERENCE_BLOCK.kicad_sch`
- Channel 1 analog child sheet: `V5/EMG_CHANNEL_1_ANALOG.kicad_sch`
- Channel 2 analog child sheet: `V5/EMG_CHANNEL_2_ANALOG.kicad_sch`
- Root schematic now keeps the hierarchical `EMG_CHANNEL_1_ANALOG`, `EMG_CHANNEL_2_ANALOG`, and `POWER_REFERENCE_BLOCK` structure.
- `POWER_REFERENCE_BLOCK` now receives `EMG1_RAW`, `EMG1_RECT`, `EMG1_ENV`, `EMG2_RAW`, `EMG2_RECT`, and `EMG2_ENV` as hierarchical inputs.

Current schematic status:

- Power/reference schematic block: **ADDED / REVIEWED ENOUGH FOR CURRENT PLANNING**
- Channel 1 RECT KiCad candidate: **IMPLEMENTED / REVIEWED / NOT FINAL HARDWARE APPROVAL**
- Channel 2 analog schematic: **IMPLEMENTED / NOT FINAL HARDWARE APPROVAL**
- PCB layout: **NOT STARTED / NO**
- Full two-channel analog EMG chain schematic: **CANDIDATES IMPLEMENTED / NOT FINAL HARDWARE APPROVAL**
- DSTK SPI-only integration: **CONNECTED / REVIEWED / NOT FINAL HARDWARE APPROVAL**
- DSTK external 3.3 V powering: **NOT APPROVED YET**
- USB plus external 3.3 V simultaneous connection: **NOT APPROVED**
- Human USB testing: **FORBIDDEN**
- 5V analog/ADC: **NOT APPROVED**

DSTK SPI/U501 status: **DSTK_SPI_CONNECTED_U501_UNUSED_PINS_MARKED_NOT_FINAL_HW_APPROVAL**.

- Commit `79730dd connect dstk spi and mark unused pins` connected the DSTK22807 SPI-only path and marked intentional U501 unused pins.
- Commit `3494631 sync kicad metadata and analog label placement` recorded KiCad sheet metadata sync plus analog reference/value placement cleanup.
- U501 was added to the root schematic with symbol `EMG_V5:DSTK22807_ESP32H2_SuperMini_Measured`.
- U501 footprint is `EMG_V5:DSTK22807_ESP32H2_SuperMini_Carrier_Measured_THT`.
- U501 GND / pin 11 is connected to root/system GND for SPI logic reference only.
- This GND connection is not DSTK power-source approval.
- MCP3208 SPI mapping is now:
  - `ADC_CS` -> U501 GPIO14 / pin 13
  - `ADC_SCLK` -> U501 GPIO13 / pin 14
  - `ADC_MOSI` -> U501 GPIO12 / pin 15
  - `ADC_MISO` -> U501 GPIO11 / pin 16
- U201 MCP3208 SPI mapping remains: CLK -> `ADC_SCLK`, Din -> `ADC_MOSI`, Dout -> `ADC_MISO`, CS/SHDN -> `ADC_CS`.
- `POWER_REFERENCE_BLOCK` exposes `ADC_CS`, `ADC_SCLK`, `ADC_MOSI`, and `ADC_MISO` as hierarchical labels/sheet pins to root.
- Analog channel mapping remains CH0 -> `EMG1_RAW`, CH1 -> `EMG1_RECT`, CH2 -> `EMG1_ENV`, CH3 -> `EMG2_RAW`, CH4 -> `EMG2_RECT`, CH5 -> `EMG2_ENV`, CH6 -> `VREF_MON`, CH7 -> `BAT_MON`.
- Explicit KiCad no_connect markers are used only for intentional unused U501 pins: TX, RX, GPIO0, GPIO1, GPIO2, GPIO3, GPIO4, GPIO5, GPIO8, 5V_VBUS, 3V3, GPIO10, and GPIO9.
- No ERC marker was used.
- no_connect markers are not placed on U501 GND or SPI-connected pins.
- U501 3V3 is not connected to any rail.
- U501 5V_VBUS is not connected to any rail.
- There is no DSTK 3V3 -> `3V3_ADC` connection.
- There is no 5V analog/ADC supply.
- USB/backfeed strategy remains unresolved.
- `DSTK_3V3_CANDIDATE` remains a power strategy placeholder.
- `V5/EMG_v5.kicad_pro` sheet metadata / sheets list sync was committed; `EMG_CHANNEL_2_ANALOG` is present in the sheet list.
- ERC/project/library settings were not intentionally changed by the metadata sync.
- Channel 1/2 analog sheet changes in the metadata/placement commit were limited to R301/R302/R303 and R401/R402/R403 reference/value property placement.
- No Channel 1/2 net, wire, pin, topology, value, or footprint change was made by that placement cleanup.

Final DSTK SPI/U501 ERC status: **DSTK_SPI_U501_ERC_ZERO_ERRORS_TWO_WARNINGS_NOT_FINAL_HW_APPROVAL**.

- Root ERC after DSTK SPI/U501 cleanup reported 2 total messages: 0 errors and 2 warnings.
- Remaining warnings:
  - U501 footprint library warning: current CLI config does not include the `EMG_V5` footprint library.
  - `DSTK_3V3_CANDIDATE` isolated pin label.
- Closed by DSTK SPI/U501 work:
  - U201 CLK/Din/`~CS/SHDN` input-not-driven errors.
  - `ADC_CS` / `ADC_SCLK` / `ADC_MOSI` / `ADC_MISO` isolated label warnings.
  - U501 unused pin-not-connected errors.
  - U501 RX input-not-driven error.
  - U501 3V3/5V_VBUS power input-not-driven errors.
- U302/U402 MCP6004 mismatch warnings remain absent.
- D331/D332/D431/D432 D_Schottky mismatch warnings remain absent.
- Unconnected wire endpoint warnings remain absent.
- U202 missing/unplaced unit warnings remain absent.
- No label-not-connected or pin-not-connected errors remain.
- No U201 DGND error remains.
- No #FLG conflict remains.
- No new real Channel 1/Channel 2 ERC error remains.

Cleanup series technical summary:

- MCP6004 cleanup: U302/U402 embedded MCP6004 symbol metadata mismatch was cleaned as datasheet/description metadata sync only. No pin, unit, net, topology, refdes, value, or footprint change was made.
- D_Schottky cleanup: D331/D332/D431/D432 embedded `Device:D_Schottky` metadata mismatch was cleaned as `ki_fp_filters` metadata sync only. They remain `Device:D_Schottky`, value `BAS70ZFILM`, footprint `Diode_SMD:D_SOD-123`, with pin convention pin 1 = K / cathode and pin 2 = A / anode. Rectifier topology and diode orientation were preserved.
- U202B unused unit cleanup: U202 is `MCP6002-xP` in `POWER_REFERENCE_BLOCK`; U202A remains the analog VREF buffer; the U202 power unit remains V+ -> `3V3_ADC` and V- -> GND. U202B/unit 2 was placed as a safe unused unity follower with pin 5 (+) to `analog VREF`, pin 6 (-) tied to pin 7 output, and pin 7 only local feedback. U202B output is not connected to `analog VREF`, `ADC_REF`, `3V3_ADC`, GND, `VREF_MON`, `BAT_MON`, or any other signal path.
- PCB has not been started.
- DSTK power strategy has not been solved.
- DSTK SPI is connected only as SPI logic integration.
- Final hardware approval has not been given.
- `ADC_REF` is not placed inside analog child sheets.
- `ADC_REF` is not merged with `analog VREF`.
- 5V analog/ADC remains not approved.
- `EMG1_REF_ELECTRODE` and `EMG2_REF_ELECTRODE` remain high-Z/reference electrode only and are not tied to GND/chassis/USB.
- No No ERC markers were added.

Channel TRS / RAW gain fix status: **CHANNEL_TRS_AND_RAW_GAIN_FIX_PUSHED_NOT_FINAL_HW_APPROVAL**.

- Commit `1e5d3b0 fix channel trs mapping and raw gain input polarity` has been pushed to `github-emg/v5/measured-dstk22807-footprint` as part of the current branch history.
- J301 TRS mapping is fixed: Tip/T -> `EMG1_IN_P`, Ring/R -> `EMG1_IN_N`, Sleeve/S -> `EMG1_REF_ELECTRODE`.
- J401 TRS mapping is fixed: Tip/T -> `EMG2_IN_P`, Ring/R -> `EMG2_IN_N`, Sleeve/S -> `EMG2_REF_ELECTRODE`.
- Physical TRS electrode cable measurement note: sari/yellow tip = Tip/T, yesil/green middle body = Ring/R, kirmizi/red rear body = Sleeve/S.
- With that physical measurement, the project mapping is now sari/yellow Tip/T -> `EMGx_IN_P`, yesil/green Ring/R -> `EMGx_IN_N`, and kirmizi/red Sleeve/S -> `EMGx_REF_ELECTRODE`.
- U302A RAW gain topology is fixed: `RAW_HPF_NODE` -> U302A non-inverting input / +, `RAW_GAIN_FB` -> U302A inverting input / -, and the U302A output downstream RAW path is preserved.
- U402A RAW gain topology is fixed: `EMG2_RAW_HPF_NODE` -> U402A non-inverting input / +, `EMG2_RAW_GAIN_FB` -> U402A inverting input / -, and the U402A output downstream RAW path is preserved.
- Preserved boundaries: U301/U401 INA polarity unchanged; U302D/U402D ENV buffer unchanged; RECT path unchanged; ENV path unchanged; MCP3208/U201 mapping unchanged; root schematic unchanged; `POWER_REFERENCE_BLOCK` unchanged.
- PCB was not started, DSTK power was not connected, no No ERC markers were added, and this is not final hardware approval.
- Root ERC in the earlier TRS/RAW post-fix review reported 29 total messages: 3 errors and 26 warnings; this historical result has since been superseded by the DSTK SPI/U501 review result of 2 total messages: 0 errors and 2 warnings.
- Former U201 SPI placeholder errors are now closed by the DSTK SPI-only integration.
- No unexpected post-fix errors were reported: no new Channel 1/Channel 2 error, no U201 CH0-CH5 input error, no DGND error, no GND / `3V3_ADC` / `analog VREF` label-not-connected error, no #FLG conflict, and no D331/D332/D431/D432 pin-not-connected error.

Channel 2 and legacy root cleanup status: **CHANNEL_2_IMPLEMENTED_LEGACY_ROOT_REMOVED_NOT_FINAL_HW_APPROVAL**.

- Commit `60d5881 implement channel 2 analog schematic` created `V5/EMG_CHANNEL_2_ANALOG.kicad_sch`.
- Commit `60d5881` also updated `V5/EMG_v5.kicad_sch` and `V5/POWER_REFERENCE_BLOCK.kicad_sch`.
- Channel 2 was created as a controlled copy/adaptation of Channel 1.
- Channel 2 uses +100 refdes mapping: J401, U401, U402, R4xx, C4xx, and D431/D432.
- Channel 2 uses `EMG2_*` nets.
- Shared rails remain `3V3_ADC`, `analog VREF`, and GND.
- `ADC_REF` was not introduced into the Channel 2 analog sheet.
- `EMG2_REF_ELECTRODE` remains isolated from GND/chassis/USB.
- No No ERC markers were added.
- PCB was not started.
- At that checkpoint, DSTK SPI/power was not connected; DSTK SPI has since been connected in commit `79730dd`.
- EMG2 outputs are integrated with MCP3208 U201: `EMG2_RAW -> CH3`, `EMG2_RECT -> CH4`, and `EMG2_ENV -> CH5`.
- Previous U201 CH3/CH4/CH5 placeholder input errors disappeared.
- Channel 2 inherited the Channel 1 BOM/footprint strategy: D431/D432 use `BAS70ZFILM`, `Device:D_Schottky`, and `Diode_SMD:D_SOD-123`; U402 uses MCP6004 and `Package_DIP:DIP-14_W7.62mm`; Channel 2 RECT resistors inherit the 0805 resistor footprint strategy; relevant capacitors inherit the 0805 capacitor footprint strategy.
- Commit `080eef1 remove legacy root emg schematic block` removed the old standalone/legacy root EMG schematic block from `V5/EMG_v5.kicad_sch`.
- The removed legacy flat block included the old J1/U1/U2/J2-style EMG circuit.
- Root now keeps the hierarchical Channel 1, Channel 2, and `POWER_REFERENCE_BLOCK` structure.
- Root GND PWR_FLAG/source declaration was restored cleanly after legacy block deletion.
- The old legacy block was not restored.
- GUI/ERC target after Channel 2 implementation and legacy cleanup reached 3 remaining errors.
- Remaining errors are expected SPI placeholders only: U201 CLK input not driven, U201 Din input not driven, and U201 `~CS/SHDN` input not driven.
- U201 CH3/CH4/CH5 errors are gone.
- U201 DGND error was fixed.
- GND label-not-connected error was fixed.
- #FLG power-output conflict was fixed.
- `3V3_ADC` / `analog VREF` label-not-connected errors were fixed.
- No D431/D432 pin-not-connected errors appeared.
- No new real Channel 1/Channel 2 errors remain.
- Remaining caveats: this is not final hardware approval; D331/D332/D431/D432 cathode-band orientation must be visually reviewed before PCB; exact passive ordering MPNs remain unlocked; DSTK power remains unresolved; PCB remains not started. U302/U402 MCP6004 and D331/D332/D431/D432 D_Schottky metadata mismatch warnings have since been cleaned.

Channel 1 analog / RECT implementation status at checkpoint `8f2bf9e`:

- Channel 1 analog schematic remains polarity fixed.
- `V5/EMG_CHANNEL_1_ANALOG.kicad_sch` now includes a reviewable Channel 1 VREF-centered absolute-value rectifier candidate.
- RECT is no longer only a blocked placeholder in the KiCad schematic.
- Channel 1 RECT KiCad candidate: **IMPLEMENTED / REVIEWED / CLEANED READABILITY / FOOTPRINT FIELDS ASSIGNED / DIODE VALUES CLARIFIED / NOT FINAL HARDWARE APPROVAL**.
- Checkpoint `3825181 cleanup channel 1 rectifier schematic readability` applied the Channel 1 RECT schematic readability cleanup.
- Checkpoint `eca2cc3 assign channel 1 rectifier footprints` assigned Channel 1 RECT/ENV-related schematic footprint fields.
- Checkpoint `8f2bf9e clarify channel 1 rectifier diode values` clarified the visible D331/D332 value text.
- Cleanup target was `V5/EMG_CHANNEL_1_ANALOG.kicad_sch` only.
- Footprint assignment edit target was also `V5/EMG_CHANNEL_1_ANALOG.kicad_sch` only.
- Diode value-text clarification target was also `V5/EMG_CHANNEL_1_ANALOG.kicad_sch` only.
- RECT block readability was improved without intentional topology/function change.
- Footprint assignment diff was limited to footprint property fields; no value, net, or topology changes were intended.
- Diode value-text clarification changed D331/D332 visible value properties only; no net, topology, symbol, footprint, or orientation change was made.
- R332 / the old RECT placeholder marker was fully removed.
- `EMG1_RECT_PLACEHOLDER_REMOVED` is no longer present.
- Stale/forbidden strings were checked and absent from the cleaned child sheet: `ADC_REF`, `No ERC`, `R_RECT_PLACEHOLDER`, `EMG1_RECT_PLACEHOLDER_REMOVED`, `RECT BLOCKED PLACEHOLDER`, `ENV placeholder`, `not functional until RECT`, and `TP_analog_VREF`.
- `EMG1_RECT_DRV` is no longer passively biased to `analog VREF` through the old 1M placeholder as the active implementation.
- U302B and U302C were repurposed from reserved followers into the two-op-amp RECT candidate.
- U302D remains the ENV buffer.
- D331/D332 Schottky candidate diodes were added.
- R333-R338 were added for the 10k/20k rectifier resistor network.
- New internal nets include `EMG1_RECT_SUM1`, `EMG1_RECT_A1_OUT`, `EMG1_RECT_U302B_DRV`, and `EMG1_RECT_SUM2`.
- Output remains `EMG1_RECT_DRV -> R331 470R -> EMG1_RECT`, with `C331 1nF` to GND.
- ENV path follows `EMG1_RECT_DRV` through `R341 33k`, `C341 1uF` to `analog VREF`, U302D buffer, and `EMG1_ENV_DRV`.
- `ADC_REF` remains absent from the analog child sheet.
- `EMG1_REF_ELECTRODE` remains isolated from GND.
- No No ERC markers were added.
- No 5V analog/ADC use was introduced.
- `EMG1_RECT` and `EMG1_ENV` are no longer merely placeholder-only in KiCad, but still need PCB/hardware review before layout.
- Channel 2 was later implemented as a controlled schematic duplicate/adaptation of Channel 1 at checkpoint `60d5881`.
- PCB remains **NOT STARTED**.
- DSTK power remains **NOT CONNECTED / NOT APPROVED**; DSTK SPI-only logic wiring is now connected.
- Exact MCP600x vendor model and exact diode model limitations still remain.
Channel 1 RECT post-implementation review result:

- Post-review verdict: **POST_REVIEW_PASS_WITH_CLEANUP_RECOMMENDED**.
- The implemented topology matches the planned VREF-centered rectifier candidate at schematic-connectivity level.
- This remains reviewable and is not final hardware approval.
- U302B role was confirmed: `+` input -> `analog VREF`, `-` input -> `EMG1_RECT_SUM1`, output -> `EMG1_RECT_U302B_DRV`.
- U302C role was confirmed: `+` input -> `analog VREF`, `-` input -> `EMG1_RECT_SUM2`, output -> `EMG1_RECT_DRV`.
- D331/D332 orientation matches the implementation plan using the local KiCad `Device:D_Schottky` pin convention.
- R333-R338 match the planned 10k/20k rectifier resistor network.
- R332 / old RECT placeholder marker has now been fully removed and no longer biases `EMG1_RECT_DRV`.
- `EMG1_RECT_DRV` is driven by U302C output.
- `EMG1_RECT_DRV -> R331 470R -> EMG1_RECT` remains intact.
- `C331 1nF` to GND remains intact.
- ENV path remains `EMG1_RECT_DRV -> R341 33k -> ENV_LPF_NODE`, `C341 1uF` to `analog VREF`, U302D buffer -> `EMG1_ENV_DRV`.
- `ADC_REF` remains absent from the analog child sheet.
- `EMG1_REF_ELECTRODE` remains isolated from GND.
- No No ERC markers were added.
- No new 5V analog/ADC use was introduced.

Cleanup/checks before Channel 2 or PCB:

- RECT block readability cleanup was applied at checkpoint `3825181`.
- Channel 1 RECT remains reviewable, not final hardware approval.
- Final pre-duplication review verdict: **CHANNEL_1_READY_FOR_CHANNEL_2_DUPLICATION**.
- Channel 1 ready status: **CHANNEL_1_READY_FOR_CHANNEL_2_DUPLICATION_NOT_FINAL_HW_APPROVAL**.
- Issues requiring fix before Channel 2: none found in the reviewed gates.
- Channel 2 may start as a controlled schematic duplicate/adaptation of Channel 1.
- This is still not PCB approval and not final hardware approval.
- Confirmed pass item: RAW path still reaches `EMG1_RAW_DRV`, then `R321 -> EMG1_RAW` with `C321` to GND.
- Confirmed pass item: RECT path is present around U302B/U302C, D331/D332, and R333-R338.
- Confirmed pass item: D331/D332 values are `BAS70ZFILM`, symbol `Device:D_Schottky`, footprint `Diode_SMD:D_SOD-123`.
- Confirmed pass item: ENV path remains `EMG1_RECT_DRV -> R341 -> ENV_LPF_NODE`, `C341` to `analog VREF`, U302D -> `EMG1_ENV_DRV`.
- Confirmed pass item: output paths remain `EMG1_RECT_DRV -> R331 -> EMG1_RECT` and `EMG1_ENV_DRV -> R351 -> EMG1_ENV`.
- Confirmed pass item: `ADC_REF` is absent from the child sheet.
- Confirmed pass item: no No ERC markers were added.
- Confirmed pass item: old RECT placeholders and `BAT54/BAS70` text are absent.
- Confirmed pass item: `EMG1_REF_ELECTRODE` remains isolated from GND.
- Confirmed pass item: docs match schematic fields for D331/D332, U302, R333-R338, and C321/C331/C351/C341/C360.
- D331/D332 and MCP6004 symbol metadata mismatch warnings have since been reviewed and cleaned; exact ordering/package/orientation review still remains before PCB.
- D331/D332 diode MPN/footprint direction is recorded for first-prototype planning, and schematic footprint fields were assigned at checkpoint `eca2cc3`.
- D331/D332 visible schematic value text was clarified at checkpoint `8f2bf9e`.
- D331 value is now `BAS70ZFILM`.
- D332 value is now `BAS70ZFILM`.
- The previous visible `BAT54/BAS70` ambiguity is removed from the Channel 1 analog schematic.
- D331/D332 KiCad symbol convention: `Device:D_Schottky` uses pin 1 = K / cathode and pin 2 = A / anode.
- D331/D332 still use KiCad symbol `Device:D_Schottky` and footprint `Diode_SMD:D_SOD-123`.
- Current D331/D332 orientation matches the previous Channel 1 RECT review: D331 K -> `EMG1_RECT_U302B_DRV`, D331 A -> `EMG1_RECT_A1_OUT`; D332 K -> `EMG1_RECT_SUM1`, D332 A -> `EMG1_RECT_U302B_DRV`.
- Do not use `BAS70FILM` with a plain 2-pin `Device:D_Schottky` symbol plus generic 3-pad SOT-23 footprint for the first prototype.
- `BAS70FILM` is deferred because the ST SOT-23 single diode uses two active package pins plus a third unused/NC-style pad and needs explicit custom mapping or a project-local 3-pin symbol/footprint review.
- Recommended D331/D332 first-prototype direction: MPN `BAS70ZFILM`, package SOD-123, KiCad symbol `Device:D_Schottky`, KiCad footprint `Diode_SMD:D_SOD-123`.
- `BAS70ZFILM` is preferred because the two-terminal SOD-123 package avoids SOT-23 third-pad / NC ambiguity and maps cleanly to the 2-pin diode symbol.
- Avoid `BAS70-04`, `BAS70-05`, `BAS70-06`, and BAT54A/C/S dual/common variants for this rectifier pass.
- Before PCB/layout, visually verify D331/D332 cathode-band orientation against the KiCad footprint and rectifier schematic.
- U302 MCP6004 MPN/package/footprint verification verdict: **MCP6004_PDIP14_SOCKETED_FIRST_PROTO_READY**.
- U302 uses KiCad symbol `Amplifier_Operational:MCP6004`.
- U302 units are present as one package: U302A, U302B, U302C, U302D, plus power unit.
- U302 pin mapping matches MCP6004/LM324-style quad op-amp pinout: U302A 1=OUT, 2=-, 3=+; U302B 7=OUT, 6=-, 5=+; U302C 8=OUT, 9=-, 10=+; U302D 14=OUT, 13=-, 12=+; power 4=V+, 11=V-.
- U302 power unit is present with V+ -> `3V3_ADC` and V- -> GND.
- C360 100nF local decoupling is present.
- MCP6004 caveat: the schematic previously had an MCP6004 symbol/library mismatch or stale metadata warning, but functional pin numbering matched the expected MCP6004 pinout. The embedded MCP6004 metadata mismatch has since been cleaned without pin/unit/net/topology/refdes/value/footprint change.
- Recommended U302 first-prototype direction: MPN `MCP6004-I/P`, package PDIP-14, KiCad symbol `Amplifier_Operational:MCP6004`, KiCad footprint `Package_DIP:DIP-14_W7.62mm`.
- Use socketed PDIP for first prototype bring-up/debug.
- Recommended compact U302 alternative: MPN `MCP6004-I/SL`, package SOIC-14, KiCad footprint `Package_SO:SOIC-14_3.9x8.7mm_P1.27mm`.
- MCP6004-I/ST / TSSOP-14 is only a later compact option with KiCad footprint `Package_SO:TSSOP-14_4.4x5mm_P0.65mm`; it is not recommended for the first prototype because hand bring-up/debug and inspection are worse than PDIP/SOIC.
- Before PCB, keep the MCP6004 package/MPN/ordering review open even though the MCP6004 metadata mismatch warning has been cleaned.
- Before PCB, confirm MCP6004 input common-mode range and output swing at `3V3_ADC = 3.3 V`.
- Before PCB, confirm rectifier crossover behavior and output stability with ADC RC caps and ENV/storage load.
- Exact MCP6004 vendor LTspice model is still not locally locked.
- Final MCP6004 purchase-page/datasheet package check is still required before ordering.
- Channel 1 RECT resistor matching verification verdict: **RECT_RESISTOR_MATCHING_DECISION_READY**.
- Confirmed RECT resistor roles: `R333 = 10k` RIN1, `R334 = 10k` RF1, `R335 = 1M` A1 bias / off-state return toward `analog VREF`, `R336 = 20k` RRAW2, `R337 = 10k` RA12, and `R338 = 20k` RF2.
- Ratio-critical resistors: R333/R334 as the 10k/10k first-stage pair, and R336/R337/R338 for the weighted summer.
- Key weighted-summer ratios: `R336 = R338 = 20k` and `R338/R337 = 2`.
- `R335 = 1M` is not ratio-critical; it is mainly bias/leakage/off-state related.
- First-prototype resistor decision: use 0.1% thin-film, 0805 for R333, R334, R336, R337, and R338.
- Use the same manufacturer/series/package/tempco for the ratio-critical resistors where possible.
- Reason: the first prototype should separate rectifier topology error from resistor mismatch.
- `R335 = 1M` may be 1% thin-film 0805; 0.1% is optional only for BOM simplicity or later sensitivity findings.
- Compact / cost-down alternatives: 0603 thin-film can be used later if layout density matters, and 1% thin-film may be considered after tolerance sensitivity or bench validation.
- 1% thick-film is probably functional but is not preferred for first validation of the rectifier candidate.
- Remaining risk: run tolerance sensitivity or bench check before relaxing ratio-critical parts to 1%.
- Exact resistor MPN/series/tempco is still not locked.
- MCP6004 offset/swing/crossover and BAS70 leakage may dominate small-signal error.
- Layout should keep the ratio network close and thermally similar.
- Channel 1 RAW/RECT/ENV capacitor dielectric/package verification verdict: **RECT_ENV_CAPACITOR_DECISION_READY**.
- Confirmed capacitor roles: `C321 = 1nF` RAW ADC/output cap to GND, `C331 = 1nF` RECT ADC/output cap to GND, `C351 = 4.7nF` ENV ADC/output cap to GND, `C341 = 1uF` ENV LPF storage cap from `ENV_LPF_NODE` to `analog VREF`, and `C360 = 100nF` U302 local decoupling cap.
- First-prototype capacitor decision for C321/C331: 1nF C0G/NP0, 0805, 5% or better preferred.
- First-prototype capacitor decision for C351: 4.7nF C0G/NP0, 0805 preferred; X7R is an acceptable fallback if C0G/NP0 availability becomes difficult.
- First-prototype capacitor decision for C341: 1uF X7R, 0805, 10% acceptable, with 16V or 25V rating to reduce DC-bias derating.
- First-prototype capacitor decision for C360: 100nF X7R, 0805, 16V or 25V local decoupling.
- Avoid Y5V/Z5U for these analog/output/decoupling capacitors.
- Compact alternative: 0603 can be considered later if layout density matters; keep C321/C331/C351 as C0G/NP0 where practical and keep C341/C360 as X7R.
- Remaining capacitor risk: C341 tolerance and DC-bias derating shift the ENV cutoff.
- Remaining capacitor risk: C341 returns to `analog VREF`, so VREF buffer stability/loading should be reviewed.
- Remaining capacitor risk: MCP6004 output stability with ADC/output caps should remain protected by 470R series resistors, but still needs review.
- Exact capacitor MPN/voltage/tolerance is not locked yet.
- Channel 1 RECT/ENV footprint field assignment status: **CHANNEL_1_RECT_FOOTPRINT_FIELDS_ASSIGNED_NOT_FINAL_HW_APPROVAL**.
- Assigned schematic footprint fields at checkpoint `eca2cc3`:
  - D331 = `Diode_SMD:D_SOD-123`
  - D332 = `Diode_SMD:D_SOD-123`
  - U302 = `Package_DIP:DIP-14_W7.62mm`
  - R333 = `Resistor_SMD:R_0805_2012Metric`
  - R334 = `Resistor_SMD:R_0805_2012Metric`
  - R335 = `Resistor_SMD:R_0805_2012Metric`
  - R336 = `Resistor_SMD:R_0805_2012Metric`
  - R337 = `Resistor_SMD:R_0805_2012Metric`
  - R338 = `Resistor_SMD:R_0805_2012Metric`
  - C321 = `Capacitor_SMD:C_0805_2012Metric`
  - C331 = `Capacitor_SMD:C_0805_2012Metric`
  - C351 = `Capacitor_SMD:C_0805_2012Metric`
  - C341 = `Capacitor_SMD:C_0805_2012Metric`
  - C360 = `Capacitor_SMD:C_0805_2012Metric`
- Root GUI ERC after footprint assignment remained 6 errors and 21 warnings.
- The 6 errors are still the expected U201 placeholder input errors: CH3, CH4, CH5, CLK, Din, and `~CS/SHDN`.
- No new D331/D332 pin-not-connected errors appeared after footprint assignment.
- No new real RECT/ENV ERC errors appeared after footprint assignment.
- D331/D332 schematic value text now says `BAS70ZFILM`; the previous visible `BAT54/BAS70` ambiguity is removed.
- D331/D332 still use symbol `Device:D_Schottky` and footprint `Diode_SMD:D_SOD-123`.
- No net, topology, footprint, symbol, or diode orientation change was made by the value-text clarification.
- Exact ordering MPNs for passives are not locked yet.
- D331/D332 cathode-band footprint orientation must be visually checked before PCB.
- MCP6004 package/MPN/ordering review still remains before PCB, although the MCP6004 metadata mismatch warning has since been cleaned.
- Channel 2 may start as a controlled schematic duplicate/adaptation of Channel 1.
- Do not start PCB.

Final Channel 1 RECT status: **CHANNEL_1_READY_FOR_CHANNEL_2_DUPLICATION_NOT_FINAL_HW_APPROVAL**.

D331/D332 diode MPN/footprint decision status: **CHANNEL_1_RECT_DIODE_MPN_FOOTPRINT_DECISION_READY_NOT_FINAL_HW_APPROVAL**.

U302 MCP6004 MPN/footprint decision status: **CHANNEL_1_RECT_MCP6004_MPN_FOOTPRINT_DECISION_READY_NOT_FINAL_HW_APPROVAL**.

Channel 1 RECT resistor matching decision status: **CHANNEL_1_RECT_RESISTOR_MATCHING_DECISION_READY_NOT_FINAL_HW_APPROVAL**.

Channel 1 RAW/RECT/ENV capacitor decision status: **CHANNEL_1_RECT_ENV_CAPACITOR_DECISION_READY_NOT_FINAL_HW_APPROVAL**.

Channel 1 RECT/ENV footprint field assignment status: **CHANNEL_1_RECT_FOOTPRINT_FIELDS_ASSIGNED_NOT_FINAL_HW_APPROVAL**.

Channel 1 RECT diode value-text status: **CHANNEL_1_RECT_DIODE_VALUES_CLARIFIED_NOT_FINAL_HW_APPROVAL**.

Channel 1 pre-duplication review status: **CHANNEL_1_READY_FOR_CHANNEL_2_DUPLICATION_NOT_FINAL_HW_APPROVAL**.

Channel 2 implementation and legacy root cleanup status: **CHANNEL_2_IMPLEMENTED_LEGACY_ROOT_REMOVED_NOT_FINAL_HW_APPROVAL**.

- Checkpoint `60d5881 implement channel 2 analog schematic` created `V5/EMG_CHANNEL_2_ANALOG.kicad_sch`.
- Root sheet `V5/EMG_v5.kicad_sch` was updated.
- `V5/POWER_REFERENCE_BLOCK.kicad_sch` was updated.
- Channel 2 was created as a controlled copy/adaptation of Channel 1.
- Channel 2 uses +100 refdes mapping: J401, U401, U402, R4xx, C4xx, and D431/D432.
- Channel 2 uses `EMG2_*` nets and shared rails `3V3_ADC`, `analog VREF`, and GND.
- `ADC_REF` was not introduced.
- `EMG2_REF_ELECTRODE` remains isolated from GND/chassis/USB.
- No No ERC markers were added.
- PCB was not started.
- At that checkpoint, DSTK SPI/power was not connected; DSTK SPI has since been connected in commit `79730dd`.
- D431/D432 inherited the Channel 1 diode decision: `BAS70ZFILM`, `Device:D_Schottky`, `Diode_SMD:D_SOD-123`.
- U402 inherited the Channel 1 op-amp package direction: MCP6004, `Package_DIP:DIP-14_W7.62mm`.
- Channel 2 RECT resistors inherit the 0805 resistor footprint strategy.
- Channel 2 relevant capacitors inherit the 0805 capacitor footprint strategy.
- `EMG2_RAW` is connected to U201 CH3.
- `EMG2_RECT` is connected to U201 CH4.
- `EMG2_ENV` is connected to U201 CH5.
- Previous U201 CH3/CH4/CH5 placeholder errors disappeared.
- Checkpoint `080eef1 remove legacy root emg schematic block` removed the old standalone/legacy root EMG schematic block from `V5/EMG_v5.kicad_sch`.
- The removed legacy flat block used the old J1/U1/U2/J2-style EMG circuit.
- Root now keeps only the hierarchical `EMG_CHANNEL_1_ANALOG`, `EMG_CHANNEL_2_ANALOG`, and `POWER_REFERENCE_BLOCK` structure.
- Root GND PWR_FLAG/source declaration was restored cleanly after legacy block deletion.
- The old legacy block was not restored.

Historical root ERC status after Channel 2 implementation and legacy root cleanup:

- GUI/ERC target reached: 3 errors remain.
- Remaining errors are expected SPI placeholders only:
  - U201 CLK input not driven
  - U201 Din input not driven
  - U201 `~CS/SHDN` input not driven
- U201 CH3/CH4/CH5 errors are gone.
- U201 DGND error was fixed.
- GND label-not-connected error was fixed.
- #FLG power-output conflict was fixed.
- `3V3_ADC` / `analog VREF` label-not-connected errors were fixed.
- No D431/D432 pin-not-connected errors appeared.
- No new real Channel 1/Channel 2 errors remain.
- This historical ERC status has since been superseded by the DSTK SPI/U501 result of 0 errors and 2 known warnings.

## Rectifier LTspice Simulation Status

Simulation planning/prototype files now exist for Channel 1 RECT review:

- `V5/sim/rectifier/RECTIFIER_LTSPICE_PLAN.md`
- `V5/sim/rectifier/emg_vref_abs_rectifier_candidate.cir`
- `V5/sim/rectifier/emg_vref_abs_rectifier_real_topology_candidate.cir`
- `V5/sim/rectifier/RECTIFIER_MODEL_CONFIRMATION.md`
- `V5/sim/rectifier/emg_vref_abs_rectifier_conservative_rrio_candidate.cir`

Behavioral target/load simulation status:

- Behavioral target/load simulation was created first.
- It validates the intended behavior: `RECT_OUT = VREF + abs(RAW_IN - VREF)`.
- Behavioral `RECT_OUT` tracks `IDEAL_RECT`.
- `ERR` is near 0.
- `EMG1_RECT_ADC` follows `RECT_OUT` through the 470R/1nF load path.
- `EMG1_ENV_DRV` gives plausible smoothing.
- This is behavioral target/load validation only, not a real op-amp/diode implementation.

Real topology simulation status:

- A real-component LTspice candidate was added.
- It uses explicit op-amp stages, explicit Schottky diode approximations, and explicit resistor networks.
- `abs()` is used only for `IDEAL_RECT` comparison, not inside the rectifier core.
- Initial LTspice review showed the nominal 0.10 V amplitude case tracks `IDEAL_RECT` closely.
- Initial LTspice review showed the 0.50 V amplitude case remains close with only small crossover/spike error.
- Initial LTspice review showed the 1.00 V amplitude stress case reaches about 2.65 V and does not clip at 3.3 V.
- `ERR` is generally small, with mV-level crossover spikes in stress cases.
- This makes the topology **REVIEWABLE**, not automatically KiCad-approved.

Model limitations:

- Exact MCP6004/MCP6002 LTspice model was not used.
- Exact BAS70/BAT54 LTspice model was not used.
- Generic RRIO op-amp and generic Schottky approximations were used.
- Because of these limitations, KiCad implementation still requires manual schematic/topology review.

Model confirmation status:

- `V5/sim/rectifier/RECTIFIER_MODEL_CONFIRMATION.md` was added.
- A conservative RRIO simulation candidate was added: `V5/sim/rectifier/emg_vref_abs_rectifier_conservative_rrio_candidate.cir`.
- Exact MCP6001/MCP6002/MCP6004/MCP600x local LTspice vendor models were not found.
- Exact BAS70 model was not found.
- Local LTspice Toshiba TBAT54 encrypted models were found under LTspice Contrib, but were not copied into the repo.
- The conservative candidate uses finite gain, finite bandwidth, limited output swing generic RRIO behavior and a generic Schottky approximation.
- Initial conservative RRIO candidate failed because the op-amp macromodel transconductance source polarity was reversed.
- Broken form: `GIN INT 0 ...`.
- Corrected form: `GIN 0 INT ...`.
- This was a macromodel polarity bug, not a topology change.
- Rectifier core, resistor network, diode orientation, VREF references, RAW input, `IDEAL_RECT`, `ERR`, and load path were preserved.
- After the fix, the conservative nominal case tracks `IDEAL_RECT`.
- The conservative 1.00 V stress case reaches about 2.65 V and does not clip at 3.3 V.
- `ERR` is no longer volt-level and remains near 0 visually, with remaining exact error still model-dependent.
- `EMG1_RECT_ADC` follows `RECT_OUT`.
- `EMG1_ENV_DRV` gives plausible smoothing.
- Conservative RRIO simulation is now **PASS / REVIEWABLE**.
- Exact MCP600x vendor model is still not available locally.
- Exact BAS70/BAT54 vendor model is still not included in the repo.
- Therefore this remains **REVIEWABLE**, not final hardware approval or KiCad approval.

Final rectifier simulation/model status: **RECTIFIER_SIMULATION_MODEL_CONFIRMATION_REVIEWABLE_NOT_KICAD_APPROVED**.

KiCad RECT implementation status:

- Channel 1 RECT KiCad candidate: **READY FOR CHANNEL 2 DUPLICATION / NOT FINAL HARDWARE APPROVAL**.
- KiCad schematic has been updated with the Channel 1 RECT candidate in `V5/EMG_CHANNEL_1_ANALOG.kicad_sch`.
- Checkpoint `3825181` applied readability cleanup only to `V5/EMG_CHANNEL_1_ANALOG.kicad_sch`.
- `EMG1_RECT_DRV` is no longer placeholder-biased to `analog VREF` through the old active 1M placeholder.
- `EMG1_RECT` and `EMG1_ENV` are no longer merely placeholder-only in KiCad, but still need PCB/hardware review before layout.
- Channel 2 has since been implemented as a controlled schematic duplicate/adaptation of Channel 1 at checkpoint `60d5881`.
- PCB remains **NOT STARTED**.
- DSTK power remains **NOT CONNECTED / NOT APPROVED**; DSTK SPI-only logic wiring is now connected.
- This is not final hardware approval and not production ready.

## Important

This is not a finished V5 fabrication layout yet.

The V5 schematic and PCB still need these planned follow-up items:

- DSTK22807 carrier footprint
- power/regulator/BAT_MON details
- V5 test points and silkscreen safety warnings

PCB layout must not start for the DSTK22807 carrier until the physical board is measured and pin labels are confirmed.

## Next Actions

- Continue with review/planning only; do not treat the TRS/RAW gain fix as final hardware approval.
- Review D331/D332 `BAS70ZFILM` / `Diode_SMD:D_SOD-123` cathode-band orientation visually against the schematic before PCB.
- Review D431/D432 `BAS70ZFILM` / `Diode_SMD:D_SOD-123` cathode-band orientation visually against the schematic before PCB.
- Keep U302 `MCP6004-I/P` / `Package_DIP:DIP-14_W7.62mm` and U402 `MCP6004` / `Package_DIP:DIP-14_W7.62mm` package/orderability review open before PCB; the MCP6004 metadata mismatch warning itself has been cleaned.
- Lock exact ordering MPNs for R333/R334/R336/R337/R338 as 0.1% thin-film 0805 from the same series where possible; keep R335 as 1% thin-film 0805 unless BOM simplification or sensitivity results justify 0.1%.
- Lock exact ordering MPNs for C321/C331 as 1nF C0G/NP0 0805, C351 as 4.7nF C0G/NP0 0805 preferred, and C341/C360 as X7R 0805 with suitable voltage ratings before Channel 2 or PCB.
- Confirm ENV behavior and analog VREF buffer loading after RECT/capacitor decisions.
- Plan DSTK power-source strategy separately; DSTK SPI-only logic wiring is now connected.
- Current root ERC review result is 0 errors plus 2 known warnings; do not add No ERC markers for the remaining planning/library warnings.
- Do not start PCB layout yet.
- Do not connect DSTK22807 3V3 or 5V/VBUS to analog/ADC rails until power behavior is verified and explicitly approved.
- Do not treat the KiCad/LTspice RECT candidates as final hardware approval or production readiness.

## Next Required Input

Measure or photograph the DSTK22807 board clearly enough to confirm:

- board length and width
- header/castellated pitch
- exact pin labels
- antenna location and keepout
- USB connector overhang
- usable SPI-capable pins
