//
// Created by tomas on 27.9.22.
//

#pragma once

#include <glm/vec3.hpp>
#include <memory>
#include "IMesh.h"
#include "IAnimation.h"
#include "IUniform.h"
#include "ICamera.h"
#include "ITexture.h"
#include "IProgram.h"

namespace Galax::Renderer {
    class INode : public Galax::Renderer::SceneObject {
    public:
        enum class DrawTarget {
            SCREEN,
            TEXTURE
        };

        INode() : SceneObject() {};

        INode(const std::string &name) : SceneObject(name) {};

        virtual ~INode() = default;

        virtual void draw() = 0;

        virtual const glm::vec3 &getPosition() = 0;

        virtual const glm::vec3 &getRotation() = 0;

        virtual const glm::vec3 &getScale() = 0;

        virtual void setPosition(const glm::vec3 &position) = 0;

        virtual void setRotation(const glm::vec3 &rotation) = 0;

        virtual void setScale(const glm::vec3 &scale) = 0;

        virtual void addChild(std::shared_ptr<INode> child) = 0;

        virtual void setMesh(std::shared_ptr<IMesh> mesh) = 0;

        virtual void setProgram(std::shared_ptr<IProgram> program) = 0;

        virtual void setCamera(std::shared_ptr<ICamera> camera) = 0;

        virtual void addUniform(std::shared_ptr<IUniform> uniform) = 0;

        virtual void addTexture(std::shared_ptr<ITexture> texture) = 0;

        virtual void addAnimation(std::shared_ptr<IAnimation> animation) = 0;

        virtual void setDrawTarget(DrawTarget drawTarget) = 0;

        virtual void setDrawTexture(std::shared_ptr<ITexture> texture) = 0;

        virtual void drawAsWireframe(bool enabled) = 0;

        [[nodiscard]] virtual std::shared_ptr<IMesh> getMesh() const = 0;

        [[nodiscard]] virtual std::vector<std::shared_ptr<IProgram>> getPrograms() const = 0;

        [[nodiscard]] virtual std::shared_ptr<ICamera> getCamera() const = 0;

        [[nodiscard]] virtual std::vector<std::shared_ptr<IUniform>> getUniforms() const = 0;

        [[nodiscard]] virtual std::vector<std::shared_ptr<ITexture>> getTextures() const = 0;

        [[nodiscard]] virtual std::vector<std::shared_ptr<INode>> getChildren() const = 0;

        [[nodiscard]] virtual std::vector<std::shared_ptr<IAnimation>> getAnimations() const = 0;

        [[nodiscard]] virtual DrawTarget getDrawTarget() const = 0;

    private:
        virtual void init() = 0;


    };
}