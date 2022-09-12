//
// Created by tomas on 26.8.22.
//
#include <glbinding/glbinding.h>
#include <glbinding/gl/gl.h>

#include "QRenderer.h"
#include <chrono>
#include <chrono>
#include <thread>
#include <QMouseEvent>
#include <QKeyEvent>

using namespace gl;


namespace Galax::Renderer {
    QRenderer *QRenderer::getProcAddressHelper = nullptr;


    QRenderer::QRenderer(const QSurfaceFormat &format, QScreen *screen): QWindow(screen), context(new QOpenGLContext), repaintTimer(new QBasicTimer) {
        if (!getProcAddressHelper) {
            getProcAddressHelper = this;
        }
        repaintTimer->start(0, this);
        setSurfaceType(OpenGLSurface);
        create();

        initializeInput();
        initializeGL(format);
        SceneObject::init();
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

    void QRenderer::resizeEvent(QResizeEvent *event) {
        scene->setDimensions(width(), height());
        QWindow::resizeEvent(event);

    }

    void QRenderer::paintGL() {

        SceneObject::setFrameTime(frameTime);
        auto frameStart = std::chrono::high_resolution_clock::now();

        context->makeCurrent(this);
        gl::glViewport(0, 0, width(), height());
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if(scene){
            scene->draw();
        }

        context->swapBuffers(this);
        context->doneCurrent();

        auto frameEnd = std::chrono::high_resolution_clock::now();
        frameTime = (double)std::chrono::duration_cast<std::chrono::nanoseconds>(frameEnd - frameStart).count() / 1000000.0;
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
        this->scene = scene;
        scene->setInputHandler(this->input);
        scene->setDimensions(width(), height());
    }

    void QRenderer::initializeInput() {
        input = std::make_shared<InputHandler>();
        connect(this, &QRenderer::mouseMoveEvent, [this](QMouseEvent* event) {
            this->input->mouseMove(event->position().x(), event->position().y());
        });

        connect(this, &QRenderer::mousePressEvent, [this](QMouseEvent* event) {
            this->input->mousePress((InputHandler::MouseButton)event->button(), event->position().x(), event->position().y());
        });

        connect(this, &QRenderer::mouseReleaseEvent, this, [this](QMouseEvent* event) {
            this->input->mouseRelease((InputHandler::MouseButton)event->button(), event->position().x(), event->position().y());
        });

        connect(this, &QRenderer::wheelEvent, [this](QWheelEvent* event) {
            // this->inputHandler->mouseWheel(event->delta()); TODO
        });
        connect(this, &QRenderer::keyPressEvent, this, [&](QKeyEvent* event) {
            this->input->keyPress(InputHandler::fromQtKey(event->key()));
        });

        connect(this, &QRenderer::keyReleaseEvent, [this](QKeyEvent* event) {
            this->input->keyRelease(InputHandler::fromQtKey(event->key())); // TODO check why this is firing
        });

    }

}


