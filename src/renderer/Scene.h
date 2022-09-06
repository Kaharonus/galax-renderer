
#pragma once
#include "scene_objects/Node.h"
#include "scene_objects/Mesh.h"
#include "scene_objects/Program.h"
#include "scene_objects/Shader.h"
#include "scene_objects/Texture.h"
#include "scene_objects/Camera.h"
#include "input/InputHandler.h"

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

        void setRoot(std::shared_ptr<Node> parent);
        void setInputHandler(std::shared_ptr<InputHandler> inputHandler);
        std::shared_ptr<Node> getRoot();
        void setDimensions(int width, int height);
        void build();

        void draw();

    private:
        void addMesh(std::shared_ptr<Mesh> mesh);
        void addProgram(std::shared_ptr<Program> program);
        void addShader(std::shared_ptr<Shader> shader);
        void addTexture(std::shared_ptr<Texture> texture);
        void addMaterial(std::shared_ptr<Material> material);
        void addCamera(std::shared_ptr<Camera> camera);

        void buildNode(const Node& node);

        void drawNode(Node& node);

        std::shared_ptr<InputHandler> inputHandler;

        std::shared_ptr<Camera> currentCamera;
        glm::mat4 projectionMatrix;
        glm::mat4 viewMatrix;

        std::shared_ptr<Node> root;
        std::set<std::shared_ptr<Mesh>> meshes;
        std::set<std::shared_ptr<Texture>> textures;
        std::set<std::shared_ptr<Program>> programs;
        std::set<std::shared_ptr<Shader>> shaders;
        std::set<std::shared_ptr<Material>> materials;
        std::set<std::shared_ptr<Camera>> cameras;
    };

} // namespace MapGenerator::Renderer
