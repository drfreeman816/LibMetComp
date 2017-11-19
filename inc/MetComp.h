#pragma once

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <iterator>
#include <map>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

typedef double real;

/* Support functions

  print:
  Print series of arguments

  println:
  Print series of arguments with a new line at the end

  printf:
  Print formated text

  printfln:
  Print formated text with a new line at the end

  */
namespace supp {

// Print
template <typename... Ts> void printstr(Ts &&... strs) {
  (std::cout << ... << strs);
}

// Println
template <typename... Ts> void printstrln(Ts &&... strs) {
  (std::cout << ... << strs) << '\n';
}

// Printf
void printf(const char *);

template <typename T, typename... Ts>
void printf(const char *format, T value, Ts... values) {
  for (; *format != '\0'; format++) {
    if (*format == '$') {
      std::cout << value;
      printf(format + 1, values...); // recursive call
      return;
    }
    std::cout << *format;
  }
}

// Printfln
void printfln(const char *);

template <typename T, typename... Ts>
void printfln(const char *format, T value, Ts... values) {
  for (; *format != '\0'; format++) {
    if (*format == '$') {
      std::cout << value;
      printfln(format + 1, values...); // recursive call
      return;
    }
    std::cout << *format;
  }
}

} // namespace supp

/* General math functions

  padder:
  Sum of arguments to a power p

  pnorm:
  Returns p-norm using arguments

  */
namespace math {

static const real pi = std::atan2(0, -1);

// p-Adder
template <typename T> T padder(size_t p, T n) { return pow(n, p); }
template <typename T, typename... Ts> T padder(size_t p, T n, Ts... ns) {
  return pow(n, p) + padder(p, ns...);
}

// p-norm
template <typename... Ts> real pnorm(size_t p, Ts... ns) {
  return pow(padder(p, ns...), 1.0 / p);
}

} // namespace math

/* Complex number */

struct complex {

  bool imag;   // comp: this number has imaginary part
  real re, im; // re: real part, im: imaginary part

  // Constructors
  // Default constructor
  complex(void);
  // Both real and imaginary parts given
  complex(real, real);
  // Only phase given
  complex(real);
  // Real modulus and complex phase given
  complex(real, complex);
  // Complex "modulus" and real phase given (complex + rotation)
  complex(complex, real);

  // Destructor
  ~complex();

  // Swap function
  void swap(complex &);

  // Operators overloading
  // Assignment from a real number
  complex &operator=(const real &);
  // Assignment
  complex &operator=(const complex &);
  // Unary minus
  complex operator-(void) const;
  // Addition with a real number
  complex operator+(const real &) const;
  // Addition
  complex operator+(const complex &) const;
  // Subtraction by a real number
  complex operator-(const real &) const;
  // Subtraction
  complex operator-(const complex &) const;
  // Multiplication by a real number
  complex operator*(const real &)const;
  // Multiplication
  complex operator*(const complex &)const;
  // Division by a real number
  complex operator/(const real &) const;
  // Division
  complex operator/(const complex &) const;

  // Conjugate
  complex conj(void) const;

  // Absolute value
  real abs(void) const;

  // Absolute value squared
  real abs2(void) const;

  // Phase
  real phi(void) const;

  // To string
  std::string tostr(void) const;
  operator std::string() const;

}; // Complex number

/* Overload real operators */

/* Complex functions */

// Costant imaginary unit
const complex Iu(0, 1);

// Complex square root
complex sqrt(complex);

// Complex exponential
complex exp(complex);

// Complex logarithm
complex log(complex);

// Complex sine
complex sin(complex);

// Complex cosine
complex cos(complex);

// Complex tangent
complex tan(complex);

// Complex functions

/* Measurement
Measurement structure with value and uncertainty.
*/
struct Measurement {

  bool unc;     // unc: this number has uncertainty
  real v, u, r; // v: value, u: uncertainty, r: ratio uncertainty/value

  // Constructors
  Measurement(real, real);
  Measurement(real);
  // Destructor
  ~Measurement();

  // Swap function
  void swap(Measurement &);

  // Operators overloading
  Measurement &operator=(const Measurement &);
  Measurement operator+(const Measurement &);
  Measurement operator-(const Measurement &);
  Measurement operator*(const Measurement &);
  Measurement operator/(const Measurement &);
  bool operator<(const Measurement &);
  bool operator>(const Measurement &);

  // To string
  std::string tostr(void);
  operator std::string() const;

}; // Measurement

/* Real function
  f : R x Ts... -> T
 */
template <typename T> class Line {

  // Vector of real number
  typedef std::vector<real> R_vec;

  // Point (real, T)
  typedef std::pair<real, T> Point;
  // Map: vector of points
  typedef std::vector<Point> Map;
  // Iterator over finite elements
  typedef typename Map::iterator Elem_it;

  // Input analytical function
  // typedef std::function<T(real, Ts...)> InFunction;

  // Analytical function stored
  typedef std::function<T(real)> Function;
  // Vector of poiters to analytical functions
  typedef std::vector<Function> Func_vec;

  // Analyitcal part
  /*
  struct Analytical {
    size_t _n_functions;
    Func_vec _functions;
    Args _args;
  };
  */

  // Analytical functions
  Func_vec _functions;
  // Vector with slice points (division of regions between analytical functions)
  R_vec _s_points;
  // Step size in which the domain is diveded
  real _step;
  // Finite elements function evaluations
  Map _elements;
  // Iterator over the finite elements
  Elem_it _ptr;

  // Clear
  void _clear(void) {
    _ptr = nullptr;
    _elements.clear();
  }

  // Lower bound
  Elem_it _lower_bound(real value) {
    return std::lower_bound(
        _elements.begin(), _elements.end(), value,
        [](Point const &p, real x) { return (p.first < x); });
  }

  // Upper bound
  Elem_it _upper_bound(real value) {
    return std::upper_bound(
        _elements.begin(), _elements.end(), value,
        [](real x, Point const &p) { return (x < p.first); });
  }

  // Insert s_point on correct location
  size_t _insert_s_point(real value) {}

public:
  // Constructors

  // Endpoints given
  Line(real value1, real value2) {
    try {
      // Test interval
      if (value1 == value2)
        throw 0;
      // Sort
      if (value1 > value2)
        std::swap(value1, value2);

      // Clear analytical function
      Function zero_function = [](real x) { return 0; };
      _functions.pushback(zero_function);

      // Add points
      _elements.push_back(Point(value1, 0.0));
      _elements.push_back(Point(value2, 0.0));

      _step = value2 - value1;

      // Set element pointer
      _ptr = _elements.begin();

    } catch (...) {
      std::cerr << "Error: endpoints are equal\n";
    }
  }

  // Endpoints and number of points given (equally spaced)
  Line(real value1, real value2, size_t n_points) {
    try {
      // Test interval
      if (value1 == value2)
        throw 0;
      // Sort
      if (value1 > value2)
        std::swap(value1, value2);

      size_t i;

      // Clear analytical function
      Function zero_function = [](real x) { return 0; };
      _functions.pushback(zero_function);

      // Set step
      _step = (value2 - value1) / (n_points - 1);

      // Add first point
      _elements.push_back(Point(value1, 0.0));

      // Add other points
      for (i = 1; i < n_points; i++)
        _elements.push_back(Point(value1 + i * _step, 0.0));

      // Set element pointer
      _ptr = _elements.begin();

    } catch (...) {
      std::cerr << "Error: endpoints are equal\n";
    }
  }

  // Endpoints, number of points and analytical function given (equally spaced)
  template <typename... Ts>
  Line(real value1, real value2, size_t n_points, T (*f)(real, Ts...),
       Ts... args) {
    try {
      // Test interval
      if (value1 == value2)
        throw 0;
      // Sort
      if (value1 > value2)
        std::swap(value1, value2);

      size_t i;
      real x_val;
      T y_val;

      // Set analytical function
      Function function = [f, args...](real x) { return f(x, args...); };
      _functions.push_back(function);

      // Set s_points
      _s_points.push_back(value1);
      _s_points.push_back(value2);

      // Set step
      _step = (value2 - value1) / (n_points - 1);

      // Add points
      for (i = 0; i < n_points; i++) {
        x_val = value1 + i * _step;
        y_val = _functions[0](x_val);
        _elements.push_back(Point(x_val, y_val));
      }

      // Set pointer
      _ptr = _elements.begin();

    } catch (...) {
      std::cerr << "Error: endpoints are equal\n";
    }
  }

  // Iteration operators

  // Deference pointer
  real x(void) { return _ptr->first; }
  T y(void) { return _ptr->second; }

  // Value at index
  real x(size_t index) {
    try {
      if (index >= _elements.size())
        throw 0;

      auto pointer = _elements.begin() + index;

      return pointer->first;

    } catch (...) {
      std::cerr << "Error: Out of bounds. Returning value at pointer." << '\n';
      return _ptr->first;
    }
  }
  T y(size_t index) {
    try {
      if (index >= _elements.size())
        throw 0;

      auto pointer = _elements.begin() + index;

      return pointer->second;

    } catch (...) {
      std::cerr << "Error: Out of bounds. Returning value at pointer." << '\n';
      return _ptr->second;
    }
  }

  // Increment pointer
  Line &operator++() {
    if (_ptr != _elements.end() - 1)
      _ptr++;
    else
      std::cout << "Warning: Already at the end of line." << '\n';
    return *this;
  }

  // Decrement pointer
  Line &operator--() {
    if (_ptr != _elements.begin())
      _ptr--;
    else
      std::cout << "Warning: Already at the beginning of line." << '\n';
    return *this;
  }

  // Return point in index
  Point operator[](size_t index) {
    try {
      if (index >= _elements.size())
        throw 0;

      auto pointer = _elements.begin() + index;

      return *pointer;

    } catch (...) {
      std::cerr << "Erro: Out of bounds" << '\n';
      return *_ptr;
    }
  }

  // Jump to index
  void operator()(size_t index) {
    try {
      if (index >= _elements.size())
        throw 0;

      auto pointer = _elements.begin() + index;
      _ptr = pointer;

    } catch (...) {
      std::cerr << "Erro: Out of bounds" << '\n';
    }
  }

  // Setters

  // Set analytical function everywhere
  template <typename... Ts> void set_function(T (*f)(real, Ts...), Ts... args) {
    // Prepare analytical function
    Function function = [f, args...](real x) { return f(x, args...); };
    _functions.clear();
    _functions.push_back(function);
    // Clear slice points
    _s_points.clear();
    _s_points.push_back(_elements.front().first);
    _s_points.push_back(_elements.back().first);
    // Set values
    for (auto &element : _elements)
      element.second = function(element.first);
  }

  // Set analytical function on intervals (std::function)
  void set_function(real value1, real value2, Function function) {
    // Sort
    if (value1 > value2)
      std::swap(value1, value2);

    // Store number of points
    size_t n_points = _elements.size();

    // Auxiliary variables
    size_t i, j;
    real x_val, x_0;
    T y_val;

    // Set s_points and functions

    // Find slice point >= value1
    R_vec::iterator it_s_points =
        std::lower_bound(_s_points.begin(), _s_points.end(), value1);
    size_t pos = it_s_points - _s_points.begin();
    // Put it there
    it_s_points = ++_s_points.insert(it_s_points, value1);
    // Insert function
    _functions.insert(_functions.begin() + pos, function);
    // Slice previous function
    _functions.insert(_functions.begin() + pos + 1, _functions[pos - 1]);
    // Delete everything until value2
    while (it_s_points !=
           std::upper_bound(_s_points.begin(), _s_points.end(), value2)) {
      it_s_points = _s_points.erase(it_s_points);
      _functions.erase(_functions.begin() + pos + 1);
    }
    // Put it there
    it_s_points = _s_points.insert(it_s_points, value2);

    // Set step
    _step = (_s_points.back() - _s_points.front()) / (n_points - 1);

    // Fill elements
    _elements.clear();
    // First Point
    x_0 = _s_points.front();
    y_val = _functions[0](x_0);
    _elements.push_back(Point(x_0, y_val));
    // Other points
    for (i = 1; i < n_points; i++) {
      x_val = x_0 + i * _step;
      j = std::lower_bound(_s_points.begin(), _s_points.end(), x_val) -
          _s_points.begin();
      y_val = _functions[j - 1](x_val);
      _elements.push_back(Point(x_val, y_val));
    }
  }

  // Set analytical function on interval (pointer to function)
  template <typename... Ts>
  void set_function(real value1, real value2, T (*f)(real, Ts...), Ts... args) {

    // Prepare analytical function
    Function function = [f, args...](real x) { return f(x, args...); };
    // Call main function
    set_function(value1, value2, function);
  }

  // Set endpoints
  void set_endpoints(real value1, real value2) {
    try {
      // Test interval
      if (value1 == value2)
        throw 0;
      // Sort
      if (value1 > value2)
        std::swap(value1, value2);

      // Auxialiry variables
      size_t pos, i, j;
      real x_0, x_val;
      T y_val;

      // Store number of points
      size_t n_points = _elements.size();

      // Set s_points

      // Delete first s_point
      _s_points.erase(_s_points.begin());
      // Find position of value1
      R_vec::iterator it_s_points =
          std::lower_bound(_s_points.begin(), _s_points.end(), value1);
      pos = it_s_points - _s_points.begin();
      // Put it there if its not already
      if (*it_s_points != value1)
        it_s_points = _s_points.insert(it_s_points, value1);
      // Delete everything before that
      _s_points.erase(_s_points.begin(), it_s_points);
      _functions.erase(_functions.begin(), _functions.begin() + pos);

      // Delete last s_point
      _s_points.erase(_s_points.end() - 1);
      // Find position of value2
      it_s_points =
          std::upper_bound(_s_points.begin(), _s_points.end(), value2);
      pos = it_s_points - _s_points.begin();
      // Put it there if its not already
      if (*it_s_points != value2)
        it_s_points = _s_points.insert(it_s_points, value2);
      // Delete everything after that
      _s_points.erase(it_s_points + 1, _s_points.end());
      _functions.erase(_functions.begin() + pos, _functions.end());

      // Set step
      _step = (value2 - value1) / (n_points - 1);

      // Fill elements
      _elements.clear();
      // Fist Point
      x_0 = _s_points.front();
      y_val = _functions[0](x_0);
      _elements.push_back(Point(x_0, y_val));
      // Other points
      for (i = 1; i < n_points; i++) {
        x_val = x_0 + i * _step;
        j = std::lower_bound(_s_points.begin(), _s_points.end(), x_val) -
            _s_points.begin() - 1;
        y_val = _functions[j](x_val);
        //  std::cerr << x_val << '\t' << y_val << '\n';
        _elements.push_back(Point(x_val, y_val));
      }
    } catch (...) {
      std::cerr << "Error: endpoints are equal\n";
    }
  }

  void set_n_points(size_t n_points) {
    try {
      if (n_points < 2)
        throw 0;

      // Auxialiry variables
      size_t i, j;
      real x_0, x_val;
      T y_val;

      // Set step
      _step = (_s_points.back() - _s_points.front()) / (n_points - 1);

      // Fill elements
      _elements.clear();
      // Fist Point
      x_0 = _s_points.front();
      y_val = _functions[0](x_0);
      _elements.push_back(Point(x_0, y_val));
      // Other points
      for (i = 1; i < n_points; i++) {
        x_val = x_0 + i * _step;
        j = std::lower_bound(_s_points.begin(), _s_points.end(), x_val) -
            _s_points.begin() - 1;
        y_val = _functions[j](x_val);
        //  std::cerr << x_val << '\t' << y_val << '\n';
        _elements.push_back(Point(x_val, y_val));
      }

    } catch (...) {
      std::cerr << "Error: at least two points are required\n";
    }
  }

  void set_zero(real value1, real value2) {
    Function zero_function = [](real x) -> T { return 0; };
    set_function(value1, value2, zero_function);
  }

  // Getters

  // Analytical function associated
  T f(real x) {

    // Check boundaries
    if (x <= _s_points.back())
      return _functions.back()(x);
    if (x >= _s_points.front())
      return _functions.front()(x);

    R_vec::iterator it_s_points =
        std::lower_bound(_s_points.begin(), _s_points.end(), x);

    size_t j = it_s_points - _s_points.begin() - 1;

    return _functions[j](x);
  }

  // Get start point
  real start(void) { return _elements.front().first; }
  // Get final point
  real end(void) { return _elements.back().first; }

  // Get length
  real length(void) {
    return (_elements.back().first - _elements.front().first);
  }

  // Set Step
  real step(void) { return _step; }

  // Set y-value
  void set_y(size_t index, T value) { _elements[index].second = value; }

  // Get number of points
  size_t n_points(void) { return _elements.size(); }

  // Get number of analytical functions
  size_t n_func(void) { return _functions.size(); }

  // Debugging

  // Show all points in order for debugging
  void print_points(void) {
    std::cerr << "print_points:" << '\n';
    // typename Map::iterator it;
    for (const auto &element : _elements)
      std::cerr << element.first << "\t\t" << std::string(element.second)
                << '\n';
    std::cerr << "\n";
  }

  // Show all s_points in order for debugging
  void print_s_points(void) {
    std::cerr << "print_s_points:" << '\n';
    // typename Map::iterator it;
    for (const auto &s_point : _s_points)
      std::cerr << s_point << '\n';
    std::cerr << "\n";
  }

  // Debug
  void debug(void) {
    std::cerr << "DEBUG LOG\n\n";

    std::cerr << "number of points = " << n_points() << '\n';
    std::cerr << "number of functions = " << n_func() << '\n';

    print_points();
    print_s_points();

    std::cerr << "length = " << length() << "\n\n";
  }
}; // Line

/*    1D Quantum system     */

class QuantumSys {

  Line<real> _pot;
  Line<complex> _psi;

public:
  // Constructor
  QuantumSys(Line<real> pot, Line<complex> psi0) : _pot(pot), _psi(psi0) {}

  // Integrate
  real integral(void) {

    size_t n_points = _psi.n_points();
    double dx = _psi.step();

    size_t i;

    double sum = 0;

    for (i = 0; i < n_points; i++)
      sum += dx * _psi[i].second.abs2();

    return sum;
  }

  // Normalize
  void normalize(void) {

    size_t i;
    size_t n_points = _psi.n_points();
    complex temp(0, 0);

    real sum = std::sqrt(integral());

    for (i = 0; i < n_points; i++) {
      temp = _psi[i].second;
      _psi.set_y(i, temp / sum);
    }
  }

  // Update

  // Euler method
  void Euler(double dt) {
    size_t i;

    size_t n_points = _psi.n_points();
    double dx = _psi.step();

    double kappa = dt / (2 * dx * dx);

    complex temp(0, 0);

    for (i = 1; i < n_points - 1; i++) {
      temp = complex(1, -2 * kappa - dt * _pot[i].second) * _psi[i].second;
      temp =
          temp + complex(0, kappa) * (_psi[i - 1].second + _psi[i + 1].second);
      _psi.set_y(i, temp);
    }
  }

  // Crank-Nicolson method
  void CrakNicolson(double dt) {

    size_t i, j, k;

    size_t n_points = _psi.n_points();
    size_t n = n_points - 2;
    double dx = _psi.step();

    double kappa = dt / (2 * dx * dx);
    complex beta(0, -kappa / 2);
    std::vector<complex> B(n), alpha(n), chi(n), gamma(n), phi(n);

    complex temp(0, 0);

    // Fill B
    for (i = 0; i < n; i++) {
      B[i] = kappa + dt * _pot[i + 1].second / 2;
    }

    // Fill alpha
    for (i = 0; i < n; i++)
      alpha[i] = complex(1, -B[i]);

    // Fill chi
    for (i = 0; i < n; i++) {
      chi[i] = alpha[i] * _psi[i + 1].second +
               beta * (_psi[i].second + _psi[i + 2].second);
    }

    // Calculate gamma
    gamma[0] = beta / alpha[0];
    for (i = 1; i < n; i++)
      gamma[i] = beta / (alpha[j] - beta * gamma[i - 1]);

    // Calculate phi
    phi[0] = (chi[0] - beta * _psi[0].second) / alpha[0];
    for (i = 1; i < n - 1; i++)
      phi[i] = (chi[i] - beta * phi[i - 1]) / (alpha[i] - beta * gamma[i - 1]);
    phi[n] = (chi[n] - beta * (_psi[n_points - 1].second + phi[n - 1])) /
             (alpha[n] - beta * gamma[n - 1]);

    // Update psi
    _psi.set_y(n_points - 1, phi[n]);
    for (i = n_points - 2; i > 0; i--) {
      _psi.set_y(i, phi[i - 1] - gamma[i - 1] * _psi[i + 1].second);
    }
  }

  // Output

  // Output modulus sqrd of wavefunction to gnuplot interactive terminal
  void out_gnuplot(void) {

    size_t j, n_points = _psi.n_points();

    // Setup GNUPLOT
    std::cout << "set key off" << std::endl;
    std::cout << "set xrange [" << _psi.start() << ':' << _psi.end() << ']'
              << std::endl;
    std::cout << "set yrange [" << 0 << ":]" << std::endl;

    // Call interactive terminal
    // std::cout << "plot \"-\" w p pt 7 ps 0.5" << std::endl;
    std::cout << "plot \"-\" w l" << std::endl;

    for (j = 0; j < n_points; j++)
      std::cout << _psi[j].first << "\t\t" << _psi[j].second.abs2() << '\n';

    std::cout << 'e' << std::endl;
  }
};

// 1D Quantum system

// Differential Equations

//

/*  Runge-Kutta 4th order method

Problem:
Solve for y(t) in
dy/dt = f(t,y,args)

f : real x Ty x Ts... -> Ty

Method:
Returns y_(n+1) = y(t_n + h) using an approximation of order 4

h       : time step
f       : f(t,y,args)
t       : previous value of t
y       : previous value of y
args... : other arguments for f
*/
template <typename Ty, typename... Ts>
void rk4(real h, Ty (*f)(real, Ty, Ts...), real &t, Ty &y, Ts... args) {
  // Coefficients
  Ty k1, k2, k3, k4;
  // Iterators
  typename Ty::iterator it_y, it_k1, it_k2, it_k3, it_k4;

  for (it_y = y.begin(); it_y != y.end(); it_y++) {
    // Calculate coefficients
    k1 = f(t, y, args...);
    k2 = f(t + h / 2, y + h * k1 / 2, args...);
    k3 = f(t + h / 2, y + h * k2 / 2, args...);
    k4 = f(t + h, y + h * k3, args...);
    // Calculate next step
    *it_y += h * (k1 + 2 * k2 + 2 * k3 + k4) / 6;
  }
  t += h;
}

// Runge-Kutta 4 specialization for real y
template <typename Ty = real, typename... Ts>
void rk4(real h, real (*f)(real, real, Ts...), real &t, real &y, Ts... args) {
  real k1, k2, k3, k4;

  // Calculate coefficients
  k1 = f(t, y, args...);
  k2 = f(t + h / 2, y + h * k1 / 2, args...);
  k3 = f(t + h / 2, y + h * k2 / 2, args...);
  k4 = f(t + h, y + h * k3, args...);

  // Calculate next step
  y += h * (k1 + 2 * k2 + 2 * k3 + k4) / 6;
  t += h;
}

// Finite elements
