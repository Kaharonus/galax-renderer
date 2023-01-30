//
// Created by tomas on 27.9.22.
//

#pragma once

#include <vector>
#include <glm/vec3.hpp>
#include "../SceneObject.h"
namespace Galax::Renderer {
    class IMesh : public Galax::Renderer::SceneObject {
    public:
        IMesh() : SceneObject() {};

        IMesh(const std::string &name) : SceneObject(name) {};

        virtual ~IMesh() = default;

        virtual std::vector<float> &getVertices() = 0;

        virtual std::vector<unsigned int> &getIndices() = 0;

        virtual void bind() = 0;

        virtual void unbind() = 0;

        virtual unsigned int size() = 0;

        [[nodiscard]] virtual glm::vec3 getCenter() const = 0;

    };
}