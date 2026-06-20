# Dialog Data Index

本目录保存《盛夏那阵风》的对话表源数据、人工编辑表格、批量处理备份和角色立绘映射。

## 正式对话表

- `DialogDataTable_1.CSV`、`DialogDataTable_2.csv` 到 `DialogDataTable_16.csv`: 当前 CSV 源数据。
- `DialogDataTable_1.xlsx` 到 `DialogDataTable_16.xlsx`: 对应的表格编辑版本。
- `Content/DialogData/DT_Chapter_*.uasset`: CSV 导入 Unreal 后生成的数据表资产。

维护时以 CSV 和 XLSX 的内容一致性为准；导入 Unreal 后再检查 `Content/DialogData/` 中的数据表资产。

## 角色立绘映射

- `CharacterSpriteMaps/Arashi_LayeredSprites.json`: 岚分层立绘映射源。
- `CharacterSpriteMaps/Arashi_LayeredSprites.csv`: 便于人工检查的映射表。
- `CharacterSpriteMaps/fill_arashi_dialog_sprites.py`: 给空白岚对话行填充 `CurrentSprite` 和 `Face`。
- `CharacterSpriteMaps/validate_arashi_dialog_sprites.py`: 校验已填充行。
- `CharacterSpriteMaps/*report*`: 最近一次填充和校验报告。

`Face` 字段应使用表情叠加贴图，不使用完整合成立绘。

## 备份与临时文本

- `_backup_*`: 批量修改前自动或手动保存的备份，保留用于回滚。
- `_temp_*`: 原文档抽取、编码转换或测试行文本，不作为最新资料入口。
- `The Guest Of Summer Wind/`: 旧英文命名数据区，使用前先与当前 `DialogDataTable_*` 对齐。

## 常用命令

```powershell
python "D:\Unreal Projects\TheGustOfSummerWind\DialogData\CharacterSpriteMaps\fill_arashi_dialog_sprites.py"
python "D:\Unreal Projects\TheGustOfSummerWind\DialogData\CharacterSpriteMaps\fill_arashi_dialog_sprites.py" --apply
python "D:\Unreal Projects\TheGustOfSummerWind\DialogData\CharacterSpriteMaps\validate_arashi_dialog_sprites.py"
```
