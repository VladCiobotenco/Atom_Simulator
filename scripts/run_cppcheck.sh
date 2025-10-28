#!/usr/bin/bash

cppcheck --enable=all \
    --inline-suppr \
    --project="${BUILD_DIR:-build}"/compile_commands.json \
    --suppress="*:*${BUILD_DIR:-build}/*" \
    --suppress="*:*${EXT_DIR:-libraries}/*" \
    --suppress=missingIncludeSystem \
    --suppress=unmatchedSuppression \
    --suppress=useStlAlgorithm \
    --check-level=exhaustive \
    --error-exitcode=1
