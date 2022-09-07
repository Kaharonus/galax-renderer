//
// Created by tomas on 3.9.22.
//


#pragma once
#include "../orbital/Planet.h"
#include "../orbital/SolarSystem.h"

#include "../assets/AssetLoader.h"

#include <memory>

namespace Galax::Generators{

    using namespace Galax::Renderer;
    using namespace Galax::Assets;
    using namespace Galax::Orbital;

    class SolarSystemLoader {
    public:
        SolarSystemLoader() = default;
        ~SolarSystemLoader() = default;
        std::shared_ptr<SolarSystem> generateSystem();


    private:

        int planetCount = 0;

        std::shared_ptr<Camera> generateCamera(std::shared_ptr<AssetLoader> loader, float w, float h);

        std::map<float, std::shared_ptr<Mesh>> generateMesh(std::shared_ptr<AssetLoader> loader);

        std::shared_ptr<Program> generateProgram(std::shared_ptr<AssetLoader> loader);

        std::vector<std::shared_ptr<Animation>> generateAnimations(std::shared_ptr<AssetLoader> loader);

        std::shared_ptr<Animation> generateRotation(float distance);
    };
}

