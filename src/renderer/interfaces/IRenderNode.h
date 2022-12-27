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
#include <renderer/LightingModel.h>
#include <generator.hpp>

namespace Galax::Renderer {
	class IRenderNode : public Galax::Renderer::SceneObject {
	public:
		enum class DrawTarget {
			SCREEN,
			TEXTURE
		};

		IRenderNode() : SceneObject() {};

		IRenderNode(const std::string &name) : SceneObject(name) {};

		virtual ~IRenderNode() = default;

		virtual void draw(glm::mat4 parentModelMatrix) = 0;


		virtual glm::vec3 getPosition() = 0;
		virtual glm::vec3 getRotation() = 0;
		virtual glm::vec3 getScale() = 0;

		virtual const glm::vec3 &getRelativeRotation() = 0;
		virtual const glm::vec3 &getRelativePosition() = 0;
		virtual const glm::vec3 &getRelativeScale() = 0;

		virtual fpgen::generator<std::shared_ptr<IRenderNode>> getAllChildren(std::shared_ptr<IRenderNode> node) = 0;

		virtual const glm::mat4 &getModelMatrix() = 0;

		virtual void setPosition(const glm::vec3 &position) = 0;

		virtual void setRotation(const glm::vec3 &rotation) = 0;


		virtual void setScale(const glm::vec3 &scale) = 0;

		virtual void addChild(std::shared_ptr<IRenderNode> child) = 0;

		virtual void setMesh(std::shared_ptr<IMesh> mesh) = 0;

		virtual void setProgram(std::shared_ptr<IProgram> program) = 0;

		virtual void setCamera(std::shared_ptr<ICamera> camera) = 0;

		virtual void addUniform(std::shared_ptr<IUniform> uniform) = 0;

		virtual void addTexture(std::shared_ptr<ITexture> texture) = 0;

		virtual void addAnimation(std::shared_ptr<IAnimation> animation) = 0;

		virtual void setDrawTarget(DrawTarget drawTarget) = 0;

		virtual void setDrawTexture(std::shared_ptr<ITexture> texture) = 0;

		virtual void setLightingModel(std::shared_ptr<LightingModel> model) = 0;

		virtual void drawAsWireframe(bool enabled) = 0;

		virtual void setTransparent(bool transparent) = 0;

		virtual bool isTransparent() const = 0;

		[[nodiscard]] virtual std::shared_ptr<IMesh> getMesh() const = 0;

		[[nodiscard]] virtual std::vector<std::shared_ptr<IProgram>> getPrograms() const = 0;

		[[nodiscard]] virtual std::shared_ptr<ICamera> getCamera() const = 0;

		[[nodiscard]] virtual std::vector<std::shared_ptr<IUniform>> getUniforms() const = 0;

		[[nodiscard]] virtual std::vector<std::shared_ptr<ITexture>> getTextures() const = 0;

		[[nodiscard]] virtual std::vector<std::shared_ptr<IRenderNode>> getChildren() const = 0;

		[[nodiscard]] virtual std::vector<std::shared_ptr<IAnimation>> getAnimations() const = 0;

		[[nodiscard]] virtual DrawTarget getDrawTarget() const = 0;

		[[nodiscard]] virtual const glm::mat4 &getRenderModelMatrix() = 0;


		virtual void setParent(std::shared_ptr<IRenderNode> parent) = 0;

		virtual std::shared_ptr<IRenderNode> getParent() const = 0;


	private:
		virtual void init() = 0;


	};
}