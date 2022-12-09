//
// Created by tomas on 7.12.22.
//


#pragma once

#include <string>
#include <unordered_map>
#include <any>

namespace Galax::Assets{

	using nlohmann::json;

	class SceneInfo {
	public:
		typedef std::unordered_map<std::string, std::any> SceneConfig;
		std::string name;
		std::string description;
		std::string path;
		std::string generatorName;
		std::shared_ptr<SceneConfig> scene;
	};

}

namespace nlohmann {
	void from_json(const json & j, Galax::Assets::SceneInfo::SceneConfig & x);
	void to_json(json & j, const Galax::Assets::SceneInfo::SceneConfig  & x);

	void from_json(const json & j, Galax::Assets::SceneInfo & x);
	void to_json(json & j, const Galax::Assets::SceneInfo & x);

	inline void from_json(const json & j, std::shared_ptr<Galax::Assets::SceneInfo::SceneConfig> & x) {
		x = std::make_shared<Galax::Assets::SceneInfo::SceneConfig>();
		for (json::const_iterator it = j.begin(); it != j.end(); ++it) {
			(*x)[it.key()] = it.value();
		}
	}


	inline void from_json(const json & j, Galax::Assets::SceneInfo& x) {
		x.name = j.at("name").get<std::string>();
		x.description = j.at("description").get<std::string>();
		x.generatorName = j.at("generator").get<std::string>();
		x.scene = j.at("scene").get<std::shared_ptr<Galax::Assets::SceneInfo::SceneConfig>>();
	}

	inline void to_json(json & j, const Galax::Assets::SceneInfo & x) {
		j = json::object();
		j["name"] = x.name;
		j["description"] = x.description;
		j["generator"] = x.generatorName;
	}
}
