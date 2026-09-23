#!/usr/bin/env python3
"""Create the bundled medium-weight Japanese font from Noto Sans JP VF.

Requires fontTools. The source font is distributed under SIL OFL 1.1.
"""

from __future__ import annotations

import argparse
from pathlib import Path

from fontTools.ttLib import TTFont
from fontTools.varLib.instancer import instantiateVariableFont


REPOSITORY = Path(__file__).resolve().parents[1]
DEFAULT_OUTPUT = REPOSITORY / "dat" / "fonts" / "sorcery-jp-sans-medium.ttf"
WEIGHT = 500
NAMES = {
    1: "Sorcery JP Sans",
    2: "Medium",
    4: "Sorcery JP Sans Medium",
    6: "SorceryJPSans-Medium",
    16: "Sorcery JP Sans",
    17: "Medium",
}


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("source", type=Path, help="NotoSansJP-VF.ttf from the OFL distribution")
    parser.add_argument("--output", type=Path, default=DEFAULT_OUTPUT)
    args = parser.parse_args()

    source = TTFont(args.source)
    if "fvar" not in source or not any(
        axis.axisTag == "wght" and axis.minValue <= WEIGHT <= axis.maxValue
        for axis in source["fvar"].axes
    ):
        raise ValueError("Source must be a Noto Sans JP variable font with weight 500")

    family_names = {
        record.toUnicode()
        for record in source["name"].names
        if record.nameID == 1
    }
    license_texts = {
        record.toUnicode()
        for record in source["name"].names
        if record.nameID == 13
    }
    if "Noto Sans JP" not in family_names or not any(
        "SIL Open Font License" in text for text in license_texts
    ):
        raise ValueError("Source must be the OFL-licensed Noto Sans JP font")

    source_revision = next(
        record.toUnicode().split(";", 1)[0]
        for record in source["name"].names
        if record.nameID == 3
    )
    font = instantiateVariableFont(source, {"wght": WEIGHT}, inplace=False)
    font.recalcTimestamp = False
    names = font["name"]
    names_to_set = NAMES | {
        3: f"SorceryJPSans-Medium; NotoSansJP-VF {source_revision}; wght={WEIGHT}"
    }
    for name_id, value in names_to_set.items():
        names.removeNames(nameID=name_id)
        names.setName(value, name_id, 3, 1, 0x409)
        names.setName(value, name_id, 1, 0, 0)

    args.output.parent.mkdir(parents=True, exist_ok=True)
    font.save(args.output)
    print(args.output)


if __name__ == "__main__":
    main()
