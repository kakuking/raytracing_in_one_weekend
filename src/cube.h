#pragma once

#include "rtweekend.h";
#include "mesh.h"

class cube: mesh {
    public:
        cube() {}
        cube(point3 bottom_left, double edge_length): bottom_left(bottom_left), edge_length(edge_length) {}

        point3 bottom_left;
        double edge_length;

        point3 vertex3[8] = {point3(0, 0, 0), point3(1, 0, 0), point3(1, 1, 0), point3(0, 1, 0),
        point3(0, 0, 1), point3(1, 0, 1), point3(1, 1, 1), point3(0, 1, 1)};

        int indices[12][3] = {{0, 1, 2}, {2, 3, 0}, {4, 5, 6}, {6, 7, 0} };

        void generate() {
            for (size_t i = 0; i < 12; i++)
            {
                
            }
            
        }

};