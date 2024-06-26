#pragma once

#include "rtweekend.h"

class icosahedron  {
    public:
        point3 center;
        double radius;
        bool shade_smooth;

        icosahedron() {}
        icosahedron(point3 center, double radius): center(center), radius(radius) {}
        icosahedron(point3 center, double radius, bool shade_smooth): center(center), radius(radius), shade_smooth(shade_smooth) {}

        std::vector<triangle> generate_icosahedron(){
            std::vector<triangle> triangles;

            const float X = 0.525731112119133606f;
            const float Z = 0.850650808352039932f;
            
            vec3 vertices[12] = {
                vec3(-X, 0.0f, Z), vec3(X, 0.0f, Z), vec3(-X, 0.0f, -Z), vec3(X, 0.0f, -Z),
                vec3(0.0f, Z, X), vec3(0.0f, Z, -X), vec3(0.0f, -Z, X), vec3(0.0f, -Z, -X),
                vec3(Z, X, 0.0f), vec3(-Z, X, 0.0f), vec3(Z, -X, 0.0f), vec3(-Z, -X, 0.0f)
            };
            
            int indices[20][3] = {
                {0, 4, 1}, {0, 9, 4}, {9, 5, 4}, {4, 5, 8}, {4, 8, 1},
                {8, 10, 1}, {8, 3, 10}, {5, 3, 8}, {5, 2, 3}, {2, 7, 3},
                {7, 10, 3}, {7, 6, 10}, {7, 11, 6}, {11, 0, 6}, {0, 1, 6},
                {6, 1, 10}, {9, 0, 11}, {9, 11, 2}, {9, 2, 5}, {7, 2, 11}
            };
            
            for (int i = 0; i < 20; ++i) {
                point3 p1 = vertices[indices[i][0]] * radius + center;
                point3 p2 = vertices[indices[i][1]] * radius + center;
                point3 p3 = vertices[indices[i][2]] * radius + center;

                vec3 n1, n2, n3;
                if (shade_smooth){
                    n1 = p1 - center;
                    n2 = p2 - center;
                    n3 = p3 - center;
                } else {
                    n1 = -center + (p1 + p2 + p3)/3;
                    n2 = -center + (p1 + p2 + p3)/3;
                    n3 = -center + (p1 + p2 + p3)/3;
                }

                vertex3 v1(p1, n1), v2(p2, n2), v3(p3, n3);
                triangles.push_back(triangle(v1, v2, v3));
            }
            
            return triangles;
        }
};