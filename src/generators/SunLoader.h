//
// Created by tomas on 20.9.22.
//


#pragma once

#include <memory>
#include <unordered_map>

#include "../assets/AssetLoader.h"
#include "../renderer/scene_objects/Shader.h"
#include "../renderer/scene_objects/Program.h"
#include "../renderer/scene_objects/Texture.h"
#include "../renderer/scene_objects/Mesh.h"
#include "../renderer/scene_objects/Node.h"


namespace Galax::Generators {

    using namespace Galax::Renderer::SceneObjects;
    using namespace Galax::Assets;

    class SunLoader {
    public:
        static std::shared_ptr<Node> load(std::shared_ptr<AssetLoader> &loader);

    };
}
