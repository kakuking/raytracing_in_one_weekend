#pragma once

#include "rtweekend.h"
#include "hittable.h"

class hit_record;

class material{
    public:
        virtual ~material() = default;

        virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const {
            return false;
        }
};

class lambertian: public material {
    private:
        color albedo;

    public:
        lambertian(const color& albedo): albedo(albedo) {}

        bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
            auto scatter_direction = rec.normal + random_unit_vector();

            if (scatter_direction.near_zero())
                scatter_direction = rec.normal;

            scattered = ray(rec.p, scatter_direction);
            attenuation = albedo;
            return true;
        }
};

class metal: public material {
    private:
        color albedo;
        double fuzz;

    public:
        metal(const color& albedo, double fuzz) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}

        bool scatter(const ray& ray_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
            vec3 reflected = reflect(ray_in.direction(), rec.normal);
            reflected = uint_vector(reflected) + fuzz * random_unit_vector();
            scattered = ray(rec.p, reflected);
            attenuation = albedo;
            return (dot(scattered.direction(), rec.normal) > 0);
        }
};