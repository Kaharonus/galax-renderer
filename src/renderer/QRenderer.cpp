//
// Created by tomas on 26.8.22.
//
#include <glbinding/glbinding.h>
#include <glbinding/gl/gl.h>

#include "QRenderer.h"
#include <Extensions.h>
#include <chrono>
#include <chrono>
#include <thread>
#include <QMouseEvent>
#include <QKeyEvent>

using namespace gl;

using namespace Galax::Renderer;

QRenderer *QRenderer::getProcAddressHelper = nullptr;


QRenderer::QRenderer(std::shared_ptr<InputHandler> inputHandler, const QSurfaceFormat &format, QScreen *screen) :
        QWindow(screen), context(new QOpenGLContext), repaintTimer(new QBasicTimer) {
    if (!getProcAddressHelper) {
        getProcAddressHelper = this;
    }
    repaintTimer->start(0, this);
    setSurfaceType(OpenGLSurface);
    create();

    input = inputHandler;
    connectInput();
    initializeGL(format);

}

void QRenderer::initializeGL(const QSurfaceFormat &format) {
    context->setFormat(format);
    context->create();
    context->makeCurrent(this);
    glbinding::initialize(getProcAddress);
    gl::glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);


    SceneObject::init();
	this->gBuffer = std::make_shared<GBuffer>();
	this->gBuffer->init();


    context->doneCurrent();
}

void QRenderer::resize() {
    viewportWidth = width();
    viewportHeight = height();

	input->setRendererSize(viewportWidth, viewportHeight);

    gBuffer->resize(viewportWidth, viewportHeight);
    lightingModel->resize(viewportWidth, viewportHeight);
    for (auto &effect: postProcesses) {
        effect->resize(viewportWidth, viewportHeight);
    }
    scene->setDimensions(viewportWidth, viewportHeight);
}


void QRenderer::prepareRender() {
    static int previousHeight = 0;
    static int previousWidth = 0;

    context->makeCurrent(this);
    if (previousHeight != height() || previousWidth != width()) {
        resize();
        previousHeight = viewportHeight;
        previousWidth = viewportWidth;
    }

    gl::glViewport(0, 0, viewportWidth, viewportHeight);
    gl::glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    gl::glClear((gl::ClearBufferMask) GL_COLOR_BUFFER_BIT | (gl::ClearBufferMask) GL_DEPTH_BUFFER_BIT);

	//Bind transparency fbo and clear it as well
	if(scene->getTransparencyBuffer()){
		scene->getTransparencyBuffer()->bind(false);
		gl::glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		gl::glClear((gl::ClearBufferMask) GL_COLOR_BUFFER_BIT | (gl::ClearBufferMask) GL_DEPTH_BUFFER_BIT);
		scene->getTransparencyBuffer()->unbind();
	}
}

void QRenderer::drawOpaqueGeometry() {
    GL_DEBUG("Geometry o-pass", {
        if (scene) {
            scene->draw();
        }
    });
}

void QRenderer::drawTransparentGeometry() {
	GL_DEBUG("Geometry t-pass", {
		if (scene) {
			scene->drawTransparent();
		}
	})
}

void QRenderer::drawLighting() {
    GL_DEBUG("Deferred lighting", {
        lightingModel->draw();
    })
}

void QRenderer::drawPostProcess() {
    GL_DEBUG("Post processing", {
        for (auto [i, effect]: enumerate(postProcesses)) {
            GL_DEBUG(effect->getName().c_str(), {
                effect->draw();
            })
        }
    })
}

void QRenderer::finishRender() {
	gl::glBindFramebuffer((gl::GLenum)GL_FRAMEBUFFER, 0);
	gl::glViewport(0, 0, viewportWidth, viewportHeight);
	gl::glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	//gl::glClear((gl::ClearBufferMask) GL_COLOR_BUFFER_BIT | (gl::ClearBufferMask) GL_DEPTH_BUFFER_BIT);

	context->swapBuffers(this);
	context->doneCurrent();
}

void QRenderer::paintGL() {
    //Set stuff for all objects
    SceneObject::setFrameTime(frameTime);
    auto frameStart = std::chrono::high_resolution_clock::now();

    prepareRender();

	drawOpaqueGeometry();

    drawLighting();

	drawTransparentGeometry();

    drawPostProcess();

    finishRender();


    auto frameEnd = std::chrono::high_resolution_clock::now();
    frameTime = (double) std::chrono::duration_cast<std::chrono::nanoseconds>(frameEnd - frameStart).count() /
                1000000.0;
}

void QRenderer::timerEvent(QTimerEvent *event) {
    paintGL();
}

QRenderer::~QRenderer() {
    //context->deleteLater();
}

ProcAddress QRenderer::getProcAddress(const char *name) {
    if (!getProcAddressHelper || name == nullptr) {
        return nullptr;
    }
    const auto symbol = std::string(name);
    const auto qtSymbol = QByteArray::fromStdString(symbol);
    return getProcAddressHelper->context->getProcAddress(qtSymbol);
}

void QRenderer::setVerticalSync(QRenderer::VerticalSync sync) {
    context->makeCurrent(this);
    this->sync = sync;
    bool result = false;
    typedef int(APIENTRY *SWAPINTERVALEXTPROC)(int);
    static SWAPINTERVALEXTPROC glXSwapIntervalSGI = nullptr;
    if (!glXSwapIntervalSGI)
        glXSwapIntervalSGI = reinterpret_cast<SWAPINTERVALEXTPROC>(context->getProcAddress("glXSwapIntervalSGI"));
    if (glXSwapIntervalSGI)
        result = glXSwapIntervalSGI(sync);
    context->doneCurrent();

}

double QRenderer::getFrameTime() const {
    return frameTime;
}

void QRenderer::setScene(std::shared_ptr<Scene> scene) {
	scene->setGBuffer(gBuffer);
	scene->build();
    this->scene = scene;
    scene->setInputHandler(this->input);
    scene->setDimensions(width(), height());
}

void QRenderer::connectInput() {
    connect(this, &QRenderer::mouseMoveEvent, [this](QMouseEvent *event) {
        this->input->mouseMove(event->position().x(), event->position().y());

    });

    connect(this, &QRenderer::mousePressEvent, [this](QMouseEvent *event) {
        this->input->mousePress((InputHandler::MouseButton) event->button(), event->position().x(),
                                event->position().y());
    });

    connect(this, &QRenderer::mouseReleaseEvent, this, [this](QMouseEvent *event) {
        this->input->mouseRelease((InputHandler::MouseButton) event->button(), event->position().x(),
                                  event->position().y());
    });

    connect(this, &QRenderer::wheelEvent, [this](QWheelEvent *event) {
         this->input->mouseWheel(event->angleDelta().x(), event->angleDelta().y());

    });
    connect(this, &QRenderer::keyPressEvent, this, [&](QKeyEvent *event) {
        this->input->keyPress(InputHandler::fromQtKey(event->key()));
    });

    connect(this, &QRenderer::keyReleaseEvent, [this](QKeyEvent *event) {
        this->input->keyRelease(InputHandler::fromQtKey(event->key())); // TODO check why this is firing
    });

}

void QRenderer::setLightingModel(std::shared_ptr<LightingModel> lightingModel) {
    this->lightingModel = lightingModel;


    this->lightingModel->addTexture(gBuffer->getAlbedoTexture());
    this->lightingModel->addTexture(gBuffer->getNormalTexture());
    this->lightingModel->addTexture(gBuffer->getPositionTexture());
    this->lightingModel->addTexture(gBuffer->getEmissionTexture());
}

void QRenderer::addPostProcess(std::shared_ptr<IPostProcessEffect> postProcess) {
    this->postProcesses.push_back(postProcess);
    auto requested = (int)postProcess->getRequestedGBufferTextures();
    if(requested & (int)IPostProcessEffect::GBufferTexture::ALBEDO) {
        postProcess->addInputTexture(gBuffer->getAlbedoTexture());
    }
    if(requested & (int)IPostProcessEffect::GBufferTexture::NORMAL) {
        postProcess->addInputTexture(gBuffer->getNormalTexture());
    }
    if(requested & (int)IPostProcessEffect::GBufferTexture::POSITION) {
        postProcess->addInputTexture(gBuffer->getPositionTexture());
    }
    if(requested & (int)IPostProcessEffect::GBufferTexture::EMISSION) {
        postProcess->addInputTexture(gBuffer->getEmissionTexture());
    }
    if(requested & (int)IPostProcessEffect::GBufferTexture::METADATA) {
        postProcess->addInputTexture(gBuffer->getMetadataTexture());
    }
}



