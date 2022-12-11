//
// Created by tomas on 5.11.22.
//

#pragma once

#include <glm/vec3.hpp>
#include <reactphysics3d/reactphysics3d.h>
#include <renderer/interfaces/IRenderNode.h>
#include <physics/PhysicsObject.h>
#include <physics/interfaces/ICollider.h>
#include <physics/interfaces/IForce.h>

namespace Galax::Physics {
    using namespace Galax::Renderer;
    class IRigidBody : public PhysicsObject {
        friend class PhysicsEngine;

    public:

        IRigidBody() : PhysicsObject() {};

        IRigidBody(const std::string name) : PhysicsObject(name) {}

        virtual void update() = 0;

        virtual void setBodyPosition(const glm::vec3 &position) = 0;

        virtual void setBodyRotation(const glm::vec3 &rotation) = 0;

        virtual void addForce(std::shared_ptr<IForce> force) = 0;

        virtual void setBodyAngularVelocity(const glm::vec3 &velocity) = 0;

        virtual void setBodyMass(float mass) = 0;

        virtual void addBodyCollider(std::shared_ptr<ICollider> collider) = 0;

        virtual std::shared_ptr<IRenderNode> getDebugNode() = 0;

        virtual glm::vec3 getBodyPosition() const = 0;

        virtual glm::vec3 getBodyRotation() const = 0;

        [[nodiscard]] virtual std::vector<std::shared_ptr<IForce>> getForces() const = 0;

        virtual glm::vec3 getBodyAngularVelocity() const = 0;

        virtual float getBodyMass() const = 0;

        [[nodiscard]] virtual glm::vec3 getResultingForce() const = 0;

        virtual void clearColliders() = 0;

        virtual std::vector<std::shared_ptr<ICollider>> getColliders() const = 0;

        virtual void setIsMouseOver(bool isMouseOver) = 0;

        virtual bool isMouseOver() const = 0;

    protected:
        [[nodiscard]] static rp3d::Transform toRp3dTransform(const glm::vec3 &position, const glm::vec3 &rotation) {
            return {
                rp3d::Vector3(position.x, position.y, position.z),
                rp3d::Quaternion::fromEulerAngles(rotation.x, rotation.y, rotation.z)
            };
        };

        [[nodiscard]] static glm::vec3 toGlmVec3(const rp3d::Vector3 &vector) {
            return {vector.x, vector.y, vector.z};
        }

        [[nodiscard]] rp3d::RigidBody* getRP3DBody() const{
            return this->body;
        };



    private:

        void setRP3DBody(rp3d::RigidBody *b){
            this->body = b;
        };

        rp3d::RigidBody *body = nullptr;

    };



}