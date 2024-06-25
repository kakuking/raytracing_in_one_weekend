#pragma once

#include <cmath>
#include <random>
#include <iostream>
#include <limits>
#include <memory>
#include "interval.h"

using std::fabs;
using std::make_shared;
using std::shared_ptr;
using std::sqrt;

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

const interval interval::empty = interval(+infinity, -infinity);
const interval interval::universe = interval(-infinity, +infinity);

inline double degrees_to_radians(double degrees) {
    return degrees * pi/180.0;
}

inline double random_double() {
    return rand()/(RAND_MAX + 1.0);
}

inline double random_double(double min, double max) {
    return min + (max-min)*random_double();
}

#include "color.h"
#include "ray.h"
#include "vec3.h"