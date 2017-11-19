#include "MetComp.h"

complex psi0(real);
real V(real);

int main() {

  size_t n_points = 1001;
  real x1 = -5, x2 = 5;
  real dt = 1e-5;

  Line<real> potential(x1, x2, n_points, V);
  Line<complex> psi(x1, x2, n_points, psi0);

  psi.set_y(0, complex(0, 0));
  psi.set_y(n_points - 1, complex(0, 0));

  QuantumSys particle(potential, psi);

  psi.debug();

  std::cerr << "SUM = " << particle.integral() << '\n';
  particle.normalize();
  std::cerr << "SUM = " << particle.integral() << '\n';

  while (1) {
    particle.out_gnuplot();
    particle.Euler(dt);
    std::cerr << "SUM = " << particle.integral() << '\n';
    std::cin.get();
  }

  return 0;
}

complex psi0(real x) {
  complex y(0, 0);
  y = exp(-std::pow(x + 1, 2));
  return y;
}

real V(real x) { return x * x; }
