#pragma once

#include "rtweekend.h"
#include "triangle.h"

const double t_traversal = 1;
const double t_intersect = 2;

class BoundingVolume {
    public:
        point3 bottom_left, top_right;
        std::vector<shared_ptr<triangle>> contents;

        BoundingVolume() {}
        BoundingVolume(point3 bottom_left, point3 top_right): bottom_left(bottom_left), top_right(top_right) {}

        bool does_ray_intersect(const ray& r, double& t) const {
            if (isPointInside(bottom_left, top_right, r.origin()))
                return true;

            // std::clog << "\nChecking all planes";
            
            if (r.origin().x() < bottom_left.x()){
                // std::clog << "\nChecking left plane";
                if (does_ray_intersect_plane(bottom_left, point3(bottom_left.x(), top_right.y(), top_right.z()), vec3(-1, 0, 0), r, t))
                    return true;
            }

            if (r.origin().x() > top_right.x()){
                // std::clog << "\nChecking right plane";
                if (does_ray_intersect_plane(point3(top_right.x(), bottom_left.y(), top_right.z()), point3(top_right.x(), top_right.y(), bottom_left.z()), vec3(1, 0, 0), r, t))
                    return true;
            }

            if(r.origin().y() < bottom_left.y()){
                // std::clog << "\nChecking bottom plane";
                if(does_ray_intersect_plane(point3(bottom_left.x(), bottom_left.y(), top_right.z()), point3(top_right.x(), bottom_left.y(), bottom_left.z()), vec3(0, -1, 0), r, t))
                    return true;
            }

            if(r.origin().y() > top_right.y()){
                // std::clog << "\nChecking top plane";
                if(does_ray_intersect_plane(point3(bottom_left.x(), top_right.y(), bottom_left.z()), top_right, vec3(0, 1, 0), r, t))
                    return true;
            }

            if(r.origin().z() < bottom_left.z()){
                // std::clog << "\nChecking back plane";
                if(does_ray_intersect_plane(point3(top_right.x(), bottom_left.y(), bottom_left.z()), point3(bottom_left.x(), top_right.y(), bottom_left.z()), vec3(0, 0, -1), r, t))
                    return true;
            }

            if(r.origin().z() > top_right.z()){
                // std::clog << "\nChecking front plane";
                if(does_ray_intersect_plane(point3(bottom_left.x(), bottom_left.y(), top_right.z()), top_right, vec3(0, 0, 1), r, t))
                    return true;
            }

            return false;
        }

        bool check_all_triangles(const ray& r, interval ray_t, hit_record& rec) const {
            hit_record temp_rec;
            bool hit_anything = false;
            auto closest_so_far = ray_t.max;

            for(const auto& object: contents){
                if(object->hit(r, interval(ray_t.min, closest_so_far), temp_rec)) {
                    hit_anything = true;
                    closest_so_far = temp_rec.t;
                    rec = temp_rec;
                }

                // std::clog << "Hit\n";
            }

            return hit_anything;
        }

        void set_up_corners(){
            std::clog << "Setting up corners\n";
            if (contents.size() < 1){
                return;
            }

            for(const auto& tri: contents){
                vertex3 v[3] = {tri->v1, tri->v2, tri->v3};
                for(const auto& vert: v){
                    bottom_left[0] = vert.position.x() < bottom_left[0] ? vert.position.x(): bottom_left[0];
                    bottom_left[1] = vert.position.y() < bottom_left[1] ? vert.position.y(): bottom_left[1];
                    bottom_left[2] = vert.position.z() < bottom_left[2] ? vert.position.z(): bottom_left[2];

                    top_right[0] = vert.position.x() > top_right[0] ? vert.position.x(): top_right[0];
                    top_right[1] = vert.position.y() > top_right[1] ? vert.position.y(): top_right[1];
                    top_right[2] = vert.position.z() > top_right[2] ? vert.position.z(): top_right[2];
                }
            }

            // bottom_left[2] -= 0.1;
            // top_right[2] += 0.1;

            std::clog << "Bottom left: " << bottom_left << " and Top Right: " << top_right << '\n';
        }

        bool splitBoundingVolume(BoundingVolume* left, BoundingVolume* right, int num_tries) {
            if (contents.size() <= 1)
                return false;

            double spacing = (top_right.x() - bottom_left.x())/(num_tries+1);
            double current_best_cost = infinity;
            double best_plane_of_split;

            for (int i = 1; i <= num_tries; i++)
            {
                double current_cost = costOfSplit(bottom_left.x() + i*spacing);
                if(current_cost < current_best_cost){
                    current_best_cost = current_cost;
                    best_plane_of_split = bottom_left.x() + i*spacing;
                }
            }

            // best_plane_of_split = (top_right.x() + bottom_left.x())/2.0;
            
            left->bottom_left = bottom_left;
            left->top_right = point3(best_plane_of_split, top_right.y(), top_right.z());

            right->bottom_left = point3(best_plane_of_split, bottom_left.y(), bottom_left.z());
            right->top_right = top_right;

            for (auto const& tri: contents){
                // if any vertex is inside, it counts
                bool in_left =  left->isTriangleInBV(left->bottom_left, left->top_right, *tri);
                    

                bool in_right = right->isTriangleInBV(right->bottom_left, right->top_right, *tri);

                if(in_left & in_right){
                    // std::clog << "\nTriangle is split. Left before: " << left.contents.size() << " Right before: " << right.contents.size();
                    left->contents.push_back(tri);
                    right->contents.push_back(tri);
                    // std::clog << "\nLeft after: " << left.contents.size() << " Right after: " << right.contents.size();
                    continue;
                }

                if (in_left)
                    left->contents.push_back(tri);
                
                if(in_right)
                    right->contents.push_back(tri);
            }

            contents.clear();

            return true;
        }

        private:
            //axis aligned plane
            bool does_ray_intersect_plane(point3 bottom_left, point3 top_right, vec3 normal, const ray& r, double& t_global) const {
                if (dot(r.direction(), normal) == 0){
                    // std::clog << "\nray is ortho to plane";
                    return false;
                }
                
                // std::clog << "\nCalc num and denom";
                double numerator = dot(normal, bottom_left - r.origin());
                double denominator = dot(r.direction(), normal);

                double t = numerator/denominator;

                point3 pos = r.at(t);

                // std::clog << "\nPoint is: " << pos;

                if (dot(r.direction(), normal) < 0 &&
                (bottom_left.x() - pos.x())*(top_right.x() - pos.x()) <= 0 && 
                (bottom_left.y() - pos.y())*(top_right.y() - pos.y()) <= 0 &&
                (bottom_left.z() - pos.z())*(top_right.z() - pos.z()) <= 0) {
                    t_global = t;
                    return true;
                }

                // std::clog << "\nReturn false";
                return false;
                 
            }

            double costOfSplit(double planeOfSplit) const {
                double S_total = surfaceArea(bottom_left, top_right);
                double S_left = surfaceArea(bottom_left, point3(planeOfSplit, top_right.y(), top_right.z()));
                double S_right = surfaceArea(point3(planeOfSplit, bottom_left.y(), bottom_left.z()), top_right);
                
                double P_left = S_left/S_total;
                double P_right = S_right/S_total;

                double cost_left = P_left*numTrianglesInBV(bottom_left, point3(planeOfSplit, top_right.y(), top_right.z())) * t_intersect;
                double cost_right = P_right*numTrianglesInBV(point3(planeOfSplit, bottom_left.y(), bottom_left.z()), top_right) * t_intersect;

                return t_traversal + cost_left + cost_right;
            }

            int numTrianglesInBV(point3 bottom_left, point3 top_right) const {
                int num_triangles = 0;
                BoundingVolume tempVolume(bottom_left, top_right);
                // Arbitrarily chosing a normal to the left
                vec3 normal = vec3(-1, 0, 0);
                for (auto const& tri: contents){
                    // if any vertex is inside, it counts
                    if(tempVolume.isTriangleInBV(bottom_left, top_right, *tri)) {
                            num_triangles++;
                        }
                }
                return num_triangles;
            }

            bool isTriangleInBV(point3 bottom_left, point3 top_right, triangle tri){
                point3 A = tri.v1.position, B = tri.v2.position, C = tri.v3.position;

                if(isPointInside(bottom_left, top_right, A) || isPointInside(bottom_left, top_right, B) || isPointInside(bottom_left, top_right, C))
                    return true;

                ray AB(A, B - A), BC(B, C - B), CA(C, A - C);

                double t_AB, t_BC, t_CA;
                bool ray_intersects_AB = does_ray_intersect(AB, t_AB);
                ray_intersects_AB = ray_intersects_AB && 0 <= t_AB && t_AB <= 1;
                bool ray_intersects_BC = does_ray_intersect(BC, t_BC);
                ray_intersects_BC = ray_intersects_BC && 0 <= t_BC && t_BC <= 1;
                bool ray_intersects_CA = does_ray_intersect(CA, t_CA);
                ray_intersects_CA = ray_intersects_CA && 0 <= t_CA && t_CA <= 1;

                return ray_intersects_AB || ray_intersects_BC || ray_intersects_CA;
            }

            inline double surfaceArea(point3 bottom_left, point3 top_right) const {
                return (top_right.y() - bottom_left.y()) * (top_right.x() - bottom_left.x()) + (top_right.z() - bottom_left.z()) * (top_right.x() - bottom_left.x()) + (top_right.y() - bottom_left.y()) * (top_right.z() - bottom_left.z());
            }

            inline bool isPointInside(point3 bottom_left, point3 top_right, const point3 pos) const {
            return 
                    (bottom_left.x() - pos.x())*(top_right.x() - pos.x()) <= 0 && 
                    (bottom_left.y() - pos.y())*(top_right.y() - pos.y()) <= 0 &&
                    (bottom_left.z() - pos.z())*(top_right.z() - pos.z()) <= 0;
        }
};