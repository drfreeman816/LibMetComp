#include "../inc/MetComp.h"

/* Support functions */

namespace supp {
// Printf
void printf(const char *format) { std::cout << format; }

// Printfln
void printfln(const char *format) { std::cout << format << std::endl; }
} // namespace supp

/* Math */

// namespace math

/* Complex number */

// Constructors

// Default constructor
complex::complex(void) : imag(false), re(0), im(0) {}

// Both real and imaginary parts given
complex::complex(real re0, real im0) : imag(true), re(re0), im(im0) {
  if (im0 == 0)
    imag = false;
}

// Only phase given
complex::complex(real phi0)
    : imag(true), re(std::cos(phi0)), im(std::sin(phi0)) {
  if (im == 0)
    imag = false;
}

// Real modulus and complex phase given
complex::complex(real r0, complex phi0) : imag(true) {
  *this = exp(phi0) * r0;
  if (im == 0)
    imag = false;
}

// Complex "modulus" and real phase given (complex + rotation)
complex::complex(complex r0, real phi0) : imag(true) {
  *this = r0 * exp(Iu * phi0);
  if (im == 0)
    imag = false;
}

// Destructor
complex::~complex() {}

// Swap function
void complex::swap(complex &other) {
  std::swap(imag, other.imag);
  std::swap(re, other.re);
  std::swap(im, other.im);
}

// Operators overloading

// Assignment from a real number
complex &complex::operator=(const real &other) {
  complex temp(other, 0);
  this->swap(temp); // this <=> temp
  return *this;
}

// Assignment
complex &complex::operator=(const complex &other) {
  complex temp(other); // temp = other
  this->swap(temp);    // this <=> temp
  return *this;
}

// Unary minus
complex complex::operator-(void) const {
  complex result(-re, -im);
  return result;
}

// Addition with a real number
complex complex::operator+(const real &other) const {
  complex result(re + other, im);
  return result;
}

// Addition
complex complex::operator+(const complex &other) const {
  complex result(re + other.re, im + other.im);
  return result;
}

// Subtraction by a real number
complex complex::operator-(const real &other) const {
  complex result(re - other, im);
  return result;
}

// Subtraction
complex complex::operator-(const complex &other) const {
  complex result(re - other.re, im - other.im);
  return result;
}

// Multiplication by a real number
complex complex::operator*(const real &other) const {
  complex result(re * other, im * other);
  return result;
}

// Multiplication
complex complex::operator*(const complex &other) const {
  complex result(re * other.re - im * other.im, re * other.im + im * other.re);
  return result;
}

// Division by a real number
complex complex::operator/(const real &other) const {
  complex result(re / other, im / other);
  return result;
}

// Division
complex complex::operator/(const complex &other) const {
  complex result = *this * other.conj();
  result = result / math::padder(2, other.re, other.im);
  return result;
}

// Conjugate
complex complex::conj(void) const {
  complex result(re, -im);
  return result;
}

// Absolute value
real complex::abs(void) const {
  if (!imag)
    return std::abs(re);
  else
    return math::pnorm(2, re, im);
}

// Absolute value squared
real complex::abs2(void) const {
  if (!imag)
    return re * re;
  else
    return math::padder(2, re, im);
}

// Phase
real complex::phi(void) const {
  if (!imag) {
    if (re > 0)
      return 0;
    else
      return math::pi;
  } else
    return std::atan(re / im);
}

// To string
complex::operator std::string() const {
  if (!imag)
    return std::to_string(re);
  else if (im > 0)
    return (std::to_string(re) + " + " + std::to_string(im) + "i");
  else
    return (std::to_string(re) + " - " + std::to_string(std::abs(im)) + "i");
}

// Complex number

/* Overload real operators */

/* Complex functions */

// Square root of a real number
// complex sqrt(real val) {
//   if (val < 0)
//     return Iu * std::sqrt(-val);
//   else
//     return std::sqrt(val);
// }

// Complex square root
complex sqrt(complex val) {
  complex result(val.phi() / 2);
  return result * std::sqrt(val.abs());
}

// Complex exponential
complex exp(complex val) {
  complex result(std::exp(-val.im) * std::cos(val.re),
                 std::exp(-val.im) * std::sin(val.re));
  return result;
}

// Complex logarithm
complex log(complex val) {
  complex result(std::log(val.abs()), val.phi());
  return result;
}

// Complex sine
complex sin(complex val) {
  complex result;
  result = (exp(Iu * val) - exp(-Iu * val)) / 2;
  return result;
}

// Complex cosine
complex cos(complex val) {
  complex result;
  result = (exp(Iu * val) + exp(-Iu * val)) / 2;
  return result;
}

// Complex tangent
complex tan(complex val) {
  complex result;
  result = sin(val) / cos(val);
  return result;
}

// Complex functions

/* Measurement */

// Constructors
Measurement::Measurement(real v0, real r0)
    : unc(true), v(v0), u(std::abs(r0 * v0)), r(std::abs(r0)) {
  try {
    if (v0 == 0) {
      unc = false;
      u = 0;
      r = 0;
      throw "Uncertainty undefined for value zero.";
    } else if (r0 == 0) {
      unc = false;
      u = 0;
    }
  } catch (const char *msg) {
    std::cerr << msg << std::endl;
  }
}

Measurement::Measurement(real v0) : Measurement(v0, 0) { unc = false; }

// Destructor
Measurement::~Measurement() {}

// Swap function
void Measurement::swap(Measurement &other) {
  std::swap(unc, other.unc);
  std::swap(v, other.v);
  std::swap(r, other.r);
  std::swap(u, other.u);
}

// Operators overloading

// Assignment
Measurement &Measurement::operator=(const Measurement &other) {
  Measurement temp(other);
  this->swap(temp);
  return *this;
}

// z = x + y
Measurement Measurement::operator+(const Measurement &y) {
  Measurement z(v + y.v);
  if (unc || y.unc) {
    z.unc = true;
    z.u = math::pnorm(2, u, y.u);
    z.r = z.u / z.v;
  }
  return z;
}
// z = x - y
Measurement Measurement::operator-(const Measurement &y) {
  Measurement z(v - y.v);
  if (unc || y.unc) {
    z.unc = true;
    z.u = math::pnorm(2, u, y.u);
    z.r = z.u / z.v;
  }
  return z;
}
// z = x * y
Measurement Measurement::operator*(const Measurement &y) {
  Measurement z(v * y.v);
  if (unc || y.unc) {
    z.unc = true;
    z.u = math::pnorm(2, v * u, y.v * y.u);
    z.r = z.u / z.v;
  }
  return z;
}
// z = x / y
Measurement Measurement::operator/(const Measurement &y) {
  try {
    if (y.v == 0)
      throw "Cannot divide by zero! What the fu-";
    Measurement z(v / y.v);
    if (unc || y.unc) {
      z.unc = true;
      z.u = math::pnorm(2, u / y.v, v * y.u / (y.v * y.v));
      z.r = z.u / z.v;
    }
    return z;
  } catch (const char *msg) {
    std::cerr << msg << '\n';
    return *this;
  }
}
// x < y
bool Measurement::operator<(const Measurement &y) {
  try {
    real d = u + y.u / (std::abs(v - y.v));
    if (d < 1)
      return (v < y.v);
    else
      throw d;
  } catch (real d) {
    std::cerr << "Uncertainty overlap: d = " << d << '\n';
    return (v < y.v);
  }
}
// x > y
bool Measurement::operator>(const Measurement &y) {
  try {
    real d = u + y.u / (std::abs(v - y.v));
    if (d < 1)
      return (v > y.v);
    else
      throw d;
  } catch (real d) {
    std::cerr << "Uncertainty overlap: d = " << d << '\n';
    return (v > y.v);
  }
}

// To string
Measurement::operator std::string() const {
  if (unc)
    return (std::to_string(v) + " +/- " + std::to_string(u) +
            " (r=" + std::to_string(r) + ")");
  else
    return std::to_string(v);
}

// Measurement

// Differential Equations
