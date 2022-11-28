#include <glbinding/gl/gl.h>

#include <renderer/Scene.h>
#include <Extensions.h>

using namespace gl;
using namespace Galax::Renderer;

Scene::Scene() {
}

Scene::~Scene() {
}

void Scene::setInputHandler(std::shared_ptr<InputHandler> inputHandler) {
    this->inputHandler = inputHandler;
    for (auto c: this->cameras) {
        c->setInputHandler(inputHandler);
    }
}

void Scene::addMesh(std::shared_ptr<IMesh> mesh) {
    this->meshes.insert(mesh);
}

void Scene::addProgram(std::shared_ptr<IProgram> program) {
    this->programs.insert(program);
}

void Scene::addShader(std::shared_ptr<IShader> shader) {
    this->shaders.insert(shader);
}

void Scene::addTexture(std::shared_ptr<ITexture> texture) {
    this->textures.insert(texture);
}

void Scene::addCamera(std::shared_ptr<ICamera> camera) {
    this->cameras.insert(camera);
}

void Scene::buildNode(const INode &node) {
    if (node.getMesh() != nullptr) {
        addMesh(node.getMesh());
    }
    auto programs = node.getPrograms();
    if (!programs.empty()) {
        for(auto p: programs) {
            for (auto& shader: p->getShaders()) {
                addShader(shader);
            }
            addProgram(p);
        }
    }

    if (node.getCamera() != nullptr) {
        auto camera = node.getCamera();
        camera->setInputHandler(inputHandler);
        addCamera(camera);
    }
    for (auto& child: node.getChildren()) {
        buildNode(*child);
    }
}

void Scene::build() {
	for (auto& model: models) {
		buildNode(*model);
	}
}

void Scene::drawNode(INode &node, bool transparencyPass, glm::mat4 modelMatrix) {
	if(node.isTransparent() != transparencyPass){
		for (auto& child: node.getChildren()) {
			drawNode(*child, transparencyPass, modelMatrix * node.getModelMatrix());
		}
		return;
	}
	GL_DEBUG(node.getName().c_str(), {
		node.draw(modelMatrix);
	})
	for (auto& child: node.getChildren()) {
		drawNode(*child, transparencyPass, modelMatrix * node.getModelMatrix());
	}
}


void Scene::setDimensions(int w, int h) {
    this->width = w;
    this->height = h;
    for (const auto &camera: this->cameras) {
		camera->setResolution(width, height);
    }
	if(transparencyBuffer){
		this->transparencyBuffer->resize(width, height);
	}

}

void Scene::setGBuffer(std::shared_ptr<GBuffer> buffer) {
    this->gBuffer = buffer;
}

void Scene::draw() {
	assert(gBuffer != nullptr);

	for (auto& camera: this->cameras) {
		camera->update();
	}


	gBuffer->bind();

	for(auto& model: models) {
		drawNode(*model, false, glm::mat4(1.0f));
	}
	gBuffer->unbind();
}

void Scene::drawTransparent() {

	if(!transparencyBuffer){
		this->transparencyBuffer = std::make_shared<FrameBuffer>();
		this->transparencyBuffer->addOutputTexture(lightingModel->getTextures()[0]);
		this->transparencyBuffer->resize(width, height);
	}

	//Copy the depth buffer from the gBuffer to the transparency buffer
	transparencyBuffer->copyFrom(gBuffer->getId());
	transparencyBuffer->bind(false);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	for(auto& model: models) {
		drawNode(*model, true, glm::mat4(1.0f));
	}
	glDisable(GL_BLEND);
}

void Scene::addModel(std::shared_ptr<INode> model) {
	this->models.push_back(model);
}

void Scene::removeModel(std::shared_ptr<INode> model) {
	this->models.erase(std::remove(this->models.begin(), this->models.end(), model), this->models.end());
}

std::vector<std::shared_ptr<INode>> Scene::getModels() {
	return this->models;
}

std::shared_ptr<LightingModel> Scene::getLightingModel() const {
	return lightingModel;
}

void Scene::setLightingModel(std::shared_ptr<LightingModel> model) {
	this->lightingModel = model;
}

std::shared_ptr<FrameBuffer> Scene::getTransparencyBuffer() const {
	return transparencyBuffer;
}
