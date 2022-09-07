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
#include "JsonHelper.h"

namespace Galax::Assets {
    using nlohmann::json;

    struct ColorPalette {
        std::string name;
        std::vector<std::string> colors = {};
        double limit = 0.;
        double transition = 0.;
    };

    struct PlanetConfig {
        std::string name;
        std::vector<ColorPalette> color_palette;
    };
}


namespace nlohmann {
    void from_json(const json &j, Galax::Assets::ColorPalette &x);

    void to_json(json &j, const Galax::Assets::ColorPalette &x);

    void from_json(const json &j, Galax::Assets::PlanetConfig &x);

    void to_json(json &j, const Galax::Assets::PlanetConfig &x);

    inline void from_json(const json &j, Galax::Assets::ColorPalette &x) {
        x.name = j.at("name").get<std::string>();
        x.colors = j.at("colors").get<std::vector<std::string>>();
        x.limit = j.at("limit").get<double>();
        x.transition = j.at("transition").get<double>();
    }

    inline void to_json(json &j, const Galax::Assets::ColorPalette &x) {
        j = json::object();
        j["name"] = x.name;
        j["colors"] = x.colors;
        j["limit"] = x.limit;
        j["transition"] = x.transition;
    }

    inline void from_json(const json &j, Galax::Assets::PlanetConfig &x) {
        x.name = j.at("name").get<std::string>();
        x.color_palette = j.at("colorPalette").get<std::vector<Galax::Assets::ColorPalette>>();
    }

    inline void to_json(json &j, const Galax::Assets::PlanetConfig &x) {
        j = json::object();
        j["name"] = x.name;
        j["colorPalette"] = x.color_palette;
    }
}
