//
// Created by tomas on 27.12.22.
//
#include <glbinding/gl/gl.h>

#include <Extensions.h>
#include <renderer/Renderer.h>
#include <chrono>
#include <renderer/SceneObject.h>

using namespace Galax::Renderer;
using namespace gl;

Renderer::Renderer() {
}

Renderer::~Renderer() {

}

void Renderer::init(){

	gl::glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	gl::glEnable((gl::GLenum)GL_DEPTH_TEST);
	gl::glEnable((gl::GLenum)GL_CULL_FACE);
	gl::glCullFace((gl::GLenum)GL_BACK);
	gl::glFrontFace((gl::GLenum)GL_CCW);

	this->gBuffer = std::make_shared<GBuffer>();
	this->gBuffer->init();
	SceneObject::init();

}


void Renderer::draw() {
	prepareRender();
	drawOpaqueGeometry();
	drawLighting();
	drawTransparentGeometry();
	drawPostProcess();
	finishRender();
}

void Renderer::resize(int width, int height) {
	this->width = width;
	this->height = height;

	this->requestedResize = true;

}




void Renderer::drawOpaqueGeometry() {
	GL_DEBUG("Geometry o-pass", {
			if (scene) {
				scene->draw();
			}
	});
}

void Renderer::drawTransparentGeometry() {
	GL_DEBUG("Geometry t-pass", {
			if (scene) {
				scene->drawTransparent();
			}
	})
}

void Renderer::drawLighting() {
	GL_DEBUG("Deferred lighting", {
			if (lightingModel) {
				lightingModel->draw();
			}
	})
}

void Renderer::drawPostProcess() {
	GL_DEBUG("Post processing", {
			for (auto [i, effect]: enumerate(postProcesses)) {
				GL_DEBUG(effect->getName().c_str(), {
						effect->draw();
				})
			}
	})
}

void Renderer::finishRender() {
	gl::glBindFramebuffer((gl::GLenum) GL_FRAMEBUFFER, 0);
	gl::glViewport(0, 0, width, height);
}



void Renderer::prepareRender() {
	if(!this->frameTimeSet){
		throw std::runtime_error("You need to set call the setLastFrameTime method before any draw calls. "
								 "Frame time needs to be calculated before binding the context and after swapping buffers.");
	}
	this->frameTimeSet = false;

	if(this->requestedResize){
		input->setRendererSize(width, height);

		gBuffer->resize(width, height);
		if (lightingModel) {
			lightingModel->resize(width, height);

		}
		for (auto &effect: postProcesses) {
			effect->resize(width, height);
		}
		if (scene) {
			scene->setDimensions(width, height);
		}

		requestedResize = false;
	}

	gl::glViewport(0, 0, width, height);
	gl::glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	gl::glClear((gl::ClearBufferMask) GL_COLOR_BUFFER_BIT | (gl::ClearBufferMask) GL_DEPTH_BUFFER_BIT);

	//Bind transparency fbo and clear it as well
	if (scene && scene->getTransparencyBuffer()) {
		scene->getTransparencyBuffer()->bind(false);
		gl::glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		gl::glClear((gl::ClearBufferMask) GL_COLOR_BUFFER_BIT | (gl::ClearBufferMask) GL_DEPTH_BUFFER_BIT);
		scene->getTransparencyBuffer()->unbind();
	}
}

void Renderer::setScene(std::shared_ptr<Scene> scene) {
	scene->setGBuffer(gBuffer);
	scene->build();
	this->scene = scene;
	scene->setInputHandler(this->input);
	scene->setDimensions(width, height);
}

void Renderer::addPostProcess(std::shared_ptr<IPostProcessEffect> postProcess) {
	this->postProcesses.push_back(postProcess);
	auto requested = (int) postProcess->getRequestedGBufferTextures();
	if (requested & (int) IPostProcessEffect::GBufferTexture::ALBEDO) {
		postProcess->addInputTexture(gBuffer->getAlbedoTexture());
	}
	if (requested & (int) IPostProcessEffect::GBufferTexture::NORMAL) {
		postProcess->addInputTexture(gBuffer->getNormalTexture());
	}
	if (requested & (int) IPostProcessEffect::GBufferTexture::POSITION) {
		postProcess->addInputTexture(gBuffer->getPositionTexture());
	}
	if (requested & (int) IPostProcessEffect::GBufferTexture::EMISSION) {
		postProcess->addInputTexture(gBuffer->getEmissionTexture());
	}
	if (requested & (int) IPostProcessEffect::GBufferTexture::METADATA) {
		postProcess->addInputTexture(gBuffer->getMetadataTexture());
	}
}

void Renderer::setLightingModel(std::shared_ptr<LightingModel> lightingModel) {
	this->lightingModel = lightingModel;
	this->lightingModel->addTexture(gBuffer->getAlbedoTexture());
	this->lightingModel->addTexture(gBuffer->getNormalTexture());
	this->lightingModel->addTexture(gBuffer->getPositionTexture());
	this->lightingModel->addTexture(gBuffer->getEmissionTexture());
}

void Renderer::setInputHandler(std::shared_ptr<InputHandler> handler) {
	this->input = handler;
	if(this->scene){
		this->scene->setInputHandler(handler);
	}
}



void Renderer::setLastFrameTime(double frameTime) {
	this->frameTime = frameTime;
	this->frameTimeSet = true;
	SceneObject::setFrameTime(frameTime);
}



