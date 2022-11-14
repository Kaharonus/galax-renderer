//
// Created by tomas on 8.11.22.
//


#pragma once

#include <physics/interfaces/IRigidBody.h>

#include <renderer/interfaces/INode.h>
#include <physics/interfaces/IForce.h>

namespace Galax::Physics {
    using namespace Galax::Renderer;

    class RigidBody : public IRigidBody {
    public:
        RigidBody();

        explicit RigidBody(const std::string& name);

        void setBodyPosition(const glm::vec3 &position) override;

        void setBodyRotation(const glm::vec3 &rotation) override;

        void addForce(std::shared_ptr<IForce> force) override;

        void setBodyAngularVelocity(const glm::vec3 &velocity) override;

        void setBodyMass(float mass) override;

        void addBodyCollider(std::shared_ptr<ICollider> collider) override;

        void update() override;

        std::shared_ptr<INode> getDebugNode() override;

        [[nodiscard]] glm::vec3 getBodyPosition() const override;

        [[nodiscard]] glm::vec3 getBodyRotation() const override;

        [[nodiscard]] std::vector<std::shared_ptr<IForce>> getForces() const override;

        [[nodiscard]] glm::vec3 getBodyAngularVelocity() const override;

        [[nodiscard]] float getBodyMass() const override;

        [[nodiscard]] glm::vec3 getResultingForce() const override;

        void clearColliders() override;

        void setIsMouseOver(bool isMouseOver) override;

        bool getIsMouseOver() const override;

        std::vector<std::shared_ptr<ICollider>> getColliders() const override;

    protected:
        void createCollider(std::shared_ptr<ICollider> collider);

        bool isMouseOver = false;
        bool isBodyVisible = true;

        rp3d::Transform transform;
        glm::vec3 position;
        glm::vec3 rotation;
        std::vector<std::shared_ptr<IForce>> forces;
        glm::vec3 velocity;
        glm::vec3 angularVelocity;
        glm::vec3 resultingForce = {};
        float mass;
        std::vector<std::shared_ptr<ICollider>> colliders;

    private:

    };
}


