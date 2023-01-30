//
// Created by tomas on 31.8.22.
//

#include <assets/ObjReader.h>
#include <memory>
#include <thread>
#include <array>
#include <iostream>
#include <sstream>

using namespace Galax::Assets;

ObjReader::ObjReader(PHYSFS_File *file) {
    this->file = file;
}

glm::vec3 ObjReader::readV3(const std::string& vertex){
    std::stringstream ss(vertex);
    glm::vec3 v;
    ss >> v.x;
    ss >> v.y;
    ss >> v.z;
    return v;
};

glm::vec2 ObjReader::readV2(const std::string& vertex){
    std::stringstream ss(vertex);
    glm::vec2 v;
    ss >> v.x;
    ss >> v.y;
    return v;
};




void ObjReader::parseLine(const std::string& line){
    switch(line.at(0)){
        case 'v':
            if(line.at(1) == ' ') {
                vertices.push_back(readV3(line.substr(2)));
            }
            if(line.at(1) == 'n') {
                normals.push_back(readV3(line.substr(3)));
            }
            if(line.at(1) == 't') {
                texCoords.push_back(readV2(line.substr(3)));
            }
            break;
        case 'f':{
            std::stringstream lineStream(line.substr(2));
            std::string point;
            //Split by space (x/x/x y/y/y z/z/z w/w/w)
            while(std::getline(lineStream, point, ' ')) {
                std::stringstream pointStream(point);
                std::string index;
                int i = 0;
                //Split by slash (x/x/x)
                while(std::getline(pointStream, index, '/')) {
                    if(i == 0) {
                        vertexIndices.push_back(std::stoi(index) - 1);
                    }
                    if(i == 1) {
                        texCoordIndices.push_back(std::stoi(index) - 1);
                    }
                    if(i == 2) {
                        normalIndices.push_back(std::stoi(index) - 1);
                    }
                    i++;
                }
            }
        }
        break;
        case 'o':
            name = line.substr(2);
            break;
        default:
            break;
    }
}

bool ObjReader::load() {
    auto size = (size_t)PHYSFS_fileLength(file);
    std::string buffer(size, '\0');
    PHYSFS_readBytes(file, buffer.data(), size);
    std::stringstream ss(buffer);
    std::string line;
    const auto threadCount = std::thread::hardware_concurrency();

    std::vector<std::thread> threads;
    int i = 0;
    while(std::getline(ss, line)) {
        if(line.empty() || line[0] == '#') {
            continue;
        }
        parseLine(line);
    }
    return true;
}

ObjReader::~ObjReader() = default;
