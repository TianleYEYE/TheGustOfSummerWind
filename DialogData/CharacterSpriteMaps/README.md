# Character Sprite Maps

This folder stores dialogue-fill mappings and validation scripts for character portrait assets.

## Lan

Current temporary portrait setup:

- Dialogue CSV/XLSX files must match `FDialogStruct` top-level fields:
  `Name`, `Dialog`, `Character_1`, `Character_2`, `Character_3`, `Background`, `BackgroundSound`, `ConversationalVoice`.
- Lan portrait data is stored inside the `Character_1` struct cell, not as flat `CharacterId`, `CurrentSprite`, or `Face` columns.
- `Character_1.CharacterId`: `Lan`
- `Character_1.CurrentSprite`: `/Game/Assets/VerticalPainting/Lan/arashi_pose01_relaxed_base_noface.arashi_pose01_relaxed_base_noface`
- `Character_1.Face`: `None` until the matching expression overlays for the new base poses are ready.

The current fill covers 71 Lan dialogue rows across `DialogDataTable_1.CSV` and `DialogDataTable_2.csv` through `DialogDataTable_16.csv`.

## Commands

Dry-run current Lan portrait fill:

```powershell
python "D:\Unreal Projects\TheGustOfSummerWind\DialogData\CharacterSpriteMaps\fill_lan_current_portraits.py"
```

Apply current Lan portrait fill:

```powershell
python "D:\Unreal Projects\TheGustOfSummerWind\DialogData\CharacterSpriteMaps\fill_lan_current_portraits.py" --apply
```

Validate current Lan portrait struct cells:

```powershell
python "D:\Unreal Projects\TheGustOfSummerWind\DialogData\CharacterSpriteMaps\validate_arashi_dialog_sprites.py"
```

## Legacy Mapping

`Arashi_LayeredSprites.*` and `fill_arashi_dialog_sprites.py` describe the older single-base layered setup using `无脸` plus expression overlays. Do not apply that old fill to the current CSVs unless the matching expression overlays are intentionally restored.
