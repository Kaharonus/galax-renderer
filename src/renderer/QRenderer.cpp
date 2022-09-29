//
// Created by tomas on 26.8.22.
//
#include <glbinding/glbinding.h>
#include <glbinding/gl/gl.h>

#include "QRenderer.h"
#include "../Extensions.h"
#include <chrono>
#include <chrono>
#include <thread>
#include <QMouseEvent>
#include <QKeyEvent>

using namespace gl;

using namespace Galax::Renderer;

QRenderer *QRenderer::getProcAddressHelper = nullptr;


QRenderer::QRenderer(const QSurfaceFormat &format, QScreen *screen) : QWindow(screen), context(new QOpenGLContext),
                                                                      repaintTimer(new QBasicTimer) {
    if (!getProcAddressHelper) {
        getProcAddressHelper = this;
    }
    repaintTimer->start(0, this);
    setSurfaceType(OpenGLSurface);
    create();

    initializeInput();
    initializeGL(format);
    SceneObject::init();
    this->gBuffer = std::make_shared<GBuffer>();
    this->gBuffer->init();


    //setVerticalSync(VerticalSync::Disabled);

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
}

void QRenderer::resize() {
    viewportWidth = width();
    viewportHeight = height();
    gBuffer->resize(viewportWidth, viewportHeight);
    lightingModel->resize(viewportWidth, viewportHeight);
    for (auto &effect: postProcesses) {
        effect->resize(viewportWidth, viewportHeight);
    }
    scene->setDimensions(viewportWidth, viewportHeight);
}


void QRenderer::paintGL() {
    static int previousHeight = 0;
    static int previousWidth = 0;


    SceneObject::setFrameTime(frameTime);
    auto frameStart = std::chrono::high_resolution_clock::now();

    context->makeCurrent(this);

    if (previousHeight != height() || previousWidth != width()) {
        resize();
        previousHeight = viewportHeight;
        previousWidth = viewportWidth;
        gl::glViewport(0, 0, viewportWidth, viewportHeight);
    }

    gl::glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    gl::glClear((gl::ClearBufferMask) GL_COLOR_BUFFER_BIT | (gl::ClearBufferMask) GL_DEPTH_BUFFER_BIT);

    if (scene) {
        scene->draw();
    }

    lightingModel->draw();

    for(auto [i,effect] : enumerate(postProcesses)) {
        effect->render();
    }

    //copyLastFBOToScreen();
    context->swapBuffers(this);
    context->doneCurrent();
    auto frameEnd = std::chrono::high_resolution_clock::now();
    frameTime = (double) std::chrono::duration_cast<std::chrono::nanoseconds>(frameEnd - frameStart).count() /
                1000000.0;
}

void QRenderer::copyLastFBOToScreen() {
    gl::glBindFramebuffer((gl::GLenum) GL_READ_FRAMEBUFFER, SceneObject::getLastFrameBuffer());
    gl::glBindFramebuffer((gl::GLenum) GL_DRAW_FRAMEBUFFER, 0);
    gl::glBlitFramebuffer(0, 0, this->viewportWidth, this->viewportHeight, 0, 0, this->viewportWidth,
                          this->viewportHeight, (gl::ClearBufferMask) GL_DEPTH_BUFFER_BIT, (gl::GLenum) GL_NEAREST);
    gl::glBindFramebuffer((gl::GLenum) GL_FRAMEBUFFER, 0);
}

void QRenderer::timerEvent(QTimerEvent *event) {
    paintGL();
}

QRenderer::~QRenderer() {

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
    scene->build();
    scene->setGBuffer(gBuffer);
    this->scene = scene;
    scene->setInputHandler(this->input);
    scene->setDimensions(width(), height());
}

void QRenderer::initializeInput() {
    input = std::make_shared<InputHandler>();
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
        // this->inputHandler->mouseWheel(event->delta()); TODO
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
}



