//
// Created by tomas on 8.11.22.
//


#pragma once

#include <renderer/impl/RenderNode.h>
#include <physics/impl/RigidBody.h>

namespace Galax::Physics{
    using namespace Galax::Renderer::SceneObjects;

    class PhysicalNode : public RenderNode, public RigidBody {
    public:
        explicit PhysicalNode(const std::string& name);

        void setBodyPosition(const glm::vec3 &position) override;

        void setBodyRotation(const glm::vec3 &rotation) override;

        void setPosition(const glm::vec3 &position) override;

        void setRotation(const glm::vec3 &rotation) override;

        void update() override;

        void setIsMouseOver(bool isMouseOver) override;

    protected:
        std::shared_ptr<IUniform> mouseOverUniform;
		std::mutex mut;

    };

}

