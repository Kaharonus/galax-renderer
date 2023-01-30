//
// Created by tomas on 7.12.22.
//


#pragma once

#include <string>
#include <vector>
#include <renderer/Scene.h>
#include <assets/AssetLoader.h>
#include <assets/SceneInfo.h>
#include <generators/ISceneGenerator.h>

namespace Galax::Assets{
	class SceneLoader {
	public:
		SceneLoader(const std::string& assetFile, const std::string& sceneDir);

		std::vector<std::shared_ptr<SceneInfo>> getScenes();

		std::shared_ptr<Generators::ISceneGenerator::SceneData> loadScene(const std::string& path);
		void registerGenerator(std::shared_ptr<Generators::ISceneGenerator> generator);

	private:
		std::string assetFile;
		std::string sceneDir;

		std::shared_ptr<AssetLoader> loader;

		std::unordered_map<std::string, std::shared_ptr<SceneInfo>> scenes;
		std::unordered_map<std::shared_ptr<SceneInfo>, std::shared_ptr<Galax::Generators::ISceneGenerator>> generators;

	};
}
