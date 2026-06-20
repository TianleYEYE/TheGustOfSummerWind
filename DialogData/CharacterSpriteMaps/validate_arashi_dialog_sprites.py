from __future__ import annotations

import argparse
import csv
from pathlib import Path


LAN_NAME = "\u5c9a"
CURRENT_SPRITE = "/Game/Assets/VerticalPainting/Lan/\u65e0\u8138.\u65e0\u8138"
FACES = {
    "/Game/Assets/VerticalPainting/Lan/\u9ed8\u8ba4png.\u9ed8\u8ba4png",
    "/Game/Assets/VerticalPainting/Lan/\u601d\u8003\u8868\u60c5png.\u601d\u8003\u8868\u60c5png",
    "/Game/Assets/VerticalPainting/Lan/\u6cae\u4e27\u8868\u60c5png.\u6cae\u4e27\u8868\u60c5png",
}


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
    skipped_schema_lan_rows = 0
    per_file: list[tuple[str, int, str]] = []

    for path in dialog_tables(dialog_root):
        with path.open("r", encoding="utf-8-sig", newline="") as handle:
            reader = csv.DictReader(handle)
            fields = reader.fieldnames or []
            rows = list(reader)
        lan_rows = [row for row in rows if (row.get("Name") or "").strip() == LAN_NAME]
        if not lan_rows:
            continue
        if not ("CurrentSprite" in fields and "Face" in fields):
            skipped_schema_lan_rows += len(lan_rows)
            per_file.append((path.name, len(lan_rows), "skipped_schema"))
            continue

        file_errors = 0
        for row in lan_rows:
            validated_lan_rows += 1
            row_id = row.get("", "")
            if (row.get("CurrentSprite") or "").strip() != CURRENT_SPRITE:
                file_errors += 1
                errors.append(f"{path.name}:{row_id} bad CurrentSprite")
            if (row.get("Face") or "").strip() not in FACES:
                file_errors += 1
                errors.append(f"{path.name}:{row_id} bad Face")
        per_file.append((path.name, len(lan_rows), "ok" if file_errors == 0 else f"errors={file_errors}"))

    for file_name, count, status in per_file:
        print(f"{file_name}|lan_rows={count}|{status}")
    print(f"validated_lan_rows={validated_lan_rows}")
    print(f"skipped_schema_lan_rows={skipped_schema_lan_rows}")
    if errors:
        print("ERRORS")
        for error in errors:
            print(error)
        return 1
    print("dialog sprite CSV validation OK")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
