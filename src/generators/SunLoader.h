//
// Created by tomas on 20.9.22.
//


#pragma once

#include <memory>
#include <unordered_map>
#include <orbital/Sun.h>
#include "../assets/AssetLoader.h"
#include "../renderer/impl/Shader.h"
#include "../renderer/impl/Program.h"
#include "../renderer/impl/Texture.h"
#include "../renderer/impl/Mesh.h"
#include "../renderer/impl/Node.h"


namespace Galax::Generators {

    using namespace Galax::Renderer::SceneObjects;
    using namespace Galax::Assets;
	using namespace Galax::Orbital;

    class SunLoader {
    public:

		static std::shared_ptr<Sun> load(std::shared_ptr<AssetLoader> &loader);

    };
}
