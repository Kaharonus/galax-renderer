//
// Created by tomas on 3.9.22.
//


#pragma once
#include "../orbital/Planet.h"
#include "../orbital/SolarSystem.h"
#include "../renderer/LightingModel.h"
#include "../renderer/PostProcessEffect.h"

#include "../assets/AssetLoader.h"

#include <memory>

namespace Galax::Generators{

    using namespace Galax::Renderer;
    using namespace Galax::Assets;
    using namespace Galax::Orbital;
    class SolarSystemLoader {
    public:
        typedef std::tuple<std::shared_ptr<SolarSystem>, std::shared_ptr<LightingModel>, std::vector<std::shared_ptr<PostProcessEffect>>> RenderData;

        SolarSystemLoader() = default;
        ~SolarSystemLoader() = default;
        RenderData generateSystem();

    private:

        int planetCount = 0;

        std::shared_ptr<AssetLoader> assets;

        std::shared_ptr<Animation> generatePlanetSpin(int spinLength);

        std::shared_ptr<Animation> generateRotation(float distance);

        std::shared_ptr<PostProcessEffect> generateHDR(std::shared_ptr<Texture> lightMap);

        std::shared_ptr<Node> generateSkybox(std::shared_ptr<AssetLoader> assets);

        std::shared_ptr<PostProcessEffect> generateBloom();

        std::shared_ptr<PostProcessEffect>
        generateHDR(std::shared_ptr<Texture> lightMap, std::shared_ptr<Texture> bloomTexture);

        std::shared_ptr<PostProcessEffect> generateBloom(std::shared_ptr<Texture> light);
    };
}

