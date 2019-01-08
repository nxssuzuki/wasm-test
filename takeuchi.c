#include <stdio.h>
#include<time.h>

int tarai(int, int, int);

int main(int argc, char ** argv) {
  clock_t start, end;
  int result;
  start = clock();
  result = tarai(14, 7, 0);
  end = clock();
  printf("tarai(14, 7, 0) in WASM: %f s\n", (double)(end - start) / CLOCKS_PER_SEC);
  printf("tarai(14, 7, 0) = %d\n", result);
  return 0;
}

int tarai(int x, int y, int z) {
  if(x <= y){
    return y;
  }
  return tarai(tarai(x-1, y, z), tarai(y-1, z, x), tarai(z-1, x, y));
}
