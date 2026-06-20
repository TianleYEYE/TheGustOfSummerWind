# 《盛夏那阵风》项目内容索引

本文档整理项目根目录中的正式内容、生成素材、对话数据和临时产物边界。后续新增资料时，优先放入对应正式目录，并把可复现的中间产物留在 `tmp/` 或 `output/`。

## Unreal 工程

- `TheGustOfSummerWind.uproject`: Unreal Engine 5.3 项目文件，启用 `ModelViewViewModel`、`EnhancedInput`、`EditorScriptingUtilities` 和 `UnrealClaude`。
- `Source/TheGustOfSummerWind/`: C++ 运行时代码。
  - `Game/`: 游戏实例、模式、HUD、玩家控制器、脚本管理、存档和系统数据。
  - `UI/`: 主菜单、游戏内 UI、设置、退出、按钮和 MVVM ViewModel。
  - `DataStruct/`: 对话行结构和 backlog 相关结构。
- `Content/`: Unreal 资产目录。
  - `Assets/Audio`: 音频素材。
  - `Assets/Background`: 背景图与生成背景。
  - `Assets/UIAssets`: UI 图片素材。
  - `Assets/VerticalPainting`: 角色立绘与分层立绘。
  - `Blueprints/`: UI、按钮、菜单和游戏内蓝图。
  - `DialogData/`: 导入 Unreal 后的数据表资产。
  - `UI/`: UI 相关新资产区。
- `Config/`: Unreal 项目配置。
- `Plugins/UnrealClaude/`: 当前工程启用的 UnrealClaude 插件副本。
- `Binaries/`、`Build/`、`DerivedDataCache/`、`Intermediate/`、`Saved/`: Unreal 本地构建、缓存或运行产物，不作为资料来源。

## 剧情与设定资料

- `AGENTS.md`: 主剧情、角色、地点、时间线、意象和写作风格总纲。创作或改编时先以此为准。
- `CharacterDesign/`: 角色设定和视觉资产索引。
  - `01_yue.md` 到 `10_children.md`: 角色或角色组设定。
  - `Assets/`: 角色三视图与总览图。
  - `Lan_Assets/`: 岚的无脸动作底图资产。
  - `Image2Prompts/`: 角色三视图生成提示词。
  - `tools/generate_image2_turnarounds.py`: 批量生成角色三视图脚本。

## 对话数据

- `DialogData/DialogDataTable_*.csv`: 对话表 CSV 源数据，供 Unreal 数据表导入使用。
- `DialogData/DialogDataTable_*.xlsx`: 对话表表格版本，便于人工编辑和校对。
- `DialogData/CharacterSpriteMaps/`: 岚的分层立绘映射、对话填充脚本和校验报告。
- `DialogData/_backup_*`: 批量处理前的备份目录，保留用于回滚和比对。
- `DialogData/_temp_*`: 从原文档抽取或测试用的临时文本，不作为最新剧情总纲。
- `DialogData/The Guest Of Summer Wind/`: 旧英文命名数据区，使用前需与当前 `DialogDataTable_*` 对齐。

## 生成产物与临时区

- `output/`: AI 图像、PSD、分层素材等生成过程输出。确认有用后应复制到 `Content/` 或 `CharacterDesign/` 的正式位置。
- `tmp/`: 脚本试验、下载包、插件解压、一次性日志和提示词草稿。
- `CharacterDesign/*.log`、`CharacterDesign/tools/__pycache__/`: 本地运行脚本产生的日志和缓存。

## 维护规则

- 正式剧情设定更新后，同步更新 `AGENTS.md`；角色视觉细节更新后，同步更新 `CharacterDesign/README.md`。
- 对话表批量处理前保留 `_backup_*`；处理后运行 `DialogData/CharacterSpriteMaps/validate_arashi_dialog_sprites.py` 校验立绘映射。
- 可复现的中间产物放在 `tmp/` 或 `output/`，不要直接混入正式资产目录。
- 新增 Unreal 资产时，优先使用 `Content/Assets/*`、`Content/UI/*`、`Content/DialogData/*` 的现有分类。
