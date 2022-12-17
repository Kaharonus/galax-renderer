//  To parse this JSON data, first install
//
//      Boost     http://www.boost.org
//      json.hpp  https://github.com/nlohmann/json
//
//  Then include this file, and then do
//
//     PlanetConfig data = nlohmann::json::parse(jsonString);

#pragma once

#include <nlohmann/json.hpp>
#include <assets/JsonHelper.h>

namespace Galax::Assets {
	using nlohmann::json;

	struct AtmosphereConfig {
		std::vector<int64_t> color = {};
		double radius;
		double density;
	};

	struct Color {
		std::string color;
		double endPoint;
	};

	struct ColorPalette {
		std::string name;
		double heightStart;
		int64_t transition;
		std::vector<Color> colors;
	};

	struct NoiseSetting {
		double roughness;
		double strength;
	};

	struct PlanetConfig {
		bool useVoronoi;
		std::string name;
		double clampUnder;
		int minMoons;
		int maxMoons;
		float minSize;
		float maxSize;
		std::vector<NoiseSetting> noiseSettings;
		std::vector<ColorPalette> colorPalette;
		AtmosphereConfig atmosphere;
	};
}


namespace nlohmann {
	void from_json(const json &j, Galax::Assets::AtmosphereConfig &x);

	void to_json(json &j, const Galax::Assets::AtmosphereConfig &x);

	void from_json(const json &j, Galax::Assets::Color &x);

	void to_json(json &j, const Galax::Assets::Color &x);

	void from_json(const json &j, Galax::Assets::ColorPalette &x);

	void to_json(json &j, const Galax::Assets::ColorPalette &x);

	void from_json(const json &j, Galax::Assets::NoiseSetting &x);

	void to_json(json &j, const Galax::Assets::NoiseSetting &x);

	void from_json(const json &j, Galax::Assets::PlanetConfig &x);

	void to_json(json &j, const Galax::Assets::PlanetConfig &x);

	inline void from_json(const json &j, Galax::Assets::AtmosphereConfig &x) {
		x.color = j.at("color").get<std::vector<int64_t>>();
		x.radius = j.at("radius").get<double>();
		x.density = j.at("density").get<double>();
	}

	inline void to_json(json &j, const Galax::Assets::AtmosphereConfig &x) {
		j = json::object();
		j["color"] = x.color;
		j["radius"] = x.radius;
		j["density"] = x.density;
	}

	inline void from_json(const json &j, Galax::Assets::Color &x) {
		x.color = j.at("color").get<std::string>();
		x.endPoint = j.at("endPoint").get<double>();
	}

	inline void to_json(json &j, const Galax::Assets::Color &x) {
		j = json::object();
		j["color"] = x.color;
		j["endPoint"] = x.endPoint;
	}

	inline void from_json(const json &j, Galax::Assets::ColorPalette &x) {
		x.name = j.at("name").get<std::string>();
		x.heightStart = j.at("heightStart").get<double>();
		x.transition = j.at("transition").get<int64_t>();
		x.colors = j.at("colors").get<std::vector<Galax::Assets::Color>>();
	}

	inline void to_json(json &j, const Galax::Assets::ColorPalette &x) {
		j = json::object();
		j["name"] = x.name;
		j["heightStart"] = x.heightStart;
		j["transition"] = x.transition;
		j["colors"] = x.colors;
	}

	inline void from_json(const json &j, Galax::Assets::NoiseSetting &x) {
		x.roughness = j.at("roughness").get<double>();
		x.strength = j.at("strength").get<double>();
	}

	inline void to_json(json &j, const Galax::Assets::NoiseSetting &x) {
		j = json::object();
		j["roughness"] = x.roughness;
		j["strength"] = x.strength;
	}

	inline void from_json(const json &j, Galax::Assets::PlanetConfig &x) {
		x.name = j.at("name").get<std::string>();
		x.clampUnder = j.contains("clampUnder") ? j.at("clampUnder").get<double>() : 0;
		x.noiseSettings = j.at("noiseSettings").get<std::vector<Galax::Assets::NoiseSetting>>();
		x.colorPalette = j.contains("colorPalette") ?
				j.at("colorPalette").get<std::vector<Galax::Assets::ColorPalette>>() : std::vector<Galax::Assets::ColorPalette>();
		x.atmosphere = j.contains("atmosphere") ?
				j.at("atmosphere").get<Galax::Assets::AtmosphereConfig>() : Galax::Assets::AtmosphereConfig();
		x.minMoons = j.contains("minMoons") ? j.at("minMoons").get<int>() : 0;
		x.maxMoons = j.contains("maxMoons") ? j.at("maxMoons").get<int>() : 0;

		x.minSize = j.at("minSize").get<float>();
		x.maxSize =  j.at("maxSize").get<float>();
	}

	inline void to_json(json &j, const Galax::Assets::PlanetConfig &x) {
		j = json::object();
		j["name"] = x.name;
		j["clampUnder"] = x.clampUnder;
		j["noiseSettings"] = x.noiseSettings;
		j["colorPalette"] = x.colorPalette;
		j["atmosphere"] = x.atmosphere;
	}
}
