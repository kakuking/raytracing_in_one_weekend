#pragma once

#include "rtweekend.h"
#include "triangle.h"
#include <tuple>

class quad {
    public:
        static std::vector<triangle> create_quad(point3 p1, point3 p2, point3 p3, point3 p4, point3 center) {
            vec3 outward_normal, normal = unit_vector(cross(p3-p1, p2-p1));
            vec3 to_out = p1 - center;

            outward_normal = dot(to_out, normal) > 0 ? normal : -normal;

            vertex3 v1 = vertex3(p1, outward_normal);
            vertex3 v2 = vertex3(p2, outward_normal);
            vertex3 v3 = vertex3(p3, outward_normal);
            vertex3 v4 = vertex3(p4, outward_normal);

            triangle t1(v1, v2, v3), t2(v3, v4, v1);

            std::vector<triangle> ret;

            ret.push_back(t1);            
            ret.push_back(t2);

            return ret;            
        }
};