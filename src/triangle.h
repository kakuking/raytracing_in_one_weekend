#pragma once

#include "rtweekend.h"
#include "hittable.h" 
#include "material.h"

using coefficients = vec3;

class triangle: public hittable{
    public:
        vertex3 v1, v2, v3;
        shared_ptr<material> mat;

        triangle() {}
        triangle(vertex3 v1, vertex3 v2, vertex3 v3, shared_ptr<material> mat): v1(v1), v2(v2), v3(v3), mat(mat) {}

        bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
            std::optional<hit_record> hit_rec_opt = ray_triangle_intersection(r);

            if(!hit_rec_opt.has_value())
                return false;
            
            rec = hit_rec_opt.value();
            rec.mat = mat;

            return true;
        }

        coefficients barycentric_of(const vertex3& p) {
            double v1v2p = area_of_triangle(v1, v2, p);
            double v1v3p = area_of_triangle(v1, v3, p);
            double v2v3p = area_of_triangle(v2, v3, p);

            double v1v2v3 = area_of_triangle(v1, v2, v3);

            return coefficients(v2v3p/v1v2v3, v1v3p/v1v2v3, v1v2p/v1v2v3);
        }

        vertex3 barycentric_at(const coefficients& coeff) const {
            point3 p = v1.position * coeff[0] + v2.position * coeff[1] + v3.position * coeff[2];

            vec3 normal = v1.normal * coeff[0] + v2.normal * coeff[1] + v3.normal * coeff[2];

            return vertex3(p, normal);
        }

        std::optional<hit_record> ray_triangle_intersection(const ray& r) const {
            // if ray is parallel return -1
            if (is_ray_parallel(r))
                return {};
            
            std::optional<coefficients> coeff_opt = find_ray_intersection(r);

            // if no solution/solution out of bounds
            if (!coeff_opt.has_value())
                return {};
            
            // contains (t, u, v) where t is for ray, u and v are for triangle
            coefficients coeff = coeff_opt.value();

            // Need to derive first coeff from the other 2
            hit_record rec;
            vertex3 hit_p = barycentric_at(coefficients(1-coeff[1]-coeff[2], coeff[1], coeff[2]));

            rec.p = hit_p.position;
            rec.t = coeff[0];
            rec.set_face_normal(r, hit_p.normal);

            return rec;
        }

    private:
        double area() {
            return area_of_triangle(v1, v2, v3);
        }

        bool is_ray_parallel(const ray& r) const {
            return dot(cross(v2.position - v1.position, v3.position-v2.position), r.direction()) == 0;
        }

        std::optional<coefficients> find_ray_intersection(const ray& r) const {
            vec3 AB = v2.position - v1.position;
            vec3 AC = v3.position - v1.position;
            vec3 ray_cross_AC = cross(r.direction(), AC);

            double det = dot(AB, ray_cross_AC);

            double inv_det = 1.0/det;

            vec3 s = r.origin() - v1.position;
            double u = inv_det * dot(s, ray_cross_AC);

            if (u < 0 || u > 1) 
                return {};
            
            vec3 s_cross_AB = cross(s, AB);
            float v = inv_det * dot(r.direction(), s_cross_AB);

            if (v < 0 || u + v > 1)
                return {};

            float t = inv_det * dot(AC, s_cross_AB);
            if(t > 0.001) {
                return coefficients(t, u, v);
            }
            return {};
        }
};

inline std::ostream& operator<<(std::ostream& out, const triangle& t) {
    return out << t.v1 << ", " << t.v2 << ", " << t.v3; //" color - (" << t.mat;
}