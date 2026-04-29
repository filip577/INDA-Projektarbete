"""
generate_map.py

Generates one AI-created map for the game using the local Ollama API.
The script asks the model for only the INNER AREA of a map, validates
the result, places the player start, wraps it with outer walls, and
writes it to:

    assets/maps/generated/generated_map.txt

Current map symbols:
    1 = wall
    0 = floor
    P = player start
"""

from pathlib import Path
from typing import Optional, Tuple

import requests

# ----------------------------
# Configuration
# ----------------------------

OLLAMA_URL = "http://localhost:11434/api/generate"
MODEL = "mistral"

INNER_WIDTH = 20
INNER_HEIGHT = 20

OUTPUT_PATH = Path("assets/maps/generated/generated_map.txt")


def build_prompt(width: int, height: int) -> str:
    """
    Build the text prompt sent to the model.

    The model only generates the INNER AREA using:
    - walls (1)
    - floor (0)

    Python will later:
    - place the player start (P)
    - add the outer walls
    """
    return f"""
Generate a simple inner map for a raycasting FPS game.

Requirements:
- Output exactly {height} lines.
- Each line must contain exactly {width} characters.
- Use only these characters: 1 and 0
- 1 means wall
- 0 means floor
- Include a reasonable amount of walls and floor.
- Do not repeat the same row pattern too often.

Output rules:
- Output only the rows
- No explanation
- No numbering
- No markdown
- No blank lines
""".strip()


def count_char(rows: list[str], target: str) -> int:
    """Count how many times a character appears in the whole map."""
    return sum(row.count(target) for row in rows)


def is_correct_size(rows: list[str]) -> bool:
    """Check that the inner map has the correct size."""
    if len(rows) != INNER_HEIGHT:
        return False
    return all(len(row) == INNER_WIDTH for row in rows)


def has_valid_symbols(rows: list[str]) -> bool:
    """Check that only allowed map symbols are used."""
    allowed = {"1", "0", "P"}
    return all(ch in allowed for row in rows for ch in row)


def find_player(rows: list[str]) -> Optional[Tuple[int, int]]:
    """Return the (x, y) position of the player start, or None if not found."""
    for y, row in enumerate(rows):
        for x, ch in enumerate(row):
            if ch == "P":
                return (x, y)
    return None


def basic_playable(rows: list[str]) -> bool:
    """
    Basic playability check.

    Starting from P, verify that the player can reach at least a few
    walkable tiles. This is intentionally softer than requiring the
    whole map to be one connected region.
    """
    start = find_player(rows)
    if start is None:
        return False

    width = len(rows[0])
    height = len(rows)
    stack = [start]
    visited = set()

    while stack:
        x, y = stack.pop()
        if (x, y) in visited:
            continue
        visited.add((x, y))

        for dx, dy in ((1, 0), (-1, 0), (0, 1), (0, -1)):
            nx, ny = x + dx, y + dy
            if 0 <= nx < width and 0 <= ny < height:
                if rows[ny][nx] in {"0", "P"} and (nx, ny) not in visited:
                    stack.append((nx, ny))

    # Require that the player can move through at least a small area
    return len(visited) >= 4


def add_outer_walls(rows: list[str]) -> list[str]:
    """
    Wrap the inner map with a border of walls.

    If inner rows are width 14, final rows become width 16.
    """
    full_wall = "1" * (INNER_WIDTH + 2)
    wrapped = [full_wall]

    for row in rows:
        wrapped.append("1" + row + "1")

    wrapped.append(full_wall)
    return wrapped


def has_good_tile_balance(rows: list[str]) -> bool:
    """
    Reject maps that are too extreme.

    This is intentionally softer than before so more maps pass.
    """
    total = len(rows) * len(rows[0])
    wall_count = count_char(rows, "1")
    floor_count = count_char(rows, "0") + count_char(rows, "P")

    wall_ratio = wall_count / total
    floor_ratio = floor_count / total

    if wall_ratio < 0.10 or wall_ratio > 0.90:
        return False
    if floor_ratio < 0.10 or floor_ratio > 0.90:
        return False

    return True


def too_many_identical_rows(rows: list[str]) -> bool:
    """
    Reject maps where one exact same row appears too many times.

    Softer than before.
    """
    counts = {}

    for row in rows:
        counts[row] = counts.get(row, 0) + 1

    return max(counts.values()) > 5


def place_player(rows: list[str]) -> list[str]:
    """
    Place the player start on the first available floor tile.
    """
    for y, row in enumerate(rows):
        for x, ch in enumerate(row):
            if ch == "0":
                rows[y] = row[:x] + "P" + row[x + 1:]
                return rows

    raise ValueError("No floor tile found for player placement")


def validate_rows(rows: list[str]) -> tuple[bool, str]:
    """
    Run validation checks on the INNER map before saving.

    Returns:
        (True, "ok") if valid
        (False, reason) if invalid
    """

    if not rows:
        return False, "empty rows"

    if not is_correct_size(rows):
        return False, "wrong size"

    if not has_valid_symbols(rows):
        return False, "invalid symbols"

    if count_char(rows, "P") != 1:
        return False, "wrong number of players"

    if not basic_playable(rows):
        return False, "not playable"

    if not has_good_tile_balance(rows):
        return False, "bad tile balance"

    if too_many_identical_rows(rows):
        return False, "too many identical rows"

    return True, "ok"


def request_map_text() -> str:
    """
    Call the Ollama API and ask for one inner map.

    We request non-streaming plain text output.
    """
    payload = {
        "model": MODEL,
        "prompt": build_prompt(INNER_WIDTH, INNER_HEIGHT),
        "stream": False,
        "options": {
            "temperature": 0.3
        }
    }

    response = requests.post(OLLAMA_URL, json=payload, timeout=120)
    response.raise_for_status()

    data = response.json()
    return data["response"].strip()


def parse_rows(raw_text: str) -> list[str]:
    """
    Split model output into map rows and repair small formatting issues.

    - Removes empty lines
    - Keeps only valid symbols
    - Crops rows that are too long
    - Pads rows that are too short with floor tiles
    """
    rows = [line.strip() for line in raw_text.splitlines() if line.strip()]
    fixed = []

    for row in rows[:INNER_HEIGHT]:
        cleaned = "".join(ch for ch in row if ch in {"0", "1"})

        if len(cleaned) > INNER_WIDTH:
            cleaned = cleaned[:INNER_WIDTH]
        elif len(cleaned) < INNER_WIDTH:
            cleaned = cleaned.ljust(INNER_WIDTH, "0")

        fixed.append(cleaned)

    return fixed


def save_map(rows: list[str], output_path: Path) -> None:
    """Write the validated final map to disk."""
    output_path.parent.mkdir(parents=True, exist_ok=True)
    output_path.write_text("\n".join(rows) + "\n", encoding="utf-8")


def main() -> None:
    """
    Try a few times until we get one valid inner map.
    Then place the player, add outer walls, and save the final map.
    """
    max_attempts = 10

    for attempt in range(1, max_attempts + 1):
        try:
            raw_text = request_map_text()
            print(raw_text)

            rows = parse_rows(raw_text)
            rows = place_player(rows)

            print("ROW COUNTS:")
            counts = {}
            for row in rows:
                counts[row] = counts.get(row, 0) + 1
            print(counts)

            print("ROWS COUNT:", len(rows))
            for row in rows:
                print(len(row), row)

            valid, reason = validate_rows(rows)

            if not valid:
                print(f"[SKIP] Attempt {attempt}: {reason}")
                continue

            final_rows = add_outer_walls(rows)
            save_map(final_rows, OUTPUT_PATH)
            print(f"[OK] Saved generated map to {OUTPUT_PATH}")
            return

        except Exception as e:
            print(f"[ERROR] Attempt {attempt}: {e}")

    raise SystemExit("Failed to generate a valid map")


if __name__ == "__main__":
    main()