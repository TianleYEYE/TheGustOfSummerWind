from __future__ import annotations

import argparse
import csv
import shutil
from datetime import datetime
from pathlib import Path

from openpyxl import Workbook


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


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--dialog-root", default=str(Path(__file__).resolve().parents[1]))
    parser.add_argument("--backup", action="store_true")
    args = parser.parse_args()

    dialog_root = Path(args.dialog_root)
    backup_dir = None
    if args.backup:
        backup_dir = dialog_root / f"_backup_xlsx_sync_{datetime.now().strftime('%Y%m%d_%H%M%S')}"
        backup_dir.mkdir(parents=True, exist_ok=False)

    updated: list[str] = []
    for csv_path in dialog_tables(dialog_root):
        xlsx_path = csv_path.with_suffix(".xlsx")
        if backup_dir and xlsx_path.exists():
            shutil.copy2(xlsx_path, backup_dir / xlsx_path.name)

        with csv_path.open("r", encoding="utf-8-sig", newline="") as handle:
            rows = list(csv.reader(handle))

        workbook = Workbook()
        sheet = workbook.active
        sheet.title = csv_path.stem[:31]
        for row in rows:
            sheet.append(row)

        workbook.save(xlsx_path)
        updated.append(xlsx_path.name)

    print(f"updated_xlsx={len(updated)}")
    if backup_dir:
        print(f"backup_dir={backup_dir}")
    for name in updated:
        print(name)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
