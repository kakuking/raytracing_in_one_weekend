#include "rtweekend.h"
#include "camera.h"
#include "hittable_list.h"
#include "sphere.h"
#include "mesh.h"
#include "material.h"

#include <chrono>

int main(){
    // World
    hittable_list world;

    // auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    // world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

    // for (int a = -11; a < 11; a++) {
    //     for (int b = -11; b < 11; b++) {
    //         auto choose_mat = random_double();
    //         point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

    //         if ((center - point3(4, 0.2, 0)).length() > 0.9) {
    //             shared_ptr<material> sphere_material;

    //             if (choose_mat < 0.8) {
    //                 // diffuse
    //                 auto albedo = color::random() * color::random();
    //                 sphere_material = make_shared<lambertian>(albedo);
    //                 world.add(make_shared<sphere>(center, 0.2, sphere_material));
    //             } else if (choose_mat < 0.95) {
    //                 // metal
    //                 auto albedo = color::random(0.5, 1);
    //                 auto fuzz = random_double(0, 0.5);
    //                 sphere_material = make_shared<metal>(albedo, fuzz);
    //                 world.add(make_shared<sphere>(center, 0.2, sphere_material));
    //             } else {
    //                 // glass
    //                 sphere_material = make_shared<dielectric>(1.5);
    //                 world.add(make_shared<sphere>(center, 0.2, sphere_material));
    //             }
    //         }
    //     }
    // }

    // auto material1 = make_shared<dielectric>(1.5);
    // world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    // auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    // world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    // auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    // world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    auto material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
    auto material_left = make_shared<dielectric>(1.5);
    auto material_bubble = make_shared<dielectric>(1.0/1.5);
    auto material_right = make_shared<metal>(color(0.8, 0.6, 0.2), 1.0);

    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100, material_ground));

    // world.add(make_shared<sphere>(point3(0.0, 0.0, -1.2), 0.5, material_center));

    // world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.2), 0.5, material_left));
    // world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.2), 0.4, material_bubble));

    // world.add(make_shared<sphere>(point3(1.0, 0.0, -1.2), 0.5, material_right));    

    point3 p1(0.0, 0.0, -1.2), p2(1.0, 0.0, -1.2), p3(1.0, 1.0, -1.2);
    vec3 n1(-13, -2, -3), n2(-13, -2, -3), n3(-13, -2, -3);
    vertex3 v1(p1, n1), v2(p2, n2), v3(p3, n3);
    triangle tri1(v1, v2, v3);
    std::vector<triangle> tris;
    tris.push_back(tri1);

    world.add(make_shared<mesh>(tris, material_center));

    camera cam(world);
    cam.aspect_ratio = 16.0/9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 20;

    cam.vfov = 20;
    cam.lookfrom = point3(13, 2, 3);
    cam.lookat = point3(0, 0, 0);
    cam.vup = vec3(0, 1, 0);
    
    cam.defocus_angle = 0.6;
    cam.focus_dist = 10.0;

    cam.num_threads = 5;

    auto start = std::chrono::high_resolution_clock::now();

    cam.render(world);

    auto stop = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::clog << "Time taken to render with " << cam.num_threads << " threads - " << duration.count() << "ms\n";
}