
#pragma once
#include "impl/Node.h"
#include "impl/Mesh.h"
#include "impl/Program.h"
#include "impl/Shader.h"
#include "impl/Texture.h"
#include "impl/Camera.h"
#include "input/InputHandler.h"
#include "GBuffer.h"

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

        void setRoot(std::shared_ptr<INode> parent);
        void setInputHandler(std::shared_ptr<InputHandler> inputHandler);
        std::shared_ptr<INode> getRoot();
        void setDimensions(int w, int h);

        void setGBuffer(std::shared_ptr<GBuffer> buffer);

        void build();

        void draw();

    private:
        void addMesh(std::shared_ptr<IMesh> mesh);
        void addProgram(std::shared_ptr<IProgram> program);
        void addShader(std::shared_ptr<IShader> shader);
        void addTexture(std::shared_ptr<ITexture> texture);
        void addCamera(std::shared_ptr<ICamera> camera);

        void buildNode(const INode& node);

        void drawNode(INode& node);

        int width;
        int height;

        std::shared_ptr<InputHandler> inputHandler;

        std::shared_ptr<ICamera> currentCamera;
        glm::mat4 projectionMatrix;
        glm::mat4 viewMatrix;

        std::shared_ptr<INode> root;
        std::set<std::shared_ptr<IMesh>> meshes;
        std::set<std::shared_ptr<ITexture>> textures;
        std::set<std::shared_ptr<IProgram>> programs;
        std::set<std::shared_ptr<IShader>> shaders;
        std::set<std::shared_ptr<ICamera>> cameras;

        std::shared_ptr<GBuffer> gBuffer;
    };

} // namespace MapGenerator::Renderer
