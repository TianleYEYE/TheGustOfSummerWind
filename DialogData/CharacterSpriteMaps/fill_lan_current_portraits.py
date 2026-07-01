from __future__ import annotations

import argparse
import csv
import json
import shutil
from datetime import datetime
from pathlib import Path


LAN_NAME = "\u5c9a"
LAN_CHARACTER_ID = "Lan"
CURRENT_BODY = "/Game/Assets/VerticalPainting/Lan/arashi_pose01_relaxed_base_noface.arashi_pose01_relaxed_base_noface"

TARGET_HEADERS = [
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
    for pattern in ("DialogDataTable_*.csv", "DialogDataTable_*.CSV"):
        for path in dialog_root.glob(pattern):
            key = str(path.resolve()).lower()
            if key not in seen:
                seen.add(key)
                paths.append(path)
    return sorted(paths, key=lambda item: item.name.lower())


def first_existing(row: dict[str, str], *keys: str) -> str:
    for key in keys:
        value = row.get(key)
        if value:
            return value
    return ""


def character_sprite_cell(character_id: str, current_sprite: str, face: str = "") -> str:
    if not character_id and not current_sprite and not face:
        return ""

    current_sprite_value = f"Texture2D'{current_sprite}'" if current_sprite else "None"
    face_value = f"Texture2D'{face}'" if face else "None"
    return (
        f'(CharacterId="{character_id}",'
        f"CurrentSprite={current_sprite_value},"
        f"Face={face_value},"
        "Position=Inherit,"
        "Transition=Inherit,"
        "bKeepPrevious=True,"
        "bHide=False,"
        "bMirror=False,"
        "ZOrder=0)"
    )


def normalize_row(row: dict[str, str]) -> tuple[dict[str, str], bool]:
    normalized = {header: "" for header in TARGET_HEADERS}
    normalized[""] = first_existing(row, "", "RowName")
    normalized["Name"] = row.get("Name", "")
    normalized["Dialog"] = row.get("Dialog", "")
    normalized["Background"] = row.get("Background", "")
    normalized["BackgroundSound"] = row.get("BackgroundSound", "")
    normalized["ConversationalVoice"] = first_existing(row, "ConversationalVoice", "Conversational voice")

    for key in ("Character_1", "Character_2", "Character_3"):
        normalized[key] = row.get(key, "")

    changed_lan_row = False
    if (normalized["Name"] or "").strip() == LAN_NAME:
        new_cell = character_sprite_cell(LAN_CHARACTER_ID, CURRENT_BODY)
        if normalized["Character_1"] != new_cell:
            normalized["Character_1"] = new_cell
            changed_lan_row = True
        normalized["Character_2"] = ""
        normalized["Character_3"] = ""

    return normalized, changed_lan_row


def process_table(path: Path) -> tuple[bool, bool, list[dict[str, str]], list[dict[str, str]]]:
    original_bytes = path.read_bytes()
    had_bom = original_bytes.startswith(b"\xef\xbb\xbf")
    with path.open("r", encoding="utf-8-sig", newline="") as handle:
        reader = csv.DictReader(handle)
        original_headers = reader.fieldnames or []
        original_rows = list(reader)

    normalized_rows: list[dict[str, str]] = []
    changed_rows: list[dict[str, str]] = []
    for row in original_rows:
        normalized_row, changed_lan_row = normalize_row(row)
        normalized_rows.append(normalized_row)
        if changed_lan_row:
            changed_rows.append(
                {
                    "row_id": normalized_row.get("", ""),
                    "dialog": (normalized_row.get("Dialog") or "")[:120],
                }
            )

    header_changed = original_headers != TARGET_HEADERS
    return had_bom, header_changed, normalized_rows, changed_rows


def write_report(report: dict, output_dir: Path) -> None:
    output_dir.mkdir(parents=True, exist_ok=True)
    prefix = "Lan_CurrentPortrait_fill_report" if report["mode"] == "apply" else "Lan_CurrentPortrait_fill_dry_run_report"
    (output_dir / f"{prefix}.json").write_text(json.dumps(report, ensure_ascii=False, indent=2), encoding="utf-8")

    lines = [
        "# Lan Current Portrait Fill Report",
        "",
        f"- Mode: `{report['mode']}`",
        "- Target schema: `Name/Dialog/Character_1/Character_2/Character_3/Background/BackgroundSound/ConversationalVoice`",
        f"- Body path: `{CURRENT_BODY}`",
        f"- CharacterId: `{LAN_CHARACTER_ID}`",
        f"- Backup directory: `{report.get('backup_dir') or ''}`",
        f"- Updated files: {len(report['updated_files'])}",
        f"- Updated Lan rows: {len(report['updated_rows'])}",
        "",
        "| File | Lan rows | Header changed |",
        "| --- | ---: | --- |",
    ]
    for item in report["updated_files"]:
        lines.append(f"| {item['file']} | {item['changed_lan_rows']} | {item['header_changed']} |")
    (output_dir / f"{prefix}.md").write_text("\n".join(lines) + "\n", encoding="utf-8")


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--dialog-root", default=str(Path(__file__).resolve().parents[1]))
    parser.add_argument("--apply", action="store_true")
    args = parser.parse_args()

    dialog_root = Path(args.dialog_root)
    report_dir = dialog_root / "CharacterSpriteMaps"
    backup_dir = None
    if args.apply:
        backup_dir = dialog_root / f"_backup_lan_current_portrait_fill_{datetime.now().strftime('%Y%m%d_%H%M%S')}"
        backup_dir.mkdir(parents=True, exist_ok=False)

    report = {
        "mode": "apply" if args.apply else "dry-run",
        "backup_dir": str(backup_dir) if backup_dir else None,
        "current_body": CURRENT_BODY,
        "character_id": LAN_CHARACTER_ID,
        "updated_files": [],
        "updated_rows": [],
    }

    for path in dialog_tables(dialog_root):
        had_bom, header_changed, rows, changed_rows = process_table(path)
        if header_changed or changed_rows:
            if args.apply and backup_dir is not None:
                shutil.copy2(path, backup_dir / path.name)
                encoding = "utf-8-sig" if had_bom else "utf-8"
                with path.open("w", encoding=encoding, newline="") as handle:
                    writer = csv.DictWriter(handle, fieldnames=TARGET_HEADERS, lineterminator="\n", extrasaction="ignore")
                    writer.writeheader()
                    writer.writerows(rows)

            report["updated_files"].append(
                {
                    "file": path.name,
                    "changed_lan_rows": len(changed_rows),
                    "header_changed": header_changed,
                }
            )
            for item in changed_rows:
                item["file"] = path.name
                report["updated_rows"].append(item)

    write_report(report, report_dir)
    print(json.dumps({key: report[key] for key in ("mode", "backup_dir", "current_body", "character_id")}, ensure_ascii=False, indent=2))
    print(f"updated_files={len(report['updated_files'])}")
    print(f"updated_lan_rows={len(report['updated_rows'])}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
