#!/bin/bash

TEST_ROM_DIR="./roms/blargg/"

RED="\e[31m"
GREEN="\e[32m"
RESET="\e[0m"

run_test_rom() {
    printf "%-30s" "$(basename "$1")"

    local OUTPUT=$(./bin/emulator -q -j "$1")
    echo $OUTPUT | grep 'Failed' &> /dev/null

    if [ $? == 0 ]; then
        printf "${RED}Failed${RESET}\n"
        return 1
    else
        printf "${GREEN}Passed${RESET}\n"
        return 0
    fi
}

main() {
    for rom in ${TEST_ROM_DIR}*; do
        run_test_rom "$rom"
    done
}

main