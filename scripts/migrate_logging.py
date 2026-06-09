#!/usr/bin/env python3
"""
Bulk migration script for evan and guillaume logging call sites.
Converts from string-argument style to stream style.

Transforms:
- getLogger().debug("msg")            -> getLogger().debug() << "msg"
- getLogger().info("a" + std::to_string(b) + "c") -> getLogger().info() << "a" << b << "c"
- Loggable<X, StandardLogger>         -> Loggable<X, DefaultLogger>
- #include <utility/logging/standard_logger.hpp> -> #include <utility/logging/default_logger.hpp>
"""

import re
import sys
from pathlib import Path

EVAN_DIR = Path("/Users/clement-antoine-xavier/etib-corp/xider/libraries/evan")
GUILLAUME_DIR = Path("/Users/clement-antoine-xavier/etib-corp/xider/libraries/guillaume")


def strip_to_string(expr: str) -> str:
    """Remove wrapping std::to_string(...) calls."""
    expr = expr.strip()
    m = re.fullmatch(r"std::to_string\s*\(\s*(.+?)\s*\)", expr)
    if m:
        return m.group(1)
    return expr


def convert_log_call(text: str) -> str:
    """
    Convert all getLogger().level(...) calls to stream style.
    Handles string literals, concatenations, and multi-line arguments.
    """
    # Pattern: getLogger().method(  ...  )
    # We use a simple brace-matching approach after finding the prefix.
    pattern = re.compile(r'(getLogger\(\)\.(debug|info|warning|error|log)\()')
    result = []
    last_end = 0

    for match in pattern.finditer(text):
        start = match.start()
        prefix = match.group(1)
        method = match.group(2)
        open_paren = match.end() - 1  # index of '('
        # find matching ')'
        depth = 1
        i = open_paren + 1
        while i < len(text) and depth > 0:
            if text[i] == '(':
                depth += 1
            elif text[i] == ')':
                depth -= 1
            i += 1
        close_paren = i - 1
        arg_text = text[open_paren + 1:close_paren]

        # Convert argument text to stream parts
        parts = split_concatenations(arg_text)
        if not parts:
            stream_expr = f"getLogger().{method}()"
        else:
            stream_expr = f"getLogger().{method}()"
            for part in parts:
                stream_expr += f" << {part}"

        result.append(text[last_end:start])
        result.append(stream_expr)
        last_end = close_paren + 1

    result.append(text[last_end:])
    return "".join(result)


def split_concatenations(arg_text: str) -> list:
    """
    Split a C++ string-concatenation expression into parts for << streaming.
    Handles:
      "literal"
      "a" + "b"
      "a" + std::to_string(x) + "b"
      variable + "text"
    We do a best-effort split on top-level '+' operators.
    """
    arg_text = arg_text.strip()
    if not arg_text:
        return []

    # Simple heuristic: split on top-level '+'
    parts = []
    depth = 0
    current = []
    for ch in arg_text:
        if ch == '(':
            depth += 1
            current.append(ch)
        elif ch == ')':
            depth -= 1
            current.append(ch)
        elif ch == '+' and depth == 0:
            part = "".join(current).strip()
            if part:
                parts.append(strip_to_string(part))
            current = []
        else:
            current.append(ch)
    part = "".join(current).strip()
    if part:
        parts.append(strip_to_string(part))
    return parts


def replace_includes_and_aliases(text: str) -> str:
    text = text.replace(
        '#include <utility/logging/standard_logger.hpp>',
        '#include <utility/logging/default_logger.hpp>'
    )
    text = text.replace(
        '#include "utility/logging/standard_logger.hpp"',
        '#include "utility/logging/default_logger.hpp"'
    )
    # Replace explicit Loggable template arguments
    text = re.sub(
        r'utility::logging::Loggable<\s*([^,]+),\s*utility::logging::StandardLogger\s*>',
        r'utility::logging::Loggable<\1, utility::logging::DefaultLogger>',
        text
    )
    text = re.sub(
        r'Loggable<\s*([^,]+),\s*StandardLogger\s*>',
        r'Loggable<\1, DefaultLogger>',
        text
    )
    # Replace any other bare StandardLogger mentions that aren't inside comments
    # Be careful not to break legitimate references.
    text = re.sub(r'\bStandardLogger\b', 'DefaultLogger', text)
    return text


def process_file(path: Path) -> None:
    original = path.read_text()
    text = original
    text = replace_includes_and_aliases(text)
    text = convert_log_call(text)
    if text != original:
        path.write_text(text)
        print(f"Updated: {path}")
    else:
        print(f"No changes: {path}")


def main():
    for directory in [EVAN_DIR, GUILLAUME_DIR]:
        for ext in ["*.cpp", "*.hpp", "*.tpp"]:
            for f in directory.rglob(ext):
                process_file(f)


if __name__ == "__main__":
    main()
