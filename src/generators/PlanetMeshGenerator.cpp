//
// Created by tomas on 3.9.22.
//

#include "PlanetMeshGenerator.h"

#include <utility>
#include <unordered_map>
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>

using namespace Galax::Generators;

PlanetMeshGenerator::PlanetMeshGenerator(std::shared_ptr<AssetLoader> loader) : loader(std::move(loader)) {

}


std::shared_ptr<Mesh> calculatePlanet(const std::shared_ptr<Mesh> &orig) {
    auto vertices = orig->getVertices();
    auto indices = orig->getIndices();
    auto stride = orig->getStride();
    auto normals = std::unordered_map<glm::vec3, std::vector<glm::vec3>>();
    auto normalsResult = std::unordered_map<glm::vec3, glm::vec3>();
    for (int i = 0; i < vertices.size(); i += stride) {
        glm::vec3 pos = glm::vec3(vertices[i], vertices[i + 1], vertices[i + 2]);
        glm::vec3 normal = glm::vec3(vertices[i + 3], vertices[i + 4], vertices[i + 5]);
        normals[pos].push_back(normal);
    }
    for (auto &normal : normals) {
        auto sum = glm::vec3(0);
        for (auto &n : normal.second) {
            sum += n;
        }
        normalsResult[normal.first] = glm::normalize(sum);
    }
    auto result = std::make_shared<Mesh>();
    for (int i = 0; i < vertices.size(); i += stride) {
        glm::vec3 pos = glm::vec3(vertices[i], vertices[i + 1], vertices[i + 2]);
        glm::vec3 normal = glm::vec3(vertices[i + 3], vertices[i + 4], vertices[i + 5]);
        glm::vec2 texCoord = glm::vec2(vertices[i + 6], vertices[i + 7]);
        glm::vec3 tangent = normalsResult[pos];
        result->addVertex(pos, normal, texCoord, tangent);
    }
    for(unsigned int index : indices) {
        result->addIndex(index);
    }

    return result;

}

std::vector<std::shared_ptr<Mesh>>
PlanetMeshGenerator::getPlanetMesh(const std::string &planetName, const std::string &path, int levels) {
    auto result = std::vector<std::shared_ptr<Mesh>>();
    auto meshes = loader->getMesh(path, levels, planetName);
    for (const auto &mesh: meshes) {
        auto planet = calculatePlanet(mesh);
        result.push_back(planet);
    }
    return result;

}