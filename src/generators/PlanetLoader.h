//
// Created by tomas on 7.9.22.
//

#pragma once

#include <memory>
#include <unordered_map>
#include "../orbital/Planet.h"
#include "../assets/AssetLoader.h"
#include "../renderer/impl/Shader.h"
#include "../renderer/impl/Program.h"
#include "../renderer/impl/Texture.h"
#include "../assets/PlanetConfig.h"
#include "../renderer/impl/Mesh.h"

namespace Galax::Generators {

    using namespace Galax::Orbital;
    using namespace Galax::Assets;
    using namespace Galax::Renderer;
    class PlanetLoader {
    public:
        static std::shared_ptr<Planet> fromType(const std::string &name, Planet::Type type);

        static void init(const std::shared_ptr<AssetLoader> &loader);

    private:
        static bool initialized;
        static std::unordered_map<Planet::Type, std::shared_ptr<Program>> programs;
        static std::unordered_map<Planet::Type, std::shared_ptr<PlanetConfig>> configs;
        static std::unordered_map<Planet::Type, std::shared_ptr<Texture>> colorPalette;
        static std::map<float, std::shared_ptr<IMesh>> meshes;

        static glm::vec3 fromHex(const std::string &hex);

        static Planet::Type fromStr(const std::string &str);

        static void generatePalette(Planet::Type type);

        static void generateMesh(const std::shared_ptr<AssetLoader> &loader);

        static std::shared_ptr<Mesh> calculatePlanetMesh(const std::shared_ptr<Mesh> &orig, int i);


    };

}
