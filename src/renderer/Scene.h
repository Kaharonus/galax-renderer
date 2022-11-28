
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


		void addModel(std::shared_ptr<INode> model);

		void removeModel(std::shared_ptr<INode> model);

		std::vector<std::shared_ptr<INode>> getModels();


        void setInputHandler(std::shared_ptr<InputHandler> inputHandler);
        void setDimensions(int w, int h);

        void setGBuffer(std::shared_ptr<GBuffer> buffer);

		void setLightingModel(std::shared_ptr<LightingModel> mode);

		std::shared_ptr<LightingModel> getLightingModel() const;

		std::shared_ptr<FrameBuffer> getTransparencyBuffer() const;


        void build();

        void draw();

		void drawTransparent();

    private:
        void addMesh(std::shared_ptr<IMesh> mesh);
        void addProgram(std::shared_ptr<IProgram> program);
        void addShader(std::shared_ptr<IShader> shader);
        void addTexture(std::shared_ptr<ITexture> texture);
        void addCamera(std::shared_ptr<ICamera> camera);

        void buildNode(const INode& node);

        int width;
        int height;

        std::shared_ptr<InputHandler> inputHandler;

        std::shared_ptr<ICamera> currentCamera;
        glm::mat4 projectionMatrix;
        glm::mat4 viewMatrix;

		std::shared_ptr<FrameBuffer> transparencyBuffer;

        std::vector<std::shared_ptr<INode>> models;
        std::set<std::shared_ptr<IMesh>> meshes;
        std::set<std::shared_ptr<ITexture>> textures;
        std::set<std::shared_ptr<IProgram>> programs;
        std::set<std::shared_ptr<IShader>> shaders;
        std::set<std::shared_ptr<ICamera>> cameras;

        std::shared_ptr<GBuffer> gBuffer;
		std::shared_ptr<LightingModel> lightingModel;

		void drawNode(INode &node, bool transparencyPass, glm::mat4 modelMatrix);
	};

} // namespace MapGenerator::Renderer
