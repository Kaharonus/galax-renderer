//
// Created by tomas on 8.11.22.
//

#include <physics/impl/RigidBody.h>

using namespace Galax::Physics;

RigidBody::RigidBody() {

}

void RigidBody::setBodyPosition(const glm::vec3 &position) {
    this->position = position;
    this->transform = toRp3dTransform(position, rotation);
    if(this->getRP3DBody() != nullptr) {
        this->getRP3DBody()->setTransform(this->transform);
    }
}

void RigidBody::setBodyRotation(const glm::vec3 &rotation) {
    this->rotation = rotation;
    this->transform = toRp3dTransform(position, rotation);
    if(this->getRP3DBody() != nullptr) {
        this->getRP3DBody()->setTransform(this->transform);
    }
}


void RigidBody::setBodyAngularVelocity(const glm::vec3 &velocity) {
    this->angularVelocity = velocity;
    if(this->getRP3DBody() != nullptr) {
        this->getRP3DBody()->setAngularVelocity(rp3d::Vector3(velocity.x, velocity.y, velocity.z));
    }
}

void RigidBody::setBodyMass(float mass) {
    this->mass = mass;
    if(this->getRP3DBody() != nullptr) {
        this->getRP3DBody()->setMass(mass);
    }
}

void RigidBody::createCollider(std::shared_ptr<ICollider> collider){
    auto body = this->getRP3DBody();
    auto coll = body->addCollider(collider->getRP3DCollisionShape(), transform);

}


void RigidBody::addBodyCollider(std::shared_ptr<ICollider> collider) {
    colliders.push_back(collider);
    if(this->getRP3DBody() != nullptr) {
        createCollider(collider);

    }
}

std::shared_ptr<INode> RigidBody::getDebugNode() {
    return std::shared_ptr<INode>();
}

glm::vec3 RigidBody::getBodyPosition() const {
    return position;
}

glm::vec3 RigidBody::getBodyRotation() const {
    return rotation;
}


glm::vec3 RigidBody::getBodyAngularVelocity() const {
    return angularVelocity;
}

float RigidBody::getBodyMass() const {
    return mass;
}

void RigidBody::update() {
    if(this->getRP3DBody() != nullptr) {
        auto transform = this->getRP3DBody()->getTransform();
        this->position = toGlmVec3(transform.getPosition());
        this->rotation = toGlmVec3(transform.getOrientation().getVectorV());
    }

}

void RigidBody::addForce(const glm::vec3 &force) {
    this->forces.emplace_back(force.x, force.y, force.z);
    if(this->getRP3DBody() != nullptr) {
        this->getRP3DBody()->applyLocalForceAtCenterOfMass(rp3d::Vector3(force.x, force.y, force.z));
    }
}

std::vector<rp3d::Vector3> RigidBody::getForces() const {
    return forces;
}
