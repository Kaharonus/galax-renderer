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

    struct Color {
        std::string color;
        double endPoint = 0;
    };

    struct ColorPalette {
        std::string name;
        double heightStart = 0;
        double transition = 0;
        std::vector<Color> colors;
    };

    struct PlanetConfig {
        std::string name;
        std::vector<ColorPalette> colorPalette;
    };
}


namespace nlohmann {
    void from_json(const json & j, Galax::Assets::Color & x);
    void to_json(json & j, const Galax::Assets::Color & x);

    void from_json(const json & j, Galax::Assets::ColorPalette & x);
    void to_json(json & j, const Galax::Assets::ColorPalette & x);

    void from_json(const json & j, Galax::Assets::PlanetConfig & x);
    void to_json(json & j, const Galax::Assets::PlanetConfig & x);

    inline void from_json(const json & j, Galax::Assets::Color& x) {
        x.color = j.at("color").get<std::string>();
        x.endPoint = j.at("endPoint").get<double>();
    }

    inline void to_json(json & j, const Galax::Assets::Color & x) {
        j = json::object();
        j["color"] = x.color;
        j["endPoint"] = x.endPoint;
    }

    inline void from_json(const json & j, Galax::Assets::ColorPalette& x) {
        x.name = j.at("name").get<std::string>();
        x.heightStart = j.at("heightStart").get<double>();
        x.transition = j.at("transition").get<double>();
        x.colors = j.at("colors").get<std::vector<Galax::Assets::Color>>();
    }

    inline void to_json(json & j, const Galax::Assets::ColorPalette & x) {
        j = json::object();
        j["name"] = x.name;
        j["heightStart"] = x.heightStart;
        j["transition"] = x.transition;
        j["colors"] = x.colors;
    }

    inline void from_json(const json & j, Galax::Assets::PlanetConfig& x) {
        x.name = j.at("name").get<std::string>();
        x.colorPalette = j.at("colorPalette").get<std::vector<Galax::Assets::ColorPalette>>();
    }

    inline void to_json(json & j, const Galax::Assets::PlanetConfig & x) {
        j = json::object();
        j["name"] = x.name;
        j["colorPalette"] = x.colorPalette;
    }
}
