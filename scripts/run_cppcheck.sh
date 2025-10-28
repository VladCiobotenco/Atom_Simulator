#!/usr/bin/bash
set -euo pipefail

REPO_ROOT="${GITHUB_WORKSPACE:-$(pwd)}"
BUILD_DIR="${BUILD_DIR:-build}"
LIB_DIR="libraries"
LIB_ABS="${REPO_ROOT%/}/${LIB_DIR#./}"

# Create a suppressions-file that matches the absolute path cppcheck usually reports in CI.
SUPPRESS_FILE="${REPO_ROOT%/}/.cppcheck_suppressions.txt"
mkdir -p "$(dirname "$SUPPRESS_FILE")"
printf '%s\n' "*:${LIB_ABS}/*" > "$SUPPRESS_FILE"

# Debug output to verify paths in CI logs
echo "cppcheck binary: $(command -v cppcheck || echo 'not found')"
cppcheck --version || true
echo "Repo root: $REPO_ROOT"
echo "Library absolute path (suppressed): $LIB_ABS"
echo "Suppressions file contents:"
cat "$SUPPRESS_FILE"

# Run cppcheck; keep build dir in -i so compile_commands.json resolves, do NOT add libraries to -i
cppcheck --enable=all \
    --inline-suppr \
    --project="${BUILD_DIR}/compile_commands.json" \
    -i"${BUILD_DIR}" --suppress="*:${BUILD_DIR}/*" \
    --suppressions-list="$SUPPRESS_FILE" \
    --suppress=missingIncludeSystem \
    --suppress=unmatchedSuppression \
    --suppress=useStlAlgorithm \
    --check-level=exhaustive \
    --error-exitcode=1
