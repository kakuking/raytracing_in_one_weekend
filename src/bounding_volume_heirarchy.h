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
};

class Bounding_Volume_Heirarchy{
    public:
        int depth = 2;
        BVH_node* root;

        Bounding_Volume_Heirarchy(){}
        Bounding_Volume_Heirarchy(int depth): depth(depth) {}
        Bounding_Volume_Heirarchy(int depth, BoundingVolume root_volume): depth(depth) {
            assign_to_root_and_split(root_volume);
        }

        void assign_to_root_and_split(BoundingVolume& root_volume) {
            root = new BVH_node();
            root->volume = root_volume;
            root->left = nullptr;
            root->right = nullptr;

            root->split(depth);
        }

        void print_BVH_nodes(BVH_node* current_node, int depth){
            if(current_node == nullptr) {
                return;
            }
            std::clog << "\nCurrent node at depth " << depth << " has size: " << current_node->volume.contents.size() << " and corners " << current_node->volume.bottom_left << ", " << current_node->volume.top_right << "\n Contents: " ;
            for(auto const& tri: current_node->volume.contents){
                std::clog << *tri;
            }
            
            print_BVH_nodes(current_node->left, depth + 1);
            print_BVH_nodes(current_node->right, depth + 1);
        }
};