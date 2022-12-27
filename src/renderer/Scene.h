
#pragma once

#include "impl/RenderNode.h"
#include "impl/Mesh.h"
#include "impl/Program.h"
#include "impl/Shader.h"
#include "impl/Texture.h"
#include "impl/Camera.h"
#include "input/InputHandler.h"
#include "GBuffer.h"
#include "LightingModel.h"

#undef foreach
#include <fpgen/fpgen.hpp>


#include <iterator>
#include <iostream>
#include <coroutine>
#include <set>
#include <memory>
#include <vector>
#include <Extensions.h>

using namespace Galax::Renderer::SceneObjects;


namespace Galax::Renderer {

	class Scene {
	public:

		Scene();

		~Scene();


		virtual void addModel(std::shared_ptr<IRenderNode> model);

		virtual void removeModel(std::shared_ptr<IRenderNode> model);

		virtual std::vector<std::shared_ptr<IRenderNode>> getModels();


		virtual void setInputHandler(std::shared_ptr<InputHandler> inputHandler);

		virtual void setDimensions(int w, int h);

		virtual void setGBuffer(std::shared_ptr<GBuffer> buffer);

		virtual void setLightingModel(std::shared_ptr<LightingModel> mode);

		[[nodiscard]] virtual std::shared_ptr<LightingModel> getLightingModel() const;

		[[nodiscard]] virtual std::shared_ptr<FrameBuffer> getTransparencyBuffer() const;

		virtual void build();

		virtual void draw();

		virtual void drawTransparent();


		fpgen::generator<std::shared_ptr<IRenderNode>> getAllNodes();

	protected:
		void addCamera(std::shared_ptr<ICamera> camera);

		virtual void buildNode(IRenderNode &node);

		virtual void drawNode(IRenderNode &node, bool transparencyPass, glm::mat4 modelMatrix);

		int width;
		int height;

		std::shared_ptr<InputHandler> inputHandler;

		std::shared_ptr<ICamera> currentCamera;
		glm::mat4 projectionMatrix;
		glm::mat4 viewMatrix;

		std::shared_ptr<FrameBuffer> transparencyBuffer;

		std::vector<std::shared_ptr<IRenderNode>> models;
		std::set<std::shared_ptr<ICamera>> cameras;

		std::shared_ptr<GBuffer> gBuffer;
		std::shared_ptr<LightingModel> lightingModel;

	};

} // namespace MapGenerator::Renderer
