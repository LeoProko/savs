#emcc main.c -s WASM=1 -o main.wasm

clang++-11 --target=wasm32 -O3 square.cpp -nostdlib -o out.o -c
wasm-ld-10 --no-entry --export-all --lto-O3 --allow-undefined --import-memory out.o -o out.wasm

