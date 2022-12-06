
#include "Camera.h"
#include "../SceneObject.h"
#include "../interfaces/ICamera.h"

#include <glm/gtc/type_ptr.hpp>

#include <tuple>
#include <iostream>

using namespace Galax::Renderer::SceneObjects;

Camera::Camera() : ICamera() {
    init();
}

Camera::Camera(const std::string &name) : ICamera(name) {
    init();
}

void Camera::init(){

    viewUniform = std::make_shared<Uniform>("view", Uniform::Type::MAT4, glm::mat4(1.0));
    projectionUniform = std::make_shared<Uniform>("projection",Uniform::Type::MAT4,  glm::mat4(1.0f));
    cameraPositionUniform = std::make_shared<Uniform>("cameraPosition", Uniform::Type::VEC3, glm::vec3(0.0f));
    cameraRotationUniform = std::make_shared<Uniform>("cameraRotation", Uniform::Type::VEC2, glm::vec2(0.0f));
	resolutionUniform = std::make_shared<Uniform>("cameraResolution", Uniform::Type::VEC2, glm::vec2(0.0f));
	forwardUniform = std::make_shared<Uniform>("cameraForward", Uniform::Type::VEC3, glm::vec3(0.0f));

    position = glm::vec3(0.0f, 0.0f, 0.0f);
    front = glm::vec3(0.0f, 0.0f, -1.0f);
    up = glm::vec3(0.0f, 1.0f, 0.0f);
    worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

    yaw = -90.0f;
    pitch = 0.0f;
    movementSpeed = 0.01;
    mouseSensitivity = 0.1;
    setPerspective(60.0f, 0.1f, 1000.f);
	setResolution(1.0f, 1.0f);

}

Camera::~Camera() {
}


void Camera::setPosition(const glm::vec3 &position) {
    this->position = position;
	this->cameraPositionUniform->setValue(position);
}

void Camera::setRotation(float yaw, float pitch) {
    this->yaw = yaw;
    this->pitch = pitch;
}

void Camera::setMovementSpeed(float speed) {
    this->movementSpeed = speed;
}

void Camera::setMouseSensitivity(float sensitivity) {
    this->mouseSensitivity = sensitivity;
}

void Camera::setWorldUp(const glm::vec3 &worldUp) {
    this->worldUp = worldUp;
}

void Camera::setInputHandler(std::shared_ptr<InputHandler> input) {
    this->input = input;
}

void Camera::acceptInput(bool acceptingInput) {
    this->acceptingInput = acceptingInput;
}

void Camera::update() {
    if (!acceptingInput || input == nullptr) {
        return;
    }
    if (input->isKeyPressed(InputHandler::KEY_W)) {
        position += front * movementSpeed * frameTime;
    }
    if (input->isKeyPressed(InputHandler::KEY_S)) {
        position -= front * movementSpeed * frameTime;
    }
    if (input->isKeyPressed(InputHandler::KEY_A)) {
        position -= right * movementSpeed * frameTime;
    }
    if (input->isKeyPressed(InputHandler::KEY_D)) {
        position += right * movementSpeed * frameTime;
    }

    cameraPositionUniform->setValue(position);

    auto [deltaX, deltaY] = std::make_tuple(0.0, 0.0);
    if (input->isMouseButtonPressed(InputHandler::MOUSE_LEFT)) {
        if (previousPoint.x == 0.0 && previousPoint.y == 0.0) {
            viewMatrix = lookAt(position, position + front, up);
            viewUniform->setValue(viewMatrix);
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
    // This can be moved inside the if statement, but the frametime is more consistent if it is here

    yaw -= deltaX * mouseSensitivity;
    pitch -= deltaY * mouseSensitivity;
    pitch = pitch > 89.0f ? 89.0f : pitch < -89.0f ? -89.0f : pitch;

    cameraRotationUniform->setValue(glm::vec2(yaw, pitch));

    glm::vec3 frontTmp;
    frontTmp.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    frontTmp.y = sin(glm::radians(pitch));
    frontTmp.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = -glm::normalize(frontTmp);
	forwardUniform->setValue(front);
    // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
    viewMatrix = lookAt(position, position + front, up);
    viewUniform->setValue(viewMatrix);
}

void Camera::setResolution(float width, float height) {
    this->width = width;
    this->height = height;
    this->aspect = width / height;
    this->projectionMatrix = glm::perspective(glm::radians(fov), aspect, near, far);
    projectionUniform->setValue(projectionMatrix);
	resolutionUniform->setValue(glm::vec2(width, height));

}

void Camera::setPerspective(float fov, float near, float far) {
    this->fov = fov;
    this->near = near;
    this->far = far;
    this->projectionMatrix = glm::perspective(glm::radians(fov), aspect, near, far);
    projectionUniform->setValue(projectionMatrix);
}

glm::mat4 Camera::getViewMatrix() const {
    return viewMatrix;
}

glm::mat4 Camera::getProjectionMatrix() const {
    return projectionMatrix;
}

glm::vec3 Camera::getPosition() const {
    return position;
}

float Camera::getWidth() const {
    return width;
}

float Camera::getHeight() const {
    return height;
}

void Camera::setFOV(float fov) {
    this->setPerspective(fov, this->near, this->far);
}

std::tuple<float, float> Camera::getRotation() const {
    return std::make_tuple(yaw, pitch);
}

float Camera::getMovementSpeed() const {
    return movementSpeed;
}

float Camera::getMouseSensitivity() const {
    return mouseSensitivity;
}

bool Camera::acceptsInput() const {
    return this->acceptingInput;
}

float Camera::getFOV() const {
    return fov;
}

uint Camera::getId() {
    return -1;
}

std::shared_ptr<Galax::Renderer::IUniform> Camera::getProjectionUniform() const {
    return projectionUniform;
}

std::shared_ptr<Galax::Renderer::IUniform> Camera::getViewUniform() const {
    return viewUniform;
}

std::shared_ptr<Galax::Renderer::IUniform> Camera::getPositionUniform() const {
    return cameraPositionUniform;
}

std::shared_ptr<Galax::Renderer::IUniform> Camera::getRotationUniform() const {
    return cameraRotationUniform;
}

glm::vec3 Camera::getDirection() const {
    return front;
}

std::shared_ptr<Galax::Renderer::IUniform> Camera::getResolutionUniform() const {
	return resolutionUniform;
}

std::shared_ptr<Galax::Renderer::IUniform> Camera::getForwardUniform() const {
	return forwardUniform;
}
