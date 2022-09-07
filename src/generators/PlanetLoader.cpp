//
// Created by tomas on 7.9.22.
//

#include "PlanetLoader.h"
#include <memory>
#include <map>
#include <unordered_map>
#include <glm/gtx/hash.hpp>

using namespace Galax::Orbital;

using namespace Galax::Generators;


bool PlanetLoader::initialized = false;
std::unordered_map<Planet::Type, std::shared_ptr<Program>> PlanetLoader::programs;
std::unordered_map<Planet::Type, std::shared_ptr<PlanetConfig>> PlanetLoader::configs;
std::unordered_map<Planet::Type, std::shared_ptr<Texture>> PlanetLoader::colorPalette;
std::map<float, std::shared_ptr<Mesh>> PlanetLoader::meshes;


std::shared_ptr<Planet> PlanetLoader::fromType(const std::string &name, Galax::Orbital::Planet::Type type) {
    if (!initialized) {
        assert(!initialized);
    }
    auto planet = std::make_shared<Planet>(name, type);
    planet->setProgram(programs[type]);
    planet->setMeshWithLOD(meshes);
    planet->addTexture(colorPalette[type]);
    return planet;

}


Planet::Type PlanetLoader::fromStr(const std::string &str) {
    if (str == "rocky") return Planet::Type::ROCKY;
    if (str == "gas_giant") return Planet::Type::GAS_GIANT;
    if (str == "ice") return Planet::Type::ICE;
    if (str == "hot") return Planet::Type::HOT;
    if (str == "temperate") return Planet::Type::TEMPERATE;
    if (str == "ocean") return Planet::Type::OCEAN;
    return Planet::Type::TEMPERATE;
}


void PlanetLoader::init(const std::shared_ptr<AssetLoader>& loader) {
    if (initialized) return;
    initialized = true;

    std::vector<std::string> types = {"temperate"};

    for (auto &typeName: types) {
        //Load the programs
        auto type = fromStr(typeName);
        auto frag = loader->getShader("shaders/planets/" + typeName + ".fs.shader", Shader::Type::FRAGMENT);
        auto vert = loader->getShader("shaders/planets/" + typeName + ".vs.shader", Shader::Type::VERTEX);
        auto program = std::make_shared<Program>(typeName + " prog", vert, frag);
        programs[type] = program;

        //Load planet configs
        auto config = loader->getJson<Assets::PlanetConfig>("planets/" + typeName + ".json");
        configs[type] = config;

        //Generate color palettes
        generatePalette(type);
    }

    //Init the planet mesh generator
    generateMesh(loader);
}


void PlanetLoader::generatePalette(Planet::Type type) {
    int resolution = 32;

    auto config = configs[type];

    auto palette = std::make_shared<Texture>("palette");
    palette->setWrap(Texture::CLAMP_TO_EDGE);
    palette->setFormat(Texture::RGB);
    palette->setDimensions(resolution, resolution);

    std::sort(config->color_palette.begin(), config->color_palette.end(),
              [](const ColorPalette &a, const ColorPalette &b) {
                  return a.limit < b.limit;
              });
    auto max = config->color_palette.back().limit;
    auto min = config->color_palette.front().limit;
    std::map<float, std::pair<double, std::vector<glm::vec3>>> paletteMap;
    for (auto &color: config->color_palette) {
        auto normalised = (float) ((color.limit - min) / (max - min));
        std::vector<glm::vec3> columnColors;
        auto count = (int) color.colors.size();
        for (int i = 0; i < resolution; i++) {
            auto progress = (float) i / (float) resolution;
            auto current = (int) (progress * count);
            auto next = current + 1;
            if (next >= count) next = count - 1;
            auto currentColor = fromHex(color.colors[current]);
            auto nextColor = fromHex(color.colors[next]);
            auto resultColor = glm::mix(currentColor, nextColor, progress * count - current);
            columnColors.push_back(resultColor);
        }
        paletteMap[normalised] = {color.transition, columnColors};
    }
    auto data = std::vector<unsigned char>(resolution * resolution * 3);

    //TODO find bug here, since this is highly fucked
    for (int y = 0; y < resolution; y++) {
        //Interpolate the column
        float value = (float) y / (float) resolution;
        auto color = paletteMap.lower_bound(value);
        auto next = paletteMap.upper_bound(value);
        for (int x = 0; x < resolution; x++) {
            auto index = (y * resolution + x) * 3;
            auto current = color->second.second[x];
            auto nextColor = next->second.second[x];
            //Interpolate only if the transition is not 0 and the progress is greater than the transition
            auto interpolator = (x - color->first) / (next->first - color->first);
            auto mixPercentage = color->second.first;
            glm::vec3 result = current;
            if(interpolator > mixPercentage){
                result = glm::mix(current, nextColor, interpolator);
            }
            data[index] = (unsigned char) (result.r * 255);
            data[index + 1] = (unsigned char) (result.g * 255);
            data[index + 2] = (unsigned char) (result.b * 255);
        }
    }
    palette->setData(data);
    colorPalette[type] = palette;
}

glm::vec3 PlanetLoader::fromHex(const std::string &hex) {
    if (hex[0] == '#') {
        return {
                (float) std::stoi(hex.substr(1, 2), nullptr, 16) / 255.0f,
                (float) std::stoi(hex.substr(3, 2), nullptr, 16) / 255.0f,
                (float) std::stoi(hex.substr(5, 2), nullptr, 16) / 255.0f
        };
    }
    return {0, 0, 0};
}

void PlanetLoader::generateMesh(const std::shared_ptr<AssetLoader>& loader){
    auto result = std::vector<std::shared_ptr<Mesh>>();
    std::string basePath = "models/planet/";
    auto levels = 1;
    for(int i = 0; i < levels; i++) {
        auto path = basePath + "lod" + std::to_string(i) + ".obj";
        auto mesh = loader->getMesh(path);
        auto planetMesh = calculatePlanetMesh(mesh);
        auto distance = i * 10.f;
        meshes[distance] = planetMesh;
    }
}

std::shared_ptr<Mesh> PlanetLoader::calculatePlanetMesh(const std::shared_ptr<Mesh>& orig){
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
    auto result = std::make_shared<Mesh>("Planet mesh");
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

