#include <cstdlib>
#include <iostream>

#include "timer.h"

#define N 10000

int main() {
  int (*a)[N] = new int[N][N];

  srand(1827);
  for (int i = 0; i < N; ++i) {
	for (int j = 0; j < N; ++j) {
	  a[i][j] = rand() % 2;
	}
  }

  Timer t;
  int sum = 0;
  for (int i = 0; i < N; ++i) {
	for (int j = 0; j < N; ++j) {
	  sum += a[i][j];
	}
  }
  return 0;
}

