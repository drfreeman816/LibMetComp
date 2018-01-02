#include "array.h"
#include "metcomp.h"
#include "timer.h"
#include <vector>

#define N 1000000
#define M 100000

int main() {

  size_t i, j;

  //**************************************

  {
    Timer arrpartimer;

    real V[N];

#pragma omp for
    for (i = 0; i < M; i++) {

      real temp = real(i / M);

      for (j = 0; j < N; j++) {
        V[j] = temp / (j + 1);
      }
    }

    arrpartimer.stop();

    std::cerr << "PARALLEL ARRAY TIMER = " << arrpartimer.val() << '\n';
  }

  //**************************************

  {
    Timer vectimer;

    std::vector<real> V(N);

    for (i = 0; i < M; i++) {

      real temp = real(i / M);

      for (j = 0; j < N; j++) {
        V[j] = temp / (j + 1);
      }
    }

    vectimer.stop();

    std::cerr << "VECTOR TIMER = " << vectimer.val() << '\n';
  }

  //**************************************

  {
    Timer arrtimer;

    real V[N];

    for (i = 0; i < M; i++) {

      real temp = real(i / M);

      for (j = 0; j < N; j++) {
        V[j] = temp / (j + 1);
      }
    }

    arrtimer.stop();

    std::cerr << "ARRAY TIMER = " << arrtimer.val() << '\n';
  }

  //**************************************

  // std::this_thread::sleep_for(std::chrono::seconds(10));

  return 0;
}
