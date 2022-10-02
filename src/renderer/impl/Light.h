//
// Created by tomas on 14.9.22.
//


#pragma once

#include "../interfaces/ILight.h"

namespace Galax::Renderer::SceneObjects{
    class Light : public ILight {
        Light();
        Light(const std::string &name);
        ~Light() override = default;
        void setPos(glm::vec3 pos) override;
        void setPositionUniform(std::shared_ptr<IUniform> uniform) override;
        void setIntensity(float intensity) override;
        void setIntensityUniform(std::shared_ptr<IUniform> uniform) override;
        void setColor(glm::vec3 color) override;
        void setFalloff(float falloff) override;
        LightData getLightData() override;
        bool isUpdated() override;

    private:
        void init();

        std::shared_ptr<IUniform> positionUniform;
        glm::vec3 position;
        std::shared_ptr<IUniform> intensityUniform;
        float intensity;
        glm::vec3 color;
        float falloff;
        bool updated = false;
    };

}
