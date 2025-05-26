#!/bin/sh

set -xe

gcc -Wall -Wextra -ggdb -o test test.c -lm
$WASI_SDK_PATH/bin/clang -Wall -Wextra --target=wasm32-wasi --sysroot=$WASI_SDK_PATH/share/wasi-sysroot -o wasm.o -c wasm.c
$WASI_SDK_PATH/bin/wasm-ld -m wasm32 --no-entry --export-all --allow-undefined -o wasm.wasm wasm.o