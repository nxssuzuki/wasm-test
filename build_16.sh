emcc -o takeuchi_16.html takeuchi_16.c -O3 -s WASM=1 --shell-file html_template/shell_minimal.html -s NO_EXIT_RUNTIME=1
emrun --no_browser --port 8080 .
