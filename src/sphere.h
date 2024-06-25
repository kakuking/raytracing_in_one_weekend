#pragma once

#include "rtweekend.h"
#include "hittable.h"

class sphere: public hittable {
    private:
        point3 center;
        double radius;

    public:
        sphere(const point3& center, double radius): center(center), radius(fmax(0, radius)) {}

        bool hit(const ray& r, double ray_tmin, double ray_tmax, hit_record& rec) const override {
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
            if(root <= ray_tmin || ray_tmax <= root){
                root = (h + sqrtd)/a;
                if (root <= ray_tmin || ray_tmax <= root)
                    return false;
            }

            rec.t = root;
            rec.p = r.at(rec.t);
            vec3 outward_normal = (rec.p - center) / radius;
            rec.set_face_normal(r, outward_normal);

            return true;
        }
};