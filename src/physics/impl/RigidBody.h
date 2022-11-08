//
// Created by tomas on 8.11.22.
//


#pragma once

#include <physics/interfaces/IRigidBody.h>

#include <renderer/interfaces/INode.h>

namespace Galax::Physics {
    using namespace Galax::Renderer;

    class RigidBody : public IRigidBody {
    public:
        RigidBody();

        void setBodyPosition(const glm::vec3 &position) override;

        void setBodyRotation(const glm::vec3 &rotation) override;

        void addForce(const glm::vec3 &force) override;

        void setBodyAngularVelocity(const glm::vec3 &velocity) override;

        void setBodyMass(float mass) override;

        void addBodyCollider(std::shared_ptr<ICollider> collider) override;

        void update() override;

        std::shared_ptr<INode> getDebugNode() override;

        [[nodiscard]] glm::vec3 getBodyPosition() const override;

        [[nodiscard]] glm::vec3 getBodyRotation() const override;

        [[nodiscard]] std::vector<rp3d::Vector3> getForces() const override;

        [[nodiscard]] glm::vec3 getBodyAngularVelocity() const override;

        [[nodiscard]] float getBodyMass() const override;

    protected:
        void createCollider(std::shared_ptr<ICollider> collider);

        rp3d::Transform transform;
        glm::vec3 position;
        glm::vec3 rotation;
        std::vector<rp3d::Vector3> forces;
        glm::vec3 velocity;
        glm::vec3 angularVelocity;
        float mass;
        std::vector<std::shared_ptr<ICollider>> colliders;

    private:

    };
}


