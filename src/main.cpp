#include "rtweekend.h"
#include "camera.h"
#include "hittable_list.h"
#include "sphere.h"
#include "material.h"

#include <chrono>

int main(){
    // World
    hittable_list world;

    auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    auto material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
    auto material_left = make_shared<dielectric>(1.5);
    auto material_bubble = make_shared<dielectric>(1.0/1.5);
    auto material_right = make_shared<metal>(color(0.8, 0.6, 0.2), 0.0);
    auto material_cube = make_shared<metal>(color(0.8, 0.6, 0.2), 0.5);

    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100, material_ground));

    // world.add(make_shared<sphere>(point3(0.0, 0.0, -1.2), 0.5, material_center));
    // world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.2), 0.5, material_left));
    // world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.2), 0.4, material_bubble));
    // world.add(make_shared<sphere>(point3(1.0, 0.0, -1.2), 0.5, material_right));    

    // world.add(make_shared<sphere>(point3(-3, 3, 0), 1.5, material_center));

    vertex3 v1(point3(0, 0, 0), vec3(0, 0, 1));
    vertex3 v2(point3(0, 1, 0), vec3(0, 0, 1));
    vertex3 v3(point3(1, 0, 0), vec3(1, 0, 1));
    world.add(make_shared<triangle>(v1, v2, v3, material_center));

    camera cam(world);
    cam.aspect_ratio = 16.0/9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 20;

    cam.vfov = 20;
    cam.lookfrom = point3(20, 20, 20);
    cam.lookat = point3(0, 0, 0);
    cam.vup = vec3(0, 1, 0);
    
    cam.defocus_angle = -0.6;
    cam.focus_dist = 5*sqrt(3);

    cam.num_threads = 5;

    auto start = std::chrono::high_resolution_clock::now();

    cam.render(world);

    auto stop = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::clog << "Time taken to render with " << cam.num_threads << " threads - " << duration.count() << "ms\n";
}