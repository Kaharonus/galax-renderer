//
// Created by tomas on 8.11.22.
//

#include <physics/impl/PhysicalNode.h>

using namespace Galax::Physics;

PhysicalNode::PhysicalNode(const std::string name) : Node(name), RigidBody() {

}

void PhysicalNode::setBodyPosition(const glm::vec3 &position) {
    RigidBody::setBodyPosition(position);
    Node::setPosition(position);
}

void PhysicalNode::setBodyRotation(const glm::vec3 &rotation) {
    Node::setPosition(rotation);
    RigidBody::setBodyRotation(rotation);
}

void PhysicalNode::setPosition(const glm::vec3 &position) {
    RigidBody::setBodyPosition(position);
    Node::setPosition(position);
}

void PhysicalNode::setRotation(const glm::vec3 &rotation) {
    RigidBody::setBodyRotation(rotation);
    Node::setRotation(rotation);
}

void PhysicalNode::update() {
    RigidBody::update();
    auto pos = RigidBody::getBodyPosition();
    Node::setPosition(pos);
    Node::setRotation(this->getBodyRotation());
}

