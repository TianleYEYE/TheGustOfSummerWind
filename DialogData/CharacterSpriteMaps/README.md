# Character Sprite Maps

This folder stores dialogue-fill mappings for character sprite assets.

## Arashi / Lan

- Mapping JSON: `Arashi_LayeredSprites.json`
- Mapping CSV: `Arashi_LayeredSprites.csv`
- Fill report: `Arashi_DialogData_fill_report.md`
- Fill script: `fill_arashi_dialog_sprites.py`
- Validation script: `validate_arashi_dialog_sprites.py`

Current validated mapping:

- `CurrentSprite`: `/Game/Assets/VerticalPainting/Lan/无脸.无脸`
- `Face default`: `/Game/Assets/VerticalPainting/Lan/默认png.默认png`
- `Face thinking`: `/Game/Assets/VerticalPainting/Lan/思考表情png.思考表情png`
- `Face dejected`: `/Game/Assets/VerticalPainting/Lan/沮丧表情png.沮丧表情png`

## Fill Status

- Updated 66 Lan dialogue rows across 11 standard `DialogDataTable_*.csv` files.
- Skipped `DialogDataTable_2.csv` because its schema uses `Face_1`/`Face_2` and does not include `CurrentSprite`/`Face`.
- Backup directory: `DialogData/_backup_arashi_layered_sprite_fill_20260620_133133`

Do not use full-composite legacy sprites as `Face`; `Face` must be the expression overlay texture.

## Commands

Dry-run fill check:

```powershell
python "D:\Unreal Projects\TheGustOfSummerWind\DialogData\CharacterSpriteMaps\fill_arashi_dialog_sprites.py"
```

Apply fill to new empty Lan rows:

```powershell
python "D:\Unreal Projects\TheGustOfSummerWind\DialogData\CharacterSpriteMaps\fill_arashi_dialog_sprites.py" --apply
```

Validate filled rows:

```powershell
python "D:\Unreal Projects\TheGustOfSummerWind\DialogData\CharacterSpriteMaps\validate_arashi_dialog_sprites.py"
```
