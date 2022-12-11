//
// Created by tomas on 3.9.22.
//


#pragma once
#include <orbital/Planet.h>
#include <renderer/LightingModel.h>
#include <renderer/impl/PostProcessEffect.h>
#include <orbital/OrbitAnimation.h>
#include <orbital/SpaceCamera.h>
#include <orbital/SolarSystem.h>

#include <assets/AssetLoader.h>
#include <effects/Bloom.h>
#include <effects/Outline.h>

#include <memory>
#include <generators/ISceneGenerator.h>

namespace Galax::Generators{

    using namespace Galax::Renderer;
    using namespace Galax::Assets;
    using namespace Galax::Orbital;
    class SolarSystemGenerator : public ISceneGenerator {
    public:

        SolarSystemGenerator() = default;
        ~SolarSystemGenerator() = default;
		std::shared_ptr<SceneData> generate() override;

		void setConfig(const std::shared_ptr<Assets::SceneInfo::SceneConfig> &config) override;

		void setAssets(const std::shared_ptr<Assets::AssetLoader> &assets) override;

		std::string getName() override;

	private:

        std::shared_ptr<AssetLoader> assets;
		std::shared_ptr<Assets::SceneInfo::SceneConfig> config;

		std::shared_ptr<SolarSystem> scene;

		void addCamera();

		void addLighting();
		std::shared_ptr<LightingModel> lighting;
		std::shared_ptr<Texture> lightTexture;
		std::shared_ptr<Texture> bloomTexture;

		void addPlanets();
		std::shared_ptr<SpaceCamera> camera;

		void addSun();
		std::shared_ptr<Node> sun;

		void addSkyBox();


		void addPostProcessing();
		std::vector<std::shared_ptr<IPostProcessEffect>> postProcessEffects;

		void addBloom();
		std::shared_ptr<Texture> bloomResult;

		void addOutline();
		std::shared_ptr<Texture> outlineResult;

		void addFXAA();
		std::shared_ptr<Texture> fxaaResult;

		void addHDR();
	};
}

