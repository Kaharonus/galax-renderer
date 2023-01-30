//
// Created by tomas on 31.8.22.
//


#pragma once

#include <string>
#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <physfs.h>


namespace Galax::Assets {
        class ObjReader {
        public:
            ObjReader(PHYSFS_File* file);
            bool load();
            ~ObjReader();

            std::vector<glm::vec3> vertices;
            std::vector<glm::vec3> normals;
            std::vector<glm::vec2> texCoords;
            std::vector<unsigned int> vertexIndices;
            std::vector<unsigned int> normalIndices;
            std::vector<unsigned int> texCoordIndices;
            std::string name;
        private:
            PHYSFS_File *file;

            void parseLine(const std::string &source);


            static glm::vec3 readV3(const std::string &vertex);

            static glm::vec2 readV2(const std::string &vertex);
        };
} // Assets
