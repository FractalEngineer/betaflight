#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")/.."
mkdir -p obj/hx711-test
cc -std=c11 -Wall -Wextra -Werror -DUSE_HX711 \
  -Isrc/test/hx711/stubs -Isrc/main \
  src/test/hx711/hx711_test.c src/main/drivers/hx711.c \
  -o obj/hx711-test/test
obj/hx711-test/test
