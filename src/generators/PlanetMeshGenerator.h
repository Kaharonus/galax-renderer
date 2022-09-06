//
// Created by tomas on 3.9.22.
//


#pragma once

#include <memory>
#include "../assets/AssetLoader.h"
#include "../renderer/scene_objects/Node.h"

namespace Galax::Generators {

    using namespace Galax::Assets;

    class PlanetMeshGenerator {
    public:
        explicit PlanetMeshGenerator(std::shared_ptr<AssetLoader> loader);

        std::vector<std::shared_ptr<Mesh>> getPlanetMesh(const std::string& planetName, const std::string& path = "models/planet", int levels = 6);

    private:
        std::shared_ptr<AssetLoader> loader;
    };

}

