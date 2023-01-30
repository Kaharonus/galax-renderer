
//
// Created by tomas on 8.11.22.
//


#pragma once

#include <physics/interfaces/IRigidBody.h>

#include <renderer/interfaces/IRenderNode.h>

namespace Galax::Physics {
    using namespace Galax::Renderer;

    class RigidBody : public IRigidBody {
    public:
        RigidBody();

        explicit RigidBody(const std::string& name);

        void setBodyPosition(const glm::vec3 &position) override;

        void setBodyRotation(const glm::vec3 &rotation) override;

        void addBodyCollider(std::shared_ptr<ICollider> collider) override;

        void update() override;

        std::shared_ptr<IRenderNode> getDebugNode() override;

        [[nodiscard]] glm::vec3 getBodyPosition() const override;

        [[nodiscard]] glm::vec3 getBodyRotation() const override;



        [[nodiscard]] float getBodyMass() const override;


        void clearColliders() override;

        void setIsMouseOver(bool isMouseOver) override;

        bool isMouseOver() const override;

        [[nodiscard]] std::vector<std::shared_ptr<ICollider>> getColliders() const override;

    protected:
        void createCollider(std::shared_ptr<ICollider> collider);

        bool mouseOver = false;
        bool isBodyVisible = true;

        rp3d::Transform transform;
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 velocity;
        float mass;
        std::vector<std::shared_ptr<ICollider>> colliders;

    private:

    };
}


