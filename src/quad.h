#pragma once

#include "rtweekend.h"
#include "triangle.h"
#include <tuple>

class quad {
    public:
        static std::tuple<triangle, triangle> create_quad(point3 p1, point3 p2, point3 p3, point3 p4) {
            vec3 outward_normal = unit_vector(cross(p2-p1, p4-p1));
            vertex3 v1 = vertex3(p1, outward_normal);
            vertex3 v2 = vertex3(p2, outward_normal);
            vertex3 v3 = vertex3(p3, outward_normal);
            vertex3 v4 = vertex3(p4, outward_normal);

            triangle t1(v1, v2, v3), t2(v3, v4, v1);

            std::vector<triangle> ret;
(t1);
            
            std::make_tuple(t1, t2);
        }
};