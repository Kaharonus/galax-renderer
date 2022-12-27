//
// Created by tomas on 8.11.22.
//

#include <physics/impl/PhysicalNode.h>

using namespace Galax::Physics;

PhysicalNode::PhysicalNode(const std::string& name) : RenderNode(name), RigidBody(name) {
    mouseOverUniform = std::make_shared<Uniform>("isMouseOver", Uniform::BOOL ,false);
}

void PhysicalNode::setBodyPosition(const glm::vec3 &position) {
	auto physicsPosition = parent ? parent->getPosition() + position : position;
	RigidBody::setBodyPosition(physicsPosition);
    RenderNode::setPosition(position);
}

void PhysicalNode::setBodyRotation(const glm::vec3 &rotation) {
	RenderNode::setPosition(rotation);
    RigidBody::setBodyRotation(rotation);
}

void PhysicalNode::setPosition(const glm::vec3 &position) {
	auto physicsPosition = parent ? parent->getPosition() + position : position;
    RigidBody::setBodyPosition(physicsPosition);
    RenderNode::setPosition(position);
}

void PhysicalNode::setRotation(const glm::vec3 &rotation) {
	RigidBody::setBodyRotation(rotation);
    RenderNode::setRotation(rotation);
}

void PhysicalNode::update() {
	setPosition(this->Galax::Renderer::SceneObjects::RenderNode::position);
	auto physicsPosition = RigidBody::getBodyPosition();
    RigidBody::update();
    auto updatedPhysicsPos = RigidBody::getBodyPosition();
	auto diff = updatedPhysicsPos - physicsPosition;
	RenderNode::setPosition(RenderNode::getRelativePosition() + diff);
    RenderNode::setRotation(this->getBodyRotation());
}

void PhysicalNode::setIsMouseOver(bool isMouseOver) {
    mouseOverUniform->setValue(isMouseOver);
    RigidBody::setIsMouseOver(isMouseOver);
}

