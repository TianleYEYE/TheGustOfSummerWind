from __future__ import annotations

import argparse
import csv
import json
import shutil
from datetime import datetime
from pathlib import Path


LAN_NAME = "\u5c9a"
CURRENT_SPRITE = "/Game/Assets/VerticalPainting/Lan/\u65e0\u8138.\u65e0\u8138"
FACES = {
    "default": "/Game/Assets/VerticalPainting/Lan/\u9ed8\u8ba4png.\u9ed8\u8ba4png",
    "thinking": "/Game/Assets/VerticalPainting/Lan/\u601d\u8003\u8868\u60c5png.\u601d\u8003\u8868\u60c5png",
    "dejected": "/Game/Assets/VerticalPainting/Lan/\u6cae\u4e27\u8868\u60c5png.\u6cae\u4e27\u8868\u60c5png",
}

DEJECTED_KEYWORDS = [
    "\u5206\u624b",
    "\u5bf9\u4e0d\u8d77",
    "\u62b1\u6b49",
    "\u96be\u8fc7",
    "\u54ed",
    "\u75bc",
    "\u75c5",
    "\u538b\u529b",
    "\u59d4\u5c48",
    "\u5bb3\u6015",
    "\u62c5\u5fc3",
    "\u6ca1\u529e\u6cd5",
    "\u4e0d\u884c",
    "\u672b\u5c3e",
    "\u5012\u6570",
    "\u6d41\u8a00",
]
THINKING_KEYWORDS = [
    "\uff1f",
    "?",
    "\u5417",
    "\u662f\u4e0d\u662f",
    "\u4e3a\u4ec0\u4e48",
    "\u600e\u4e48",
    "\u90a3\u4e2a",
    "\u5176\u5b9e",
    "\u8003\u8651",
    "\u4e0d\u77e5\u9053",
    "\u611f\u89c9",
    "\u53ef\u662f",
    "\u4e0d\u8fc7",
    "\u2026\u2026",
    "\u95ee\u9898",
]


def choose_expression(dialog: str) -> str:
    if any(keyword in dialog for keyword in DEJECTED_KEYWORDS):
        return "dejected"
    if any(keyword in dialog for keyword in THINKING_KEYWORDS):
        return "thinking"
    return "default"


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


def write_reports(report: dict, output_dir: Path) -> None:
    output_dir.mkdir(parents=True, exist_ok=True)
    prefix = "Arashi_DialogData_fill_report" if report["mode"] == "apply" else "Arashi_DialogData_fill_dry_run_report"
    json_path = output_dir / f"{prefix}.json"
    json_path.write_text(json.dumps(report, ensure_ascii=False, indent=2), encoding="utf-8")

    md_path = output_dir / f"{prefix}.md"
    lines = [
        "# Arashi DialogData Sprite Fill Report",
        "",
        f"- Mode: `{report['mode']}`",
        f"- Backup directory: `{report.get('backup_dir') or ''}`",
        f"- Updated files: {len(report['updated_files'])}",
        f"- Updated rows: {len(report['updated_rows'])}",
        f"- Skipped files: {len(report['skipped_files'])}",
        "",
        "## Updated Files",
        "",
        "| File | Rows | Default | Thinking | Dejected |",
        "| --- | ---: | ---: | ---: | ---: |",
    ]
    for item in report["updated_files"]:
        counts = item["expression_counts"]
        lines.append(
            f"| {item['file']} | {item['changed']} | {counts['default']} | {counts['thinking']} | {counts['dejected']} |"
        )
    lines.extend(["", "## Skipped Files", "", "| File | Reason | Lan Rows |", "| --- | --- | ---: |"])
    for item in report["skipped_files"]:
        lines.append(f"| {item['file']} | {item['reason']} | {item['lan_rows']} |")
    lines.extend(["", "## Mapping", "", f"- CurrentSprite: `{CURRENT_SPRITE}`"])
    for expression, face in FACES.items():
        lines.append(f"- {expression}: `{face}`")
    md_path.write_text("\n".join(lines) + "\n", encoding="utf-8")


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--dialog-root", default=str(Path(__file__).resolve().parents[1]))
    parser.add_argument("--apply", action="store_true", help="write changes; default is dry-run")
    args = parser.parse_args()

    dialog_root = Path(args.dialog_root)
    output_dir = dialog_root / "CharacterSpriteMaps"
    backup_dir = None
    if args.apply:
        backup_dir = dialog_root / f"_backup_arashi_layered_sprite_fill_{datetime.now().strftime('%Y%m%d_%H%M%S')}"
        backup_dir.mkdir(parents=True, exist_ok=False)

    report = {
        "mode": "apply" if args.apply else "dry-run",
        "backup_dir": str(backup_dir) if backup_dir else None,
        "current_sprite": CURRENT_SPRITE,
        "faces": FACES,
        "updated_files": [],
        "skipped_files": [],
        "updated_rows": [],
    }

    for path in dialog_tables(dialog_root):
        original_bytes = path.read_bytes()
        had_bom = original_bytes.startswith(b"\xef\xbb\xbf")
        with path.open("r", encoding="utf-8-sig", newline="") as handle:
            reader = csv.DictReader(handle)
            fields = reader.fieldnames or []
            rows = list(reader)

        lan_rows = [row for row in rows if (row.get("Name") or "").strip() == LAN_NAME]
        if not lan_rows:
            continue
        if not ("CurrentSprite" in fields and "Face" in fields):
            report["skipped_files"].append(
                {
                    "file": path.name,
                    "reason": "missing CurrentSprite/Face columns",
                    "fields": fields,
                    "lan_rows": len(lan_rows),
                }
            )
            continue

        changed = 0
        expression_counts = {"default": 0, "thinking": 0, "dejected": 0}
        for row in rows:
            if (row.get("Name") or "").strip() != LAN_NAME:
                continue
            if (row.get("CurrentSprite") or "").strip() or (row.get("Face") or "").strip():
                continue
            expression = choose_expression(row.get("Dialog", ""))
            row["CurrentSprite"] = CURRENT_SPRITE
            row["Face"] = FACES[expression]
            changed += 1
            expression_counts[expression] += 1
            report["updated_rows"].append(
                {
                    "file": path.name,
                    "row_id": row.get("", ""),
                    "expression": expression,
                    "dialog": (row.get("Dialog") or "")[:120],
                }
            )

        if changed:
            if args.apply and backup_dir is not None:
                shutil.copy2(path, backup_dir / path.name)
                encoding = "utf-8-sig" if had_bom else "utf-8"
                with path.open("w", encoding=encoding, newline="") as handle:
                    writer = csv.DictWriter(handle, fieldnames=fields, lineterminator="\n")
                    writer.writeheader()
                    writer.writerows(rows)
            report["updated_files"].append({"file": path.name, "changed": changed, "expression_counts": expression_counts})

    write_reports(report, output_dir)
    print(json.dumps({k: report[k] for k in ["mode", "backup_dir"]}, ensure_ascii=False, indent=2))
    print(f"updated_files={len(report['updated_files'])}")
    print(f"updated_rows={len(report['updated_rows'])}")
    print(f"skipped_files={len(report['skipped_files'])}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
