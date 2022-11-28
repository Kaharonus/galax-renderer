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


void Scene::addCamera(std::shared_ptr<ICamera> camera) {
	this->cameras.insert(camera);
}

void Scene::buildNode(IRenderNode& node) {

	if (node.getCamera() != nullptr) {
		auto camera = node.getCamera();
		camera->setInputHandler(inputHandler);
		addCamera(camera);
	}
	node.setLightingModel(lightingModel);
	for (auto &child: node.getChildren()) {
		buildNode(*child);
	}
}

void Scene::build() {
	for (auto &model: models) {
		buildNode(*model);
	}
}

void Scene::drawNode(IRenderNode &node, bool transparencyPass, glm::mat4 modelMatrix) {
	if (node.isTransparent() == transparencyPass) {
		GL_DEBUG(node.getName().c_str(), {
			node.draw(modelMatrix);
		})
	}
	for (auto &child: node.getChildren()) {
		drawNode(*child, transparencyPass, modelMatrix * node.getModelMatrix());
	}
}


void Scene::setDimensions(int w, int h) {
	this->width = w;
	this->height = h;
	for (const auto &camera: this->cameras) {
		camera->setResolution(width, height);
	}
	if (transparencyBuffer) {
		this->transparencyBuffer->resize(width, height);
	}

}

void Scene::setGBuffer(std::shared_ptr<GBuffer> buffer) {
	this->gBuffer = buffer;
}

void Scene::draw() {
	assert(gBuffer != nullptr);

	for (auto &camera: this->cameras) {
		camera->update();
	}


	gBuffer->bind();

	for (auto &model: models) {
		drawNode(*model, false, glm::mat4(1.0f));
	}
	gBuffer->unbind();
}

void Scene::drawTransparent() {

	// Let's use existing buffer for this
	auto lightBuffer = lightingModel->getFrameBuffer();
	// And let's copy the depth from gBuffer, so that we can actually render something
	gBuffer->getFrameBuffer()->copyTo(lightBuffer->getId());
	// Do NOT clear the buffer (we need both depth and color)
	lightBuffer->bind(false);

	//Make sure blending is enabled
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	for (auto &model: models) {
		drawNode(*model, true, glm::mat4(1.0f));
	}
	glDisable(GL_BLEND);


	lightBuffer->unbind();
}

void Scene::addModel(std::shared_ptr<IRenderNode> model) {
	this->models.push_back(model);
}

void Scene::removeModel(std::shared_ptr<IRenderNode> model) {
	this->models.erase(std::remove(this->models.begin(), this->models.end(), model), this->models.end());
}

std::vector<std::shared_ptr<IRenderNode>> Scene::getModels() {
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
