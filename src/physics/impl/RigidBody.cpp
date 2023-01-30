//
// Created by tomas on 8.11.22.
//

#include <physics/impl/RigidBody.h>
#include "reactphysics3d/mathematics/Transform.h"

using namespace Galax::Physics;

RigidBody::RigidBody() {

}

RigidBody::RigidBody(const std::string &name) : IRigidBody(name) {

}


void RigidBody::setBodyPosition(const glm::vec3 &position) {
    this->position = position;
    this->transform = toRp3dTransform(position, rotation);
    if (this->getRP3DBody() != nullptr) {
        this->getRP3DBody()->setTransform(this->transform);
    }
}

void RigidBody::setBodyRotation(const glm::vec3 &rotation) {
    this->rotation = rotation;
    this->transform = toRp3dTransform(position, rotation);
    if (this->getRP3DBody() != nullptr) {
        this->getRP3DBody()->setTransform(this->transform);
    }
}


void RigidBody::createCollider(std::shared_ptr<ICollider> collider) {
    auto body = this->getRP3DBody();
    auto shape = collider->getRP3DCollisionShape();
    auto coll = body->addCollider(shape, reactphysics3d::Transform::identity());
    collider->setRP3DCollider(coll);
}


void RigidBody::addBodyCollider(std::shared_ptr<ICollider> collider) {
    colliders.push_back(collider);
    if (this->getRP3DBody() != nullptr) {
        createCollider(collider);
    }
}

std::shared_ptr<IRenderNode> RigidBody::getDebugNode() {
    return {}; //TODO
}

glm::vec3 RigidBody::getBodyPosition() const {
    return position;
}

glm::vec3 RigidBody::getBodyRotation() const {
    return rotation;
}

float RigidBody::getBodyMass() const {
    return mass;
}

void RigidBody::update() {
    if (this->getRP3DBody() == nullptr) {
        return;
    }

    for(const auto& collider : colliders) {
        if(!collider->getRP3DCollider()) {
            createCollider(collider);
        }
    }
}



void RigidBody::clearColliders() {
    if(this->getRP3DBody() == nullptr) {
        return;
    }

    for (auto i = 0; i <  this->getRP3DBody()->getNbColliders(); i++) {
        auto collider = this->getRP3DBody()->getCollider(i);
        this->getRP3DBody()->removeCollider(collider);
    }
    colliders.clear();

}

void RigidBody::setIsMouseOver(bool isMouseOver) {
    this->mouseOver = isMouseOver;
}

bool RigidBody::isMouseOver() const {
    return mouseOver;
}

std::vector<std::shared_ptr<ICollider>> RigidBody::getColliders() const {
    return colliders;
}
