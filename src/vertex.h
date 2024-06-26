#pragma once

#include "rtweekend.h"

class vertex3 {
    public:
        point3 position = point3(0, 0, 0);
        vec3 normal = vec3(1, 0, 0);

        vertex3() {}
        vertex3(point3 position, vec3 normal_vector): position(position) {
            setNormal(normal_vector);
        }
        
        void setNormal(const vec3& normal_vector) {
            normal = unit_vector(normal_vector);
        }

        double x() const { return position[0]; }
        double y() const { return position[1]; }
        double z() const { return position[2]; }
};

inline std::ostream& operator<<(std::ostream& out, const vertex3& v) {
    return out << "Position: " << v.position << " and Normal: " << v.normal;
}

inline vertex3 operator*(vertex3& v, double t) {
    return vertex3(v.position * t, v.normal * t);
}

inline double area_of_triangle(const vertex3& v1, const vertex3& v2, const vertex3& v3) {
    return cross(v2.position - v1.position, v3.position - v1.position).length() * 0.5;
}