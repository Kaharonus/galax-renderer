//
// Created by tomas on 8.11.22.
//


#pragma once

#include <renderer/impl/Node.h>
#include <physics/impl/RigidBody.h>

namespace Galax::Physics{
    using namespace Galax::Renderer::SceneObjects;

    class PhysicalNode : public Node, public RigidBody {
    public:
        explicit PhysicalNode(const std::string name);

        void setBodyPosition(const glm::vec3 &position) override;

        void setBodyRotation(const glm::vec3 &rotation) override;

        void setPosition(const glm::vec3 &position) override;

        void setRotation(const glm::vec3 &rotation) override;

        void update() override;

    };

}

