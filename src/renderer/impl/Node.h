
#pragma once

#include "../SceneObject.h"
#include "Camera.h"
#include "Mesh.h"
#include "Uniform.h"
#include "Program.h"
#include "Texture.h"
#include "Animation.h"
#include "FrameBuffer.h"
#include "../interfaces/INode.h"


#include <algorithm>
#include <glm/fwd.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <memory>
#include <string>
#include <vector>

namespace Galax::Renderer::SceneObjects {
    class Node : public INode {
    public:
        Node();
        Node(const std::string& name);
        uint getId() override;

        void draw() override;

        ~Node() override;

        void setDrawTarget(DrawTarget target) override;
        void setDrawTexture(std::shared_ptr<ITexture> texture) override;
        void addChild(std::shared_ptr<INode> child) override;
        void removeChild(std::shared_ptr<Node> child);
        void removeAllChildren();
        void setPosition(const glm::vec3& position) override;
        void setRotation(const glm::vec3& rotation) override;
        void setScale(const glm::vec3& scale) override;
        void setMesh(std::shared_ptr<IMesh> mesh) override;

        /// <summary>
        /// Sets all lod levels including the base mesh.
        /// Key in the map is the distance from the camera at which the mesh should be used.
        /// The value with lowest key is the base mesh.
        /// </summary>
        /// \param mesh map of meshes
        void setMeshWithLOD(std::map<float, std::shared_ptr<IMesh>> mesh);

        void addLodLevel(std::shared_ptr<IMesh> mesh, float distance);
        void setProgram(std::shared_ptr<IProgram> program) override;
        void setCamera(std::shared_ptr<ICamera> camera) override;
        void addUniform(std::shared_ptr<IUniform> uniform) override;
        void addTexture(std::shared_ptr<ITexture> texture) override;
        void addAnimation(std::shared_ptr<IAnimation> animation) override;

        glm::mat4& getModelMatrix();
        glm::vec3& getPosition() override;
        glm::vec3& getRotation() override;
        glm::vec3& getScale() override;
        [[nodiscard]] std::vector<std::shared_ptr<INode>> getChildren() const override;
        [[nodiscard]] std::shared_ptr<IMesh> getMesh() const override;
        [[nodiscard]] std::shared_ptr<IProgram> getProgram() const override;
        [[nodiscard]] std::shared_ptr<ICamera> getCamera() const override;
        [[nodiscard]] std::vector<std::shared_ptr<IUniform>> getUniforms() const override;
        [[nodiscard]] std::vector<std::shared_ptr<ITexture>> getTextures() const override;
        [[nodiscard]] DrawTarget getDrawTarget() const override;
        [[nodiscard]] std::vector<std::shared_ptr<IAnimation>> getAnimations() const override;
    private:
        void useCamera();
        void calculateModelMatrix();
        void selectLOD(float distance);


        DrawTarget drawTarget = DrawTarget::SCREEN;
        std::shared_ptr<FrameBuffer> frameBuffer;
        std::shared_ptr<ITexture> drawTexture;

        std::vector<std::shared_ptr<IUniform>> uniforms;
        std::vector<std::shared_ptr<ITexture>> textures;
        std::vector<std::shared_ptr<IAnimation>> animations;

        std::vector<std::shared_ptr<INode>> children;
        std::shared_ptr<IMesh> mesh;
        std::map<float, std::shared_ptr<IMesh>> meshLODs;

        std::shared_ptr<IProgram> program;
        std::shared_ptr<ICamera> camera;


        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 scale;
        glm::mat4 modelMatrix;
        std::shared_ptr<Uniform> modelMatrixUniform;
        std::shared_ptr<Uniform> transposeInverseModelUniform;

        void init();

        float getDistance();

        void updateAnimations();
    };

} // namespace MapGenerator::Renderer::SceneObjects