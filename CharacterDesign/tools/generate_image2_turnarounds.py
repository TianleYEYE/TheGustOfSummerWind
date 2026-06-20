import subprocess
import sys
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
ASSETS = ROOT / "Assets"
PROMPTS = ROOT / "Image2Prompts"
REFERENCE = ASSETS / "lan_arashi_three_view.png"
IMAGE2_SCRIPT = Path.home() / ".codex" / "skills" / "openai-image2-api" / "scripts" / "image2_api.py"


CHARACTERS = [
    (
        "yue",
        "月 / Yue",
        "男主，城市少年到成年承担者。短深棕黑发，清瘦，白色校服衬衫或简洁夏季学生装，深蓝长裤，黑色运动鞋，可带红色书包。气质是嘴硬、保护欲强、青涩但可靠。",
    ),
    (
        "father",
        "老爸 / Father",
        "月的父亲，中年男性。短黑发夹银丝，脸部有岁月感，灰蓝夹克或朴素衬衫，深色长裤，黑鞋。气质沉默、粗粝、疲惫但清醒可靠。",
    ),
    (
        "mother",
        "母亲 / Mother",
        "月已故的母亲，年轻女性。中长深棕发，淡色连衣裙或朴素衬衫裙，旧照片般柔和配色。气质温柔但有主见，山镇出身。",
    ),
    (
        "grandfather",
        "外公 / Grandfather",
        "山镇老人，短银发，微驼背，朴素浅色短袖，宽松灰裤，布鞋。气质慈祥、平静、慢生活。",
    ),
    (
        "grandmother",
        "外婆 / Grandmother",
        "山镇老人，银发发髻，朴素上衣和围裙，深色长裙或宽松裤，布鞋。气质温和、照看孩子、有家庭温度。",
    ),
    (
        "best_friend",
        "基友 / Best Friend",
        "月的同学朋友，少年男性。短乱黑发，表情外放，浅蓝或白色校服衬衫，深色长裤，运动鞋，书包。气质八卦、调侃、校园轻喜剧。",
    ),
    (
        "asir",
        "阿sir / Bookshop Owner",
        "小镇书屋老板，成年男性。短黑发，轻微胡茬，橄榄绿或灰色休闲衬衫，深色长裤，普通鞋。气质沉默、守望者式平静、喜欢港片。",
    ),
    (
        "town_child_leader",
        "镇上孩子领头 / Town Child Leader",
        "童年欺负岚的领头孩子，男孩。短黑发，橙色夏季 T 恤，卷起的蓝色短裤，凉鞋。气质嚣张、起哄、山溪玩水的小镇孩子，不要成人化。",
    ),
    (
        "town_child_a",
        "镇上孩子A / Town Child A",
        "跟风起哄的瘦小男孩。短深色头发，青色夏季 T 恤，绿色卷边短裤，凉鞋。气质动作夸张、从众、被反击时慌张。",
    ),
    (
        "town_child_b",
        "镇上孩子B / Town Child B",
        "更犹豫的跟风男孩。棕色乱发，浅绿色 T 恤，卡其短裤，凉鞋。气质普通、犹豫、跟风，不要成人化。",
    ),
    (
        "lan_father",
        "岚父 / Lan's Father",
        "岚的父亲，中年山镇男性。短深色头发，朴素夹克或短袖，深色裤子，普通鞋。气质长期奔波、现实压力、审视但最终接纳。",
    ),
    (
        "lan_mother",
        "岚母 / Lan's Mother",
        "岚的母亲，中年山镇女性。中长棕发简单束起，灰米色朴素上衣，深色裙或裤，普通鞋。气质温和但有距离感，现实家庭感。",
    ),
    (
        "son",
        "儿子 / Son",
        "月与岚的龙凤胎儿子，小男孩。乱深棕发，明亮好奇眼神，黄色夏季短袖，蓝色短裤，运动鞋。气质活泼、调皮、好奇。",
    ),
    (
        "daughter",
        "女儿 / Daughter",
        "月与岚的龙凤胎女儿，小女孩。栗棕发，小红发饰，浅色夏裙，小凉鞋。气质清亮、温柔但更开朗，呼应岚但年龄明显更小。",
    ),
]


BASE_PROMPT = """Use the input image only as a style, composition, and quality reference: clean cel-shaded visual novel character turnaround, crisp anime lineart, flat clean colors, soft early-2000s Japanese romance VN feeling, plain light background, three full-body views arranged left to right.

Create a new character three-view design sheet for: {name}

Character design:
{description}

Required layout:
- Exactly three full-body figures only: front view, side profile view, back view.
- Same character, same outfit, same proportions across all three views.
- Clean readable silhouette, sharp edges, flat cel shading, tidy color blocks.
- Full body visible from head to feet, no cropping.
- Plain white or very light neutral background.
- No labels, no text, no watermark, no extra props floating separately, no extra headshots, no additional characters.

Important:
- Match the reference image's polished cel-shaded three-view presentation quality.
- Do not copy Lan's identity, white dress, red bow, or hairstyle unless the character description explicitly asks for it.
- Keep the design modest and suitable for a visual novel character reference sheet.
"""


def write_prompts() -> None:
    PROMPTS.mkdir(parents=True, exist_ok=True)
    for slug, name, description in CHARACTERS:
        (PROMPTS / f"{slug}_image2_prompt.txt").write_text(
            BASE_PROMPT.format(name=name, description=description),
            encoding="utf-8",
        )


def run_generation() -> None:
    if not REFERENCE.exists():
        raise FileNotFoundError(f"Missing reference image: {REFERENCE}")
    if not IMAGE2_SCRIPT.exists():
        raise FileNotFoundError(f"Missing Image-2 script: {IMAGE2_SCRIPT}")

    write_prompts()
    ASSETS.mkdir(parents=True, exist_ok=True)
    for slug, _name, _description in CHARACTERS:
        prompt = PROMPTS / f"{slug}_image2_prompt.txt"
        out = ASSETS / f"{slug}_three_view_image2.png"
        cmd = [
            sys.executable,
            str(IMAGE2_SCRIPT),
            "edit",
            "--image",
            str(REFERENCE),
            "--prompt-file",
            str(prompt),
            "--out",
            str(out),
            "--size",
            "1536x1024",
            "--quality",
            "high",
            "--output-format",
            "png",
        ]
        print(f"Generating {out.name}", flush=True)
        subprocess.run(cmd, cwd=str(ROOT), check=True)


if __name__ == "__main__":
    run_generation()
