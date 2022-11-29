
#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <tuple>
#include "../SceneObject.h"
#include "../input/InputHandler.h"
#include "../interfaces/IUniform.h"
#include "Uniform.h"

#include "../interfaces/ICamera.h"

namespace Galax::Renderer::SceneObjects {

	class Camera : public ICamera {
	public:
		Camera();

		Camera(const std::string &name);

		~Camera() override;

		void setPosition(const glm::vec3 &position) override;

		void setRotation(float yaw, float pitch) override;

		void setMovementSpeed(float speed) override;

		void setMouseSensitivity(float sensitivity) override;

		void setFOV(float fov) override;

		void setPerspective(float fov, float near, float far) override;

		void setResolution(float width, float height) override;

		void setWorldUp(const glm::vec3 &worldUp) override;


		[[nodiscard]] glm::mat4 getViewMatrix() const override;

		[[nodiscard]] glm::mat4 getProjectionMatrix() const override;

		[[nodiscard]] std::shared_ptr<IUniform> getViewUniform() const override;

		[[nodiscard]] std::shared_ptr<IUniform> getProjectionUniform() const override;

		[[nodiscard]] std::shared_ptr<IUniform> getPositionUniform() const override;

		[[nodiscard]] std::shared_ptr<IUniform> getRotationUniform() const override;

		[[nodiscard]] std::shared_ptr<IUniform> getForwardUniform() const override;

		[[nodiscard]] glm::vec3 getDirection() const override;

		std::shared_ptr<IUniform> getResolutionUniform() const override;

		[[nodiscard]] glm::vec3 getPosition() const override;

		[[nodiscard]] std::tuple<float, float> getRotation() const override;

		[[nodiscard]] float getWidth() const override;

		[[nodiscard]] float getHeight() const override;

		[[nodiscard]] float getFOV() const override;

		[[nodiscard]] float getMovementSpeed() const override;

		[[nodiscard]] float getMouseSensitivity() const override;

		[[nodiscard]] bool acceptsInput() const;

		void update() override;

		void setInputHandler(std::shared_ptr<InputHandler> input) override;

		void acceptInput(bool acceptingInput);

		uint getId() override;


	private:
		std::shared_ptr<InputHandler> input;
		bool acceptingInput = false;

		glm::vec2 previousPoint = {};


		glm::mat4 viewMatrix;
		glm::mat4 projectionMatrix;

		std::shared_ptr<Uniform> viewUniform;
		std::shared_ptr<Uniform> projectionUniform;
		std::shared_ptr<Uniform> cameraPositionUniform;
		std::shared_ptr<Uniform> cameraRotationUniform;
		std::shared_ptr<Uniform> resolutionUniform;
		std::shared_ptr<Uniform> forwardUniform;
		glm::vec3 position;
		glm::vec3 front;
		glm::vec3 up;
		glm::vec3 right;
		glm::vec3 worldUp;
		float yaw;
		float pitch;
		float movementSpeed;
		float mouseSensitivity;
		float fov;
		float near;
	private:
		float far;
		float width;
		float height;
		float aspect;

		void init();


	};
}



