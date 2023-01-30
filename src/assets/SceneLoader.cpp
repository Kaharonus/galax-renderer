//
// Created by tomas on 7.12.22.
//

#include <assets/SceneLoader.h>
using namespace Galax::Assets;
using namespace Galax::Renderer;

SceneLoader::SceneLoader(const std::string &assetFile, const std::string &sceneDir) {
	this->loader = std::make_shared<AssetLoader>();
	auto result = this->loader->load(assetFile);
	if (!result) {
		throw std::runtime_error("Failed to load asset file");
	}
	this->assetFile = assetFile;
	this->sceneDir = sceneDir;

	auto sceneFiles = this->loader->getFiles(sceneDir);
	for (auto& file : sceneFiles) {
		auto path = sceneDir + "/" + file;
		auto scene = this->loader->getJson<SceneInfo>(path);
		scene->path = path;
		this->scenes[scene->path] = scene;
	}

}

std::shared_ptr<Galax::Generators::ISceneGenerator::SceneData> SceneLoader::loadScene(const std::string &path) {
	auto scene = this->scenes[path];
	if (scene == nullptr) {
		throw std::runtime_error("Scene not found");
	}
	auto generator = this->generators[scene];
	if (generator == nullptr) {
		throw std::runtime_error("Generator not found");
	}
	return generator->generate();
}

std::vector<std::shared_ptr<SceneInfo>> SceneLoader::getScenes() {
	std::vector<std::shared_ptr<SceneInfo>> result;
	for (auto& [key, value] : this->scenes) {
		result.push_back(value);
	}
	return result;
}

void SceneLoader::registerGenerator(std::shared_ptr<Generators::ISceneGenerator> generator) {
//Match a generator to a scene based on the name
	for (auto& scene : this->scenes) {
		if (scene.second->generatorName == generator->getName()) {
			generator->setConfig(scene.second->scene);
			generator->setAssets(this->loader);
			this->generators[scene.second] = generator;
		}
	}
}
