
#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <tuple>
#include "../SceneObject.h"
#include "../input/InputHandler.h"
#include "Uniform.h"

namespace Galax::Renderer::SceneObjects{

    class Camera : public SceneObject {
    public:
        Camera();
        Camera(const std::string& name);
        ~Camera();

        void setPosition(const glm::vec3& position);
        void setRotation(float yaw, float pitch);
        void setMovementSpeed(float speed);
        void setMouseSensitivity(float sensitivity);
        void setFOV(float fov);
        void setPerspective(float fov, float near, float far);
        void setDimensions(float width, float height);
        void setWorldUp(const glm::vec3& worldUp);
    
        [[nodiscard]] glm::mat4 getViewMatrix() const;
        [[nodiscard]] glm::mat4 getProjectionMatrix() const;
        [[nodiscard]] std::shared_ptr<Uniform> getViewUniform() const;
        [[nodiscard]] std::shared_ptr<Uniform> getProjectionUniform() const;
        [[nodiscard]] std::shared_ptr<Uniform> getPositionUniform() const;
        [[nodiscard]] std::shared_ptr<Uniform> getRotationUniform() const;


        [[nodiscard]] glm::vec3 getPosition() const;
        [[nodiscard]] std::tuple<float, float> getRotation() const;
        [[nodiscard]] float getWidth() const;
        [[nodiscard]] float getHeight() const;
        [[nodiscard]] float getFOV() const;
        [[nodiscard]] float getMovementSpeed() const;
        [[nodiscard]] float getMouseSensitivity() const;
        [[nodiscard]] bool acceptsInput() const;
        void update();

        void setInputHandler(std::shared_ptr<InputHandler> input);
        void acceptInput(bool acceptingInput);

        uint getId() override;


    private:
        std::shared_ptr<InputHandler> input;
        bool acceptingInput = false;

        std::tuple<float, float> previousPoint = std::make_tuple(.0, 0.0);


        glm::mat4 viewMatrix;
        glm::mat4 projectionMatrix;

        std::shared_ptr<Uniform> viewUniform;
        std::shared_ptr<Uniform> projectionUniform;
        std::shared_ptr<Uniform> cameraPositionUniform;
        std::shared_ptr<Uniform> cameraRotationUniform;
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
        float far;
        float width;
        float height;
        float aspect;

        void init();

    };
}



