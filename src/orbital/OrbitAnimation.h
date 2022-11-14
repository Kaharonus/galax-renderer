//
// Created by tomas on 9.11.22.
//


#pragma once

#include <renderer/interfaces/IAnimation.h>
#include <glm/gtc/constants.hpp>

namespace Galax::Orbital {
    using namespace Galax::Renderer;

    class OrbitAnimation : public IAnimation {
    public:
        OrbitAnimation();

        explicit OrbitAnimation(const std::string &name);

        uint getId() override;

        void update() override;

        void start() override;

        void stop() override;

        void setTarget(Target target) override;


        [[nodiscard]] IAnimation::Target getTarget() const override;

        [[nodiscard]] std::shared_ptr<IUniform> getCustomTarget() const override;

        glm::vec3 getValue() override;

        void setSunPosition(std::shared_ptr<IUniform> sunPosition);

        void setPlanetRadius(std::shared_ptr<IUniform> planetRadius);

        void setInitialPosition(glm::vec3 initialPosition);

        void setOffset(glm::vec3 offset);

    private:
        glm::vec3 currentPosition = {0, 0, 0};
        glm::vec3 radius = {0, 0, 0};
        glm::vec3 offset = {0, 0, 0};

        std::shared_ptr<IUniform> planetSize;
        std::shared_ptr<IUniform> sunPosition;

        // Timing
        unsigned long previousTime;
        float animationTime;
        float progress = 0;
        float animationLength = 0.0f;

        void updateTime();

        // Constants
        float animationSpeed = 1.f;
        float massConstant = 1000.0f;
        static constexpr auto pi2 = glm::two_pi<float>();
        static constexpr auto pi_43 = glm::pi<float>() * 4.0f / 3.0f;
        static constexpr float G = 6.67408e-11;
    };


}
