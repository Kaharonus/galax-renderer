//
// Created by tomas on 27.8.22.
//


#pragma once
#include "../renderer/scene_objects/Shader.h"
#include "../renderer/scene_objects/Mesh.h"
#include "../renderer/scene_objects/Texture.h"


#include <nlohmann/json.hpp>


#include <string>
namespace Galax::Assets {
    using namespace Renderer::SceneObjects;
    using namespace nlohmann;

    class AssetLoader {
    public:
        AssetLoader();
        bool load(const std::string& assetFile);

        std::shared_ptr<Shader> getShader(std::string path, Shader::Type type, std::string name = "");
        std::shared_ptr<Mesh> getMesh(const std::string &asset, std::string name = "");

        std::string readTextFile(std::string asset);

        std::shared_ptr<Texture> getCubemap(const std::string& asset, std::string name = "");

        template <typename T> std::shared_ptr<T> getJson(const std::string& asset){
            auto text = readTextFile(asset);
            auto ptr = std::make_shared<T>();
            auto jsonObj = nlohmann::json::parse(text);
            nlohmann::from_json(jsonObj, *ptr);
            return ptr;
        };

    private:
        void exists(const std::string& file);

        std::string assetFile;
        bool loaded = false;
        const std::string path = "/assets/assets/";

    };

} // Galax::Assets
