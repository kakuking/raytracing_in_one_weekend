#pragma once

#include <cmath>
#include <iostream>

using std::sqrt;

class vec3 {
    public:
        double  e[3];

        vec3(): e{0, 0, 0} {}
        vec3(double e0, double e1, double e2): e{e0, e1, e2} {}
        vec3(const vec3& v): e{v[0], v[1], v[2]} {}

        double x() const { return e[0]; }
        double y() const { return e[1]; }
        double z() const { return e[2]; }

        vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
        double operator[](int i) const {return e[i]; }
        double& operator[](int i) { return e[i]; }

        vec3& operator+=(const vec3& v) {
            e[0] += v[0];
            e[1] += v[1];
            e[2] += v[2];
            return *this;
        }

        vec3& operator*=(double v) {
            e[0] *= v;
            e[1] *= v;
            e[2] *= v;
            return *this;
        }

        vec3& operator/=(double v) {
            return *this *= 1/v;
        }

        double length() const {
            return sqrt(length_squared());
        }

        double length_squared() const {
            return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
        }
};

using point3 = vec3;
// using color3 = vec3;

inline std::ostream& operator<<(std::ostream& out, const vec3& v) {
    return out << v[0] << ' ' << v[1] << ' ' << v[2];
}

inline vec3 operator+(const vec3& u, const vec3& v) {
    return vec3(u[0] + v[0], u[1] + v[1], u[2] + v[2]);
}

inline vec3 operator-(const vec3& u, const vec3& v) {
    return vec3(u[0] - v[0], u[1] - v[1], u[2] - v[2]);
}

inline vec3 operator*(const vec3& u, const vec3& v) {
    return vec3(u[0] * v[0], u[1] * v[1], u[2] * v[2]);
}

inline vec3 operator*(const vec3& v, double t) {
    return vec3(t*v[0], t*v[1], t*v[2]);
}

inline vec3 operator*(double t, const vec3& v) {
    return v * t;
}

inline vec3 operator/(const vec3& v, double t) {
    return v * (1/t);
}

inline double dot(const vec3& u, const vec3& v) {
    return u[0] * v[0] + u[1] * v[1] + u[2] * v[2];
}

inline vec3 cross(const vec3& u, const vec3& v) {
    return vec3(u[1]*v[2] - u[2]*v[1], u[2]*v[0] - u[0]*v[2], u[0]*v[1] - u[1]*v[0]);
}

inline vec3 uint_vector(const vec3& v) {
    return v / v.length();
}
