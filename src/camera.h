#pragma once

#include "rtweekend.h"
#include "hittable.h"
#include "material.h"

#include <thread>

class camera {
    public:
        double aspect_ratio = 1.0;
        int image_width = 100;
        int samples_per_pixel = 10;
        int max_depth = 10;

        double vfov = 90;
        point3 lookfrom = point3(0, 0, 0);
        point3 lookat = point3(0, 0, -1);
        vec3 vup = vec3(0, 1, 0);

        double defocus_angle = 0;
        double focus_dist = 10;

        int num_threads = 5;

        camera(hittable& w): world(w) {}

        void render(const hittable& in_world){
            initialize();
            world = in_world;

            std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

            scanlines_per_thread = image_height/num_threads;

            for (int t = 0; t < num_threads; t++)
            {
                int start = t * scanlines_per_thread;
                int end = (t == num_threads - 1) ? image_height : (t + 1) * scanlines_per_thread;

                threads.emplace_back(&camera::process_scanline, this, start, end, t);
            }
            
            for (auto& thread: threads) {
                thread.join();
            }

            std::clog << "Render done, saving to file       \n";
            
            for(auto& color: image){
                write_color(std::cout, color);
            }
            std::clog << "\rDone.                 \n";
        }

    private:
        int image_height;
        point3 center;
        point3 pixel00_loc;
        vec3 pixel_delta_u;
        vec3 pixel_delta_v;
        double pixel_samples_scale;
        vec3 u, v, w;
        vec3 defocus_disc_u;
        vec3 defocus_disc_v;

        // multithreading
        int scanlines_per_thread;
        std::vector<std::thread> threads;
        std::vector<color> image;

        hittable& world;

        void initialize(){
            image_height = int(image_width/aspect_ratio);
            image_height = image_height < 1 ? 1 : image_height;

            pixel_samples_scale = 1.0/samples_per_pixel;


            // Camera stuff
            center = lookfrom;

            // auto focal_length = (lookfrom - lookat).length();

            auto theta = degrees_to_radians(vfov);
            auto h = tan(theta/2);
            auto viewport_height = 2 * h * focus_dist;
            auto viewport_width = viewport_height * (double(image_width)/image_height);

            w = unit_vector(lookfrom - lookat);
            u = unit_vector(cross(vup, w));
            v = cross(w, u);

            auto viewport_u = viewport_width * u;
            auto viewport_v = viewport_height * -v;

            pixel_delta_u = viewport_u/image_width;
            pixel_delta_v = viewport_v/image_height;

            auto viewport_upper_left = center - (focus_dist*w) - viewport_u/2 - viewport_v/2;
            pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

            auto defocus_radius = focus_dist * tan(degrees_to_radians(defocus_angle/2));

            defocus_disc_u = u * defocus_radius;
            defocus_disc_v = v * defocus_radius;

            // Multithreading
            image.resize(image_width * image_height);
        }

        color ray_color(const ray& r, int depth, const hittable& world) const {
            if (depth <= 0)
                return color(0, 0, 0);

            hit_record rec;
            if(world.hit(r, interval(0.001, infinity), rec)){
                ray scattered;
                color attenuation;

                if(rec.mat->scatter(r, rec, attenuation, scattered))
                    return attenuation * ray_color(scattered, depth-1, world);
                return color(0, 0, 0);
            }

            vec3 unit_direction = unit_vector(r.direction());
            auto a = 0.5*(unit_direction.y() + 1.0);
            return color((1-a)*color(1, 1, 1) + a*color(0.5, 0.7, 1.0));
        }

        ray get_ray(int i, int j) const {
            auto offset = sample_square();
            auto pixel_sample = pixel00_loc + ((i + offset.x()) * pixel_delta_u) + ((j + offset.y()) * pixel_delta_v);

            auto ray_origin = (defocus_angle <= 0) ? center: defocus_disc_sample();
            auto ray_direction = pixel_sample - ray_origin;

            return ray(ray_origin, ray_direction);
        }

        point3 defocus_disc_sample() const {
            auto p = random_in_unit_disk();

            return center + (p[0] * defocus_disc_u) + (p[1] * defocus_disc_v);
        }

        vec3 sample_square() const {
            return vec3(random_double() - 0.5, random_double() - 0.5, 0);
        }

        // Multithreading
        void process_scanline(int start, int end, int thread_index) {
            for (int j = start; j < end; j++) {
                // std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
                std::clog << "\nThread " << thread_index << "Processing scanline: " << (j - start + 1) << ' ' << std::flush;
                for (int i = 0; i < image_width; i++) { 
                    color pixel_color(0, 0, 0);
                    for(int sample = 0; sample < samples_per_pixel; sample++){
                        ray r = get_ray(i, j);
                        pixel_color += ray_color(r, max_depth, world);
                    }           

                    // write_color(std::cout, pixel_color * pixel_samples_scale);
                    size_t index = j * image_width + i;
                    image[index] = pixel_color * pixel_samples_scale;
                }
            }
        }
};