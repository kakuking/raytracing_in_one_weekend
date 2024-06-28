#pragma once

#include "rtweekend.h"
#include "hittable.h"

class mesh: public hittable {
    public:
        mesh() {}
        mesh(std::vector<triangle> tris, shared_ptr<material> mat): tris(tris), mat(mat) {}
        mesh(triangle tris_array[], size_t count, shared_ptr<material> mat): mat(mat) {
            tris.resize(count);
            for (size_t i = 0; i < count; i++)
                tris.push_back(tris_array[i]);
        }

        bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
            return brute_force(r, ray_t, rec);
        }

        protected:
            std::vector<triangle> tris;
            shared_ptr<material> mat;
        private:

        bool brute_force(const ray& r, interval ray_t_init, hit_record& rec) const {
            bool any_hit = false;
            interval ray_t = ray_t_init;

            for(auto& tri: tris) {
                std::optional<hit_record> vert_opt = tri.ray_triangle_intersection(r);

                if (!vert_opt.has_value())
                    continue;

                hit_record hit_rec = vert_opt.value();

                if(!ray_t.surrounds(hit_rec.t)){
                    continue;
                }

                any_hit = true;
                rec.t = hit_rec.t;
                rec.p = hit_rec.p;
                rec.normal = hit_rec.normal;
                rec.mat = mat;

                ray_t.max = hit_rec.t;
            }

            return any_hit;
        }

};