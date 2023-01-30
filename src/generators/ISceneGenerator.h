//
// Created by tomas on 7.12.22.
//


#pragma once
#include <renderer/Scene.h>

#include <any>
#include <assets/SceneInfo.h>
#include <renderer/interfaces/IPostProcessEffect.h>
#include <assets/AssetLoader.h>

namespace Galax::Generators{
	class ISceneGenerator {
	public:

		struct SceneData{
			std::shared_ptr<Galax::Renderer::Scene> scene;
			std::vector<std::shared_ptr<Galax::Renderer::IPostProcessEffect>> postProcessEffects;
		};

		virtual std::shared_ptr<SceneData> generate() = 0;
		virtual void setConfig(const std::shared_ptr<Assets::SceneInfo::SceneConfig> &config) = 0;
		virtual void setAssets(const std::shared_ptr<Assets::AssetLoader> &assets) = 0;
		virtual std::string getName() = 0;
	};

}
