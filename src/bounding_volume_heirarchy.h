#pragma once

#include "rtweekend.h"
#include "bounding_volume.h"
#include "hittable.h"

struct BVH_node {
    BoundingVolume volume;
    BVH_node* left;
    BVH_node* right;

    void split(int target_depth){
        if (target_depth < 1)
            return;

        left = new BVH_node();
        right = new BVH_node();
        bool split_possible = volume.splitBoundingVolume(left->volume, right->volume, 4);

        if (!split_possible){
            delete left;
            left = nullptr;
            delete right;
            right = nullptr;
            return;
        }
        
        left->split(target_depth-1);
        right->split(target_depth-1);

        return;
    }

    bool search_through(const ray& r, interval ray_t, hit_record& rec){
        double t;
        if(!volume.does_ray_intersect(r, t))
            return false;

        if (left == nullptr && right == nullptr) {
            return volume.check_all_triangles(r, ray_t, rec);
        }

        double left_t = infinity, right_t = infinity;
        bool hits_left = false, hits_right = false;

        if (left != nullptr) {
            hits_left = left->volume.does_ray_intersect(r, left_t);
        }            

        if (right != nullptr) {
            hits_right = right->volume.does_ray_intersect(r, right_t);
        }

        if(hits_left && hits_right){
            if (left_t < right_t){
                return left->search_through(r, ray_t, rec);
            } else if (right_t < left_t){
                return right->search_through(r, ray_t, rec);
            } else {
                return left->search_through(r, ray_t, rec);
                return right->search_through(r, ray_t, rec);
            }
        }

        if(hits_left){
            return left->search_through(r, ray_t, rec);
        }

        if(hits_right){
            return right->search_through(r, ray_t, rec);
        }

        return false;
    }
};

class Bounding_Volume_Heirarchy: public hittable{
    public:
        shared_ptr<material> mat;
        int depth = 2;
        BVH_node* root;

        Bounding_Volume_Heirarchy(){}
        Bounding_Volume_Heirarchy(int depth): depth(depth) {}
        Bounding_Volume_Heirarchy(int depth, BoundingVolume root_volume): depth(depth) {
            assign_to_root_and_split(root_volume);
        }

        bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
            return root->search_through(r, ray_t, rec);
        }

        void assign_to_root_and_split(BoundingVolume& root_volume) {
            root = new BVH_node();
            root->volume = root_volume;
            root->left = nullptr;
            root->right = nullptr;

            root->split(depth);
        }

        void print(){
            print_BVH_nodes(root, 0);
        }

        void print_BVH_nodes(BVH_node* current_node, int depth){
            if(current_node == nullptr) {
                return;
            }
            std::clog << "\nCurrent node at depth " << depth << " has size: " << current_node->volume.contents.size() << " and corners " << current_node->volume.bottom_left << ", " << current_node->volume.top_right << "\n"; //Contents: " ;
            // for(auto const& tri: current_node->volume.contents){
            //     std::clog << *tri;
            // }
            
            print_BVH_nodes(current_node->left, depth + 1);
            print_BVH_nodes(current_node->right, depth + 1);
        }
};