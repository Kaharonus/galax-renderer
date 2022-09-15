//
// Created by tomas on 3.9.22.
//


#pragma once
#include "../orbital/Planet.h"
#include "../orbital/SolarSystem.h"
#include "../renderer/LightingModel.h"

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
        std::tuple<std::shared_ptr<SolarSystem>, std::shared_ptr<LightingModel>> generateSystem();

    private:

        int planetCount = 0;

        std::shared_ptr<Animation> generatePlanetSpin(int spinLength);

        std::shared_ptr<Animation> generateRotation(float distance);
    };
}

