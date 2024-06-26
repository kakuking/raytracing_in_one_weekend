#pragma once

#include "rtweekend.h"
#include "hittable.h"

class sphere: public hittable {
    private:
        point3 center;
        double radius;
        shared_ptr<material> mat;

    public:
        sphere(const point3& center, double radius, shared_ptr<material> mat): center(center), radius(fmax(0, radius)), mat(mat) {}

        bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
            vec3 oc = center - r.origin();
            auto a = dot(r.direction(), r.direction());
            auto h = dot(r.direction(), oc);
            auto c = dot(oc, oc) - radius*radius;

            auto dis = h*h - a*c;

            if(dis < 0) {
                return false;
            }

            auto sqrtd = sqrt(dis);

            auto root = (h-sqrtd)/a;
            if(!ray_t.surrounds(root)){
                root = (h + sqrtd)/a; 
                if (!ray_t.surrounds(root))
                    return false;
            }

            rec.t = root;
            rec.p = r.at(rec.t);
            vec3 outward_normal = (rec.p - center) / radius;
            rec.set_face_normal(r, outward_normal);
            rec.mat = mat;

            return true;
        }
};