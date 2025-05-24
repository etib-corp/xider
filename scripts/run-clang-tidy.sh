#!/usr/bin/env bash
set -euo pipefail

LOG_DIR="tidy-output"
LOG_FILE="$LOG_DIR/clang-tidy.log"
SRC_DIR="sources"

mkdir -p "$LOG_DIR"
> "$LOG_FILE"

echo "🔍 Running clang-tidy on sources..."

# Run clang-tidy on each .cpp file and collect output
find "$SRC_DIR" -name '*.cpp' | while read -r file; do
  echo "Linting $file"
  clang-tidy "$file" --quiet --warnings-as-errors=* >> "$LOG_FILE" 2>&1 || true
done

echo "Clang-Tidy run completed."

# Parse and emit GitHub annotations
echo "::group::Clang-Tidy Annotations"
grep -E '^.*:[0-9]+:[0-9]+: (warning|error):' "$LOG_FILE" | while read -r line; do
  file=$(echo "$line" | cut -d':' -f1)
  line_no=$(echo "$line" | cut -d':' -f2)
  col_no=$(echo "$line" | cut -d':' -f3)
  msg_type=$(echo "$line" | cut -d':' -f4 | xargs)
  msg=$(echo "$line" | cut -d':' -f5- | xargs)

  case "$msg_type" in
    warning)
      echo "::warning file=$file,line=$line_no,col=$col_no::$msg"
      ;;
    error)
      echo "::error file=$file,line=$line_no,col=$col_no::$msg"
      ;;
  esac
done
echo "::endgroup::"