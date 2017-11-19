#include "MetComp.h"

// Initial condition for wave function
// psi(t=0,x)
complex psi_0(real);
// Normalized Hamiltonian
// J(t, x, psi(t, x)) = (1/ih)H(t, x)psi(t, x)
// d(psi)/dt = J(t, x, psi)
std::vector<complex> J(real, std::vector<complex>);

int main() {

  // Dummy indices
  size_t i, j;
  // Number of spatial cells
  const size_t Nx = 10000;
  // Spatial domain size
  const real L = 10.0;
  // Wave function
  std::vector<complex> psi(Nx);
  // Time and time step
  real t = 0.0, dt = 0.1;
  // Spatial step
  real dx = L / Nx;

  // Apply initial condition and calculate its integral
  real sum = 0;
  for (j = 0; j < Nx; j++) {
    psi[j] = psi_0(j * dx);
    sum += dx * psi[j].abs2();
  }
  std::cout << "Total probability = " << sum << '\n';

  // Boundary conditions
  psi[0] = 0;
  psi[Nx] = 0;

  // RK4
  while (true) {
    rk4(dt, J, t, psi);
  }

  return 0;
}

complex psi_0(real x) {
  real k = 1;
  complex psi;
  if ((x > 5 - k / 2) && (x < 5 + k / 2))
    psi = 1.0 / k;
  else
    psi = 0.0;
  return psi;
}

std::vector<complex> J(real t, std::vector<complex> &psi) {}
