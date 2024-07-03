#pragma once

#include "rtweekend.h"

#include <string>
#include <sstream>
#include <fstream>

class fileLoader {
    public:
        char* filename;

        fileLoader() {}
        fileLoader(char* filename): filename(filename) {}

        void fill_position_and_normals(){
            file = std::ifstream(filename);

            std::string line;

            if(file.is_open()) {
                while(std::getline(file, line)) {
                    if(line.rfind("v ", 0) != 0 || line.rfind("vn ", 0) != 0 || line.rfind("f", 0) != 0)
                        continue;

                    if(line.rfind("v ", 0) == 0 || line.rfind("vn ", 0) == 0){

                    }                        
                }
            }
        }
    
    private:
        std::ifstream file;
        std::vector<point3> vertex_positions;
        std::vector<vec3> vertex_normals;
        std::vector<shared_ptr<triangle>> triangles;

        std::vector<std::string> split(const std::string& str, char delimiter) {
            std::vector<std::string> tokens;
            std::stringstream ss(str);
            std::string token;

            while (std::getline(ss, token, delimiter)) {
                tokens.push_back(token);
            }

            return tokens;
        }

        void add_to_positions(std::string line) {
            std::vector<std::string> tokens = split(line, ' ');
            if(tokens.size() < 3)
                return;

            point3 position;

            for (size_t i = 1; i < tokens.size(); i++)
            {
                position[i-1] = std::atof(tokens.at(i).c_str());
            }
            
            vertex_positions.push_back(position);
        }

        void add_to_normals(std::string line) {
            std::vector<std::string> tokens = split(line, ' ');
            if(tokens.size() < 3)
                return;

            vec3 normal;

            for (size_t i = 1; i < tokens.size(); i++)
            {
                normal[i-1] = std::atof(tokens.at(i).c_str());
            }
            
            vertex_normals.push_back(normal);
        }
        
        void add_to_face(std::string line) {
            std::vector<std::string> tokens = split(line, ' ');
            if(tokens.size() < 3)
                return;
            
            std::vector<vertex3> vertices;
            for(const auto& token: tokens) {
                std::vector<std::string> indices = split(line, '/');
                int position_index = std::atoi(indices.at(0).c_str());
                int normal_index = std::atoi(indices.at(2).c_str());

                vertex3 v1(vertex_positions.at(position_index), vertex_normals.at(normal_index));
                vertices.push_back(v1);
            }

            
        }
};