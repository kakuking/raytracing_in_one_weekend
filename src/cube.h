#pragma once

#include "rtweekend.h"
#include "mesh.h"
#include "quad.h"

class cube {
    public:
        cube() {}
        cube(point3 bottom_left, double edge_length, point3 center): bottom_left(bottom_left), edge_length(edge_length), center(center) {}

        point3 bottom_left, center;
        double edge_length;

        point3 points[8] = {point3(0, 0, 0), point3(1, 0, 0), point3(1, 1, 0), point3(0, 1, 0),
        point3(0, 0, 1), point3(1, 0, 1), point3(1, 1, 1), point3(0, 1, 1)};

        int indices[12][4] = {{0, 1, 2, 3}, {4, 5, 6, 7}, {0, 4, 7, 3}, {1, 5, 6, 2}, {2, 6, 7, 3}, {1, 5, 4, 0}};

        std::vector<triangle> generate() {
            std::vector<triangle> t, ret;
            for (size_t i = 0; i < 6; i++)
            {
                t = quad::create_quad(points[indices[i][0]], points[indices[i][1]], points[indices[i][2]], points[indices[i][3]], center);

                ret.insert(ret.end(), t.begin(), t.end());
            }

            return ret;
        }

};