# Character Design Index

本目录集中存放《盛夏那阵风》的角色设计资料。每个角色或角色组对应一个 Markdown 文件，包含角色设定、视觉关键词、表情动作、三视图资料或三视图制作规范。

## 文件清单

- `01_yue.md`: 月
- `02_lan_arashi.md`: 岚 / Lan / Arashi
- `03_father.md`: 老爸
- `04_mother.md`: 母亲
- `05_grandparents.md`: 外公、外婆
- `06_best_friend.md`: 基友
- `07_asir.md`: 阿sir
- `08_town_children.md`: 镇上孩子 / 欺负岚的三人
- `09_lan_parents.md`: 岚的父母
- `10_children.md`: 月与岚的孩子

## 资产

- `Assets/lan_arashi_three_view.png`: 岚的三视图落地版。
- `Assets/*_three_view_image2.png`: 其他角色或角色组成员的三视图落地版。
- `Assets/three_view_contact_sheet.png`: 全部三视图的总览检查图。
- `Lan_Assets/arashi_pose*_base_noface.png`: 岚的无脸动作底图，用于后续表情分层和立绘组合。
- `Image2Prompts/*.txt`: 其他角色的 Image-2 生成提示词。
- `tools/generate_image2_turnarounds.py`: 使用岚三视图作为参考图，批量调用 Image-2 生成其他角色三视图。
- `sd_webui_api*.log`、`tools/__pycache__/`: 本地生成日志和 Python 缓存，不作为正式角色资料。

## 三视图状态

| 角色 | 状态 |
| --- | --- |
| 岚 | 已有三视图图片 |
| 月 | 已生成，目标 `Assets/yue_three_view_image2.png` |
| 老爸 | 已生成，目标 `Assets/father_three_view_image2.png` |
| 母亲 | 已生成，目标 `Assets/mother_three_view_image2.png` |
| 外公、外婆 | 已生成，目标 `Assets/grandfather_three_view_image2.png`、`Assets/grandmother_three_view_image2.png` |
| 基友 | 已生成，目标 `Assets/best_friend_three_view_image2.png` |
| 阿sir | 已生成，目标 `Assets/asir_three_view_image2.png` |
| 镇上孩子 / 欺负岚的三人 | 已生成，目标 `Assets/town_child_*_three_view_image2.png` |
| 岚的父母 | 已生成，目标 `Assets/lan_father_three_view_image2.png`、`Assets/lan_mother_three_view_image2.png` |
| 月与岚的孩子 | 已生成，目标 `Assets/son_three_view_image2.png`、`Assets/daughter_three_view_image2.png` |

## Image-2 生成说明

如需重新生成或批量更新三视图，配置有效 `OPENAI_API_KEY` 或可用网关后，在项目根目录运行：

```powershell
python "D:\Unreal Projects\TheGustOfSummerWind\CharacterDesign\tools\generate_image2_turnarounds.py"
```

脚本会读取 `Assets/lan_arashi_three_view.png` 作为风格、排版和质量参考，并为其他角色生成干净赛璐璐风格三视图。

## 维护规则

- 角色设定文本更新时，同步检查对应 `Image2Prompts/*.txt` 是否需要调整。
- 新增角色三视图后，更新 `Assets/three_view_contact_sheet.png` 和上方状态表。
- 生成过程中的草稿、失败图和日志放入项目根目录 `tmp/` 或 `output/`，确认可用后再移动到本目录。
