#!/usr/bin/bash

cppcheck --enable=all \
    --inline-suppr \
    --project="${BUILD_DIR:-build}"/compile_commands.json \
    -i"${BUILD_DIR:-build}" --suppress="*:${BUILD_DIR:-build}/*" \
    -i"${EXT_DIR:-json.hpp}" --suppress="*:${EXT_DIR:-json.hpp}/*" \
    --suppress=missingIncludeSystem \
    --suppress=unmatchedSuppression \
    --suppress=useStlAlgorithm \
    --check-level=exhaustive \
    --error-exitcode=1
