from __future__ import annotations

import argparse
import csv
from pathlib import Path


LAN_NAME = "\u5c9a"
LAN_CHARACTER_ID = "Lan"
CURRENT_SPRITE = "/Game/Assets/VerticalPainting/Lan/arashi_pose01_relaxed_base_noface.arashi_pose01_relaxed_base_noface"
REQUIRED_HEADERS = [
    "",
    "Name",
    "Dialog",
    "Character_1",
    "Character_2",
    "Character_3",
    "Background",
    "BackgroundSound",
    "ConversationalVoice",
]


def dialog_tables(dialog_root: Path) -> list[Path]:
    paths: list[Path] = []
    seen: set[str] = set()
    for pattern in ["DialogDataTable_*.csv", "DialogDataTable_*.CSV"]:
        for path in dialog_root.glob(pattern):
            key = str(path.resolve()).lower()
            if key not in seen:
                seen.add(key)
                paths.append(path)
    return sorted(paths, key=lambda item: item.name.lower())


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--dialog-root", default=str(Path(__file__).resolve().parents[1]))
    args = parser.parse_args()

    dialog_root = Path(args.dialog_root)
    errors: list[str] = []
    validated_lan_rows = 0
    per_file: list[tuple[str, int, str]] = []

    for path in dialog_tables(dialog_root):
        with path.open("r", encoding="utf-8-sig", newline="") as handle:
            reader = csv.DictReader(handle)
            fields = reader.fieldnames or []
            rows = list(reader)

        if fields != REQUIRED_HEADERS:
            errors.append(f"{path.name}: bad headers: {fields}")

        lan_rows = [row for row in rows if (row.get("Name") or "").strip() == LAN_NAME]
        if not lan_rows:
            continue

        file_errors = 0
        for row in lan_rows:
            validated_lan_rows += 1
            row_id = row.get("", "")
            character_1 = (row.get("Character_1") or "").strip()
            if f'CharacterId="{LAN_CHARACTER_ID}"' not in character_1:
                file_errors += 1
                errors.append(f"{path.name}:{row_id} bad Character_1 CharacterId")
            if f"Texture2D'{CURRENT_SPRITE}'" not in character_1:
                file_errors += 1
                errors.append(f"{path.name}:{row_id} bad Character_1 CurrentSprite")
            if "Face=None" not in character_1:
                file_errors += 1
                errors.append(f"{path.name}:{row_id} Character_1 Face should be None until expression assets are ready")
            if (row.get("Character_2") or "").strip() or (row.get("Character_3") or "").strip():
                file_errors += 1
                errors.append(f"{path.name}:{row_id} Character_2/Character_3 should be blank for single-character Lan rows")

        per_file.append((path.name, len(lan_rows), "ok" if file_errors == 0 else f"errors={file_errors}"))

    for file_name, count, status in per_file:
        print(f"{file_name}|lan_rows={count}|{status}")
    print(f"validated_lan_rows={validated_lan_rows}")
    if errors:
        print("ERRORS")
        for error in errors:
            print(error)
        return 1
    print("dialog sprite CSV validation OK")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
