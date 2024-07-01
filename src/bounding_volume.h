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

        bool doesRayIntersect(const ray& r) const {
            if (isPointInside(bottom_left, top_right, r.origin()))
                return true;
            
            
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

            std::clog << "Bottom left: " << bottom_left << " and Top Right: " << top_right << '\n';
        }

        bool splitBoundingVolume(BoundingVolume& left, BoundingVolume& right, int num_tries) {
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
            
            left.bottom_left = bottom_left;
            left.top_right = point3(best_plane_of_split, top_right.y(), top_right.z());

            right.bottom_left = point3(best_plane_of_split, bottom_left.y(), bottom_left.z());
            right.top_right = top_right;

            for (auto const& tri: contents){
                // if any vertex is inside, it counts
                if( isPointInside(left.bottom_left, left.top_right, (tri->v1).position) ||
                    isPointInside(left.bottom_left, left.top_right, (tri->v2).position) ||
                    isPointInside(left.bottom_left, left.top_right, (tri->v3).position)) {
                        left.contents.push_back(tri);
                    }

                if( isPointInside(right.bottom_left, right.top_right, (tri->v1).position) ||
                    isPointInside(right.bottom_left, right.top_right, (tri->v2).position) ||
                    isPointInside(right.bottom_left, right.top_right, (tri->v3).position)) {
                        right.contents.push_back(tri);
                    }   
            }

            contents.clear();

            return true;
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
            // Arbitrarily chosing a normal to the left
            vec3 normal = vec3(-1, 0, 0);
            for (auto const& tri: contents){
                // if any vertex is inside, it counts
                if( isPointInside(bottom_left, top_right, (tri->v1).position) ||
                    isPointInside(bottom_left, top_right, (tri->v2).position) ||
                    isPointInside(bottom_left, top_right, (tri->v3).position)) {
                        num_triangles++;
                    }
            }
            return num_triangles;
        }

        inline double surfaceArea(point3 bottom_left, point3 top_right) const {
            return (top_right.y() - bottom_left.y()) * (top_right.x() - bottom_left.x()) + (top_right.z() - bottom_left.z()) * (top_right.x() - bottom_left.x()) + (top_right.y() - bottom_left.y()) * (top_right.z() - bottom_left.z());
        }

        inline bool isPointInside(point3 bottom_left, point3 top_right, const point3 point) const {
            return 
                    bottom_left.x() <= point.x() && point.x() <= top_right.x() &&
                    bottom_left.y() <= point.y() && point.y() <= top_right.y() &&
                    bottom_left.z() <= point.z() && point.z() <= top_right.z();
        }
};