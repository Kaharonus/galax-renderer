//
// Created by tomas on 27.8.22.
//


#pragma once
#include "../renderer/scene_objects/Shader.h"
#include "../renderer/scene_objects/Mesh.h"

#include <string>
namespace Galax::Assets {
    using namespace Renderer::SceneObjects;

    class AssetLoader {
    public:
        AssetLoader(std::string assetFile);
        bool load();

        std::shared_ptr<Shader> getShader(std::string path, Shader::Type type, std::string name = "");
        std::shared_ptr<Mesh> getMesh(const std::string &asset, std::string name = "");
        std::vector<std::shared_ptr<Mesh>> getMesh(const std::string &directory, int levels, std::string name = "");

        std::string readTextFile(std::string asset);

    private:
        void exists(const std::string& file);

        std::string assetFile;
        bool loaded = false;
        const std::string path = "/assets/assets/";

    };

} // Galax::Assets
