//
// Created by tomas on 3.9.22.
//


#pragma once
#include <orbital/Planet.h>
#include <orbital/SolarSystem.h>
#include <renderer/LightingModel.h>
#include <renderer/impl/PostProcessEffect.h>
#include <orbital/OrbitAnimation.h>

#include <assets/AssetLoader.h>
#include <effects/Bloom.h>

#include <memory>

namespace Galax::Generators{

    using namespace Galax::Renderer;
    using namespace Galax::Assets;
    using namespace Galax::Orbital;
    class SolarSystemLoader {
    public:
        typedef std::tuple<std::shared_ptr<SolarSystem>, std::shared_ptr<LightingModel>, std::vector<std::shared_ptr<IPostProcessEffect>>> RenderData;

        SolarSystemLoader() = default;
        ~SolarSystemLoader() = default;
        RenderData generateSystem();

    private:

        int planetCount = 0;

        std::shared_ptr<AssetLoader> assets;

        std::shared_ptr<Animation> generatePlanetSpin(int spinLength);


        std::shared_ptr<Node> generateSkybox(std::shared_ptr<AssetLoader> assets);


        std::shared_ptr<PostProcessEffect>
        generateHDR(std::shared_ptr<Texture> lightMap, std::shared_ptr<ITexture> bloomTexture);

        std::shared_ptr<Effects::Bloom>
        generateBloom(std::shared_ptr<AssetLoader> assets, std::shared_ptr<Texture> bloomMap);

        std::shared_ptr<OrbitAnimation> generateRotation(std::shared_ptr<Planet> planet, std::shared_ptr<IUniform> sunPosition);
    };
}

