#pragma once

#include "rtweekend.h"
#include "bounding_volume.h"

struct BVH_node {
    BoundingVolume volume;
    BVH_node* left;
    BVH_node* right;

    void split(int target_depth){
        if (target_depth < 1)
            return;

        bool split_possible = volume.splitBoundingVolume(left->volume, right->volume, 4);

        if (!split_possible)
            return;
        
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

        void assign_to_root_and_split(BoundingVolume root_volume) {
            root->volume = root_volume;
            root->left = nullptr;
            root->left = nullptr;

            root->split(depth);
        }

        void print_BVH_nodes(BVH_node* current_node, int depth){
            if(current_node == nullptr) {
                return;
            }
            std::cout << "Current node at depth" << depth << " has size: " << current_node->volume.contents.size() << std::endl;
            print_BVH_nodes(current_node->left, depth + 1);
            print_BVH_nodes(current_node->right, depth + 1);
        }
};