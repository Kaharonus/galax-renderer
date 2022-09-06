//
// Created by tomas on 3.9.22.
//


#pragma once

#include "../renderer/Scene.h"
#include "../assets/AssetLoader.h"

#include <memory>

namespace Galax::Generators{

    using namespace Galax::Renderer;
    using namespace Galax::Assets;

    class SceneGenerator {
    public:
        SceneGenerator() = default;
        ~SceneGenerator() = default;
        std::shared_ptr<Scene> generateScene(float w, float h);


    private:
        std::shared_ptr<Node> generateNode(std::shared_ptr<AssetLoader> loader);

        std::shared_ptr<Camera> generateCamera(std::shared_ptr<AssetLoader> loader, float w, float h);

        std::map<float, std::shared_ptr<Mesh>> generateMesh(std::shared_ptr<AssetLoader> loader);

        std::shared_ptr<Program> generateProgram(std::shared_ptr<AssetLoader> loader);

        std::vector<std::shared_ptr<Animation>> generateAnimations(std::shared_ptr<AssetLoader> loader);

        std::shared_ptr<Node> generatePlanet(std::map<float, std::shared_ptr<Mesh>> meshes, float size, float distance,
                                             std::shared_ptr<Program> program);

        std::shared_ptr<Node> generatePlanet(std::map<float, std::shared_ptr<Mesh>> meshes, float size, float distance);

        std::shared_ptr<Animation> generateRotation(float distance);
    };
}

