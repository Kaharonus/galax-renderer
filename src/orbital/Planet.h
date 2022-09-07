//
// Created by tomas on 7.9.22.
//

#pragma once

#include "../renderer/scene_objects/Node.h"

namespace Galax::Orbital {


    class Planet : public Renderer::SceneObjects::Node {
    public:
        enum class Type {
            ROCKY,
            GAS_GIANT,
            ICE,
            HOT,
            TEMPERATE,
            OCEAN
        };

        Planet(const std::string& name, Planet::Type type);

    private:

        Type type;
    };

}
