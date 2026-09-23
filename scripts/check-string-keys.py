#!/usr/bin/env python3
"""Check that the Japanese string catalog covers the English catalog."""

import json
from pathlib import Path
import sys


ROOT = Path(__file__).resolve().parent.parent


def load_catalog(path: Path) -> dict[str, str]:
    def unique_pairs(pairs: list[tuple[str, str]]) -> dict[str, str]:
        result = {}
        for key, value in pairs:
            if key in result:
                raise ValueError(f"{path}: duplicate key {key!r}")
            result[key] = value
        return result

    catalog = json.loads(path.read_text(encoding="utf-8"), object_pairs_hook=unique_pairs)
    if not isinstance(catalog, dict) or any(not isinstance(value, str) for value in catalog.values()):
        raise ValueError(f"{path}: expected an object containing string values")
    return catalog


def main() -> int:
    english = load_catalog(ROOT / "dat" / "strings.json")
    japanese = load_catalog(ROOT / "dat" / "strings.ja.json")
    missing = sorted(english.keys() - japanese.keys())
    extra = sorted(japanese.keys() - english.keys())
    for label, keys in (("Missing Japanese keys", missing), ("Extra Japanese keys", extra)):
        if keys:
            print(f"{label} ({len(keys)}):")
            for key in keys:
                print(f"  {key}")
    if missing or extra:
        return 1
    print(f"String catalogs match: {len(english)} keys")
    return 0


if __name__ == "__main__":
    try:
        sys.exit(main())
    except (OSError, ValueError, json.JSONDecodeError) as error:
        print(error, file=sys.stderr)
        sys.exit(1)
