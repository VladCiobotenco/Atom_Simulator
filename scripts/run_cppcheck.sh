#!/usr/bin/bash
REPO_ROOT="${GITHUB_WORKSPACE:-$(pwd)}"
BUILD_DIR="${BUILD_DIR:-build}"
EXT_DIR="${EXT_DIR:-ext}"
LIB_DIR="libraries"
LIB_ABS="${REPO_ROOT%/}/${LIB_DIR#./}"

EXCLUDE_OPT=""
if cppcheck --help 2>&1 | grep -q -- '--exclude'; then
    EXCLUDE_OPT="--exclude=./${LIB_DIR}"
fi

cppcheck --enable=all \
    --inline-suppr \
    --project="${BUILD_DIR}"/compile_commands.json \
    -i"${BUILD_DIR}" --suppress="*:${BUILD_DIR}/*" \
    -i"${EXT_DIR}" --suppress="*:${EXT_DIR}/*" \
    ${EXCLUDE_OPT} \
    --suppress="*:${LIB_ABS}/*" \
    --suppress=missingIncludeSystem \
    --suppress=unmatchedSuppression \
    --suppress=useStlAlgorithm \
    --check-level=exhaustive \
    --error-exitcode=1
