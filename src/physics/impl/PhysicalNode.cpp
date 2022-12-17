//
// Created by tomas on 8.11.22.
//

#include <physics/impl/PhysicalNode.h>

using namespace Galax::Physics;

PhysicalNode::PhysicalNode(const std::string name) : Node(name), RigidBody(name) {
    mouseOverUniform = std::make_shared<Uniform>("isMouseOver", Uniform::BOOL ,false);
}

void PhysicalNode::setBodyPosition(const glm::vec3 &position) {
    RigidBody::setBodyPosition(parent ? parent->getPosition() + position : position);
    Node::setPosition(position);
}

void PhysicalNode::setBodyRotation(const glm::vec3 &rotation) {
    Node::setPosition(rotation);
    RigidBody::setBodyRotation(rotation);
}

void PhysicalNode::setPosition(const glm::vec3 &position) {
	auto physicsPosition = parent ? (parent->getPosition() + (position)) : position;
    RigidBody::setBodyPosition(physicsPosition);
    Node::setPosition(position);
}

void PhysicalNode::setRotation(const glm::vec3 &rotation) {
    RigidBody::setBodyRotation(rotation);
    Node::setRotation(rotation);
}

void PhysicalNode::update() {
	//Probably slow. Do not care. Will fix later. Now ill fix it by fucking slowing down the refresh rate LOL
	setPosition(this->Galax::Renderer::SceneObjects::Node::position);
	auto physicsPosition = RigidBody::getBodyPosition();
    RigidBody::update();
    auto updatedPhysicsPos = RigidBody::getBodyPosition();
	auto diff = updatedPhysicsPos - physicsPosition;
	Node::setPosition(Node::getRelativePosition() + diff);
    Node::setRotation(this->getBodyRotation());
}

void PhysicalNode::setIsMouseOver(bool isMouseOver) {
    mouseOverUniform->setValue(isMouseOver);
    RigidBody::setIsMouseOver(isMouseOver);
}

