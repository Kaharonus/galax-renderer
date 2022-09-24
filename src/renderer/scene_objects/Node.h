
#pragma once

#include "../SceneObject.h"
#include "Camera.h"
#include "Mesh.h"
#include "Uniform.h"
#include "Material.h"
#include "Program.h"
#include "Texture.h"
#include "Animation.h"
#include "FrameBuffer.h"


#include <algorithm>
#include <glm/fwd.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <memory>
#include <string>
#include <vector>

namespace Galax::Renderer::SceneObjects {
    class Node : public SceneObject {
    public:
        enum class DrawTarget{
            SCREEN,
            TEXTURE
        };


        Node();
        Node(const std::string& name);
        Node(const std::string& name, std::shared_ptr<Node> parent);
        uint getId() override;

        void draw();

        ~Node();

        void setDrawTarget(DrawTarget target);
        void setDrawTexture(std::shared_ptr<Texture> texture);
        void addChild(std::shared_ptr<Node> child);
        void removeChild(std::shared_ptr<Node> child);
        void removeAllChildren();
        void setParent(std::shared_ptr<Node> parent);
        void setPosition(const glm::vec3& position);
        void setRotation(const glm::vec3& rotation);
        void setScale(const glm::vec3& scale);
        void setMesh(std::shared_ptr<Mesh> mesh);

        /// <summary>
        /// Sets all lod levels including the base mesh.
        /// Key in the map is the distance from the camera at which the mesh should be used.
        /// The value with lowest key is the base mesh.
        /// </summary>
        /// \param mesh map of meshes
        void setMeshWithLOD(std::map<float, std::shared_ptr<Mesh>> mesh);

        void addLodLevel(std::shared_ptr<Mesh> mesh, float distance);
        void setMaterial(std::shared_ptr<Material> material);
        void setProgram(std::shared_ptr<Program> program);
        void setCamera(std::shared_ptr<Camera> camera);
        void addUniform(std::shared_ptr<Uniform> uniform);
        void addTexture(std::shared_ptr<Texture> texture);
        void addAnimation(std::shared_ptr<Animation> animation);

        glm::mat4& getModelMatrix();
        glm::vec3& getPosition();
        glm::vec3& getRotation();
        glm::vec3& getScale();
        std::shared_ptr<Node> getParent() const;
        std::vector<std::shared_ptr<Node>> getChildren() const;
        std::shared_ptr<Mesh> getMesh() const;
        std::shared_ptr<Material> getMaterial() const;
        std::shared_ptr<Program> getProgram() const;
        std::shared_ptr<Camera> getCamera() const;
        std::vector<std::shared_ptr<Uniform>> getUniforms() const;
        std::vector<std::shared_ptr<Texture>> getTextures() const;
        DrawTarget getDrawTarget() const;

    private:
        void useCamera();
        void calculateModelMatrix();
        void selectLOD(float distance);


        DrawTarget drawTarget = DrawTarget::SCREEN;
        std::shared_ptr<FrameBuffer> frameBuffer;
        std::shared_ptr<Texture> drawTexture;

        std::vector<std::shared_ptr<Uniform>> uniforms;
        std::vector<std::shared_ptr<Texture>> textures;
        std::vector<std::shared_ptr<Animation>> animations;

        std::vector<std::shared_ptr<Node>> children;
        std::shared_ptr<Node> parent;
        std::shared_ptr<Mesh> mesh;
        std::map<float, std::shared_ptr<Mesh>> meshLODs;

        std::shared_ptr<Material> material;
        std::shared_ptr<Program> program;
        std::shared_ptr<Camera> camera;


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
