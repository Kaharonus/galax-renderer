//
// Created by tomas on 7.9.22.
//

#pragma once

#include <memory>
#include <unordered_map>
#include "../orbital/Planet.h"
#include "../assets/AssetLoader.h"
#include "../renderer/scene_objects/Shader.h"
#include "../renderer/scene_objects/Program.h"
#include "../renderer/scene_objects/Texture.h"
#include "../assets/PlanetConfig.h"
#include "../renderer/scene_objects/Mesh.h"

namespace Galax::Generators {

    using namespace Galax::Orbital;
    using namespace Galax::Assets;

    class PlanetLoader {
    public:
        static std::shared_ptr<Planet> fromType(const std::string &name, Planet::Type type);

        static void init(const std::shared_ptr<AssetLoader> &loader);

    private:
        static bool initialized;
        static std::unordered_map<Planet::Type, std::shared_ptr<Program>> programs;
        static std::unordered_map<Planet::Type, std::shared_ptr<PlanetConfig>> configs;
        static std::unordered_map<Planet::Type, std::shared_ptr<Texture>> colorPalette;
        static std::map<float, std::shared_ptr<Mesh>> meshes;

        static glm::vec3 fromHex(const std::string &hex);

        static Planet::Type fromStr(const std::string &str);

        static void generatePalette(Planet::Type type);

        static void generateMesh(const std::shared_ptr<AssetLoader> &loader);

        static std::shared_ptr<Mesh> calculatePlanetMesh(const std::shared_ptr<Mesh> &orig);


    };

}
