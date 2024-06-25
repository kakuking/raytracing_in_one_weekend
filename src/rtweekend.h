#pragma once

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include "interval.h"

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

#include "color.h"
#include "ray.h"
#include "vec3.h"