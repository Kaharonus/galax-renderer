//
// Created by tomas on 6.12.22.
//

#include <orbital/SpaceCamera.h>
#include <iostream>
#include <glm/vec4.hpp>
#define GLM_SWIZZLE
#include <glm/glm.hpp>

using namespace Galax::Orbital;
using namespace Galax::Renderer::SceneObjects;

void SpaceCamera::update() {
	if(!followedObject){
		Camera::update();
		return;
	}
	//Using spherical coordinates to calculate camera position relative to followed object
	auto [deltaX, deltaY] = std::make_tuple(0.0, 0.0);

	if (input->isMouseButtonPressed(InputHandler::MOUSE_LEFT)) {
		if (previousPoint.x == 0.0 && previousPoint.y == 0.0) {
			previousPoint = input->getAbsolutePosition();
			return;
		}
		auto absolute = input->getAbsolutePosition();
		deltaX = absolute.x - previousPoint.x;
		deltaY = absolute.y - previousPoint.y;
		previousPoint = input->getAbsolutePosition();

	} else {
		previousPoint = {0,0};
	}




	yaw -= deltaX * mouseSensitivity;
	pitch += deltaY * mouseSensitivity;
	pitch = pitch > 89.0f ? 89.0f : pitch < -89.0f ? -89.0f : pitch;


	auto modelMatrix = followedObject->getRenderModelMatrix();
	auto camPos = glm::vec3(modelMatrix[3]);
	//Make sure the distance is about 2 times the radius of the object

	calculateDistance();

	auto x = distance * glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
	auto y = distance * glm::sin(glm::radians(pitch));
	auto z = distance * glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
	this->position = camPos + glm::vec3(x, y, z);

	this->cameraPositionUniform->setValue(this->position);
	this->cameraRotationUniform->setValue(glm::vec2(yaw, pitch));

	this->front = -glm::normalize(glm::vec3(x, y, z));
	this->forwardUniform->setValue(front);
	this->right = glm::normalize(glm::cross(front, glm::vec3(0, 1, 0)));
	this->up = glm::normalize(glm::cross(right, front));
	auto viewMatrix = glm::lookAt(this->position, camPos, up);

	this->viewMatrix = viewMatrix;
	this->viewUniform->setValue(viewMatrix);


}

void SpaceCamera::follow(const std::shared_ptr<IRenderNode> &object) {
	followedObject = object;
	if(object){
		distance = targetDistance = object->getScale().x * 4;
	}

}

void SpaceCamera::calculateDistance() {
	auto min = followedObject->getScale().z + 0.1;
	auto max = followedObject->getScale().z * 20;
	auto scrollDelta = input->getScrollDelta().y;
	this->targetDistance -= scrollDelta * 0.0025;
	this->targetDistance = targetDistance > max ? max : targetDistance < min ? min : targetDistance;
	//Update the distance by small step until it reaches the target distance
	if(std::abs(distance - targetDistance) < 0.1){
		distance = targetDistance;
		return;
	}
	if(distance < targetDistance){
		distance += 0.075;
	}else if(distance > targetDistance){
		distance -= 0.075;
	}
}
