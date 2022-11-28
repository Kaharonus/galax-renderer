
#pragma once
#include "impl/Node.h"
#include "impl/Mesh.h"
#include "impl/Program.h"
#include "impl/Shader.h"
#include "impl/Texture.h"
#include "impl/Camera.h"
#include "input/InputHandler.h"
#include "GBuffer.h"
#include "LightingModel.h"

#include <iostream>
#include <set>
#include <memory>
#include <vector>

using namespace Galax::Renderer::SceneObjects;


namespace Galax::Renderer {

    class Scene  {
    public:
        Scene();
         ~Scene();


		void addModel(std::shared_ptr<IRenderNode> model);

		void removeModel(std::shared_ptr<IRenderNode> model);

		std::vector<std::shared_ptr<IRenderNode>> getModels();


        void setInputHandler(std::shared_ptr<InputHandler> inputHandler);
        void setDimensions(int w, int h);

        void setGBuffer(std::shared_ptr<GBuffer> buffer);

		void setLightingModel(std::shared_ptr<LightingModel> mode);

		[[nodiscard]] std::shared_ptr<LightingModel> getLightingModel() const;

		[[nodiscard]] std::shared_ptr<FrameBuffer> getTransparencyBuffer() const;


        void build();

        void draw();

		void drawTransparent();

    private:
        void addCamera(std::shared_ptr<ICamera> camera);

        void buildNode(IRenderNode &node);

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

		void drawNode(IRenderNode &node, bool transparencyPass, glm::mat4 modelMatrix);
	};

} // namespace MapGenerator::Renderer
