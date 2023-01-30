//
// Created by tomas on 26.8.22.
//

#include <glbinding/glbinding.h>

#include "QRenderer.h"
#include "Extensions.h"
#include <chrono>
#include <chrono>
#include <thread>
#include <QMouseEvent>
#include <QKeyEvent>


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

	initializeGL(format);
	this->input = inputHandler;
	this->renderer->setInputHandler(input);
	connectInput();

}

void QRenderer::initializeGL(const QSurfaceFormat &format) {
	SceneObject::selectAndLockContext("QRender");
	context->setFormat(format);
	context->create();
	context->makeCurrent(this);
	glbinding::initialize(getProcAddress);
	this->renderer = std::make_shared<Renderer>();
	this->renderer->init();

	context->doneCurrent();
	SceneObject::unlockContext();
}


void QRenderer::paintGL() {
	SceneObject::selectAndLockContext("QRender");
	static int previousHeight = 0;
	static int previousWidth = 0;
	if (previousHeight != height() || previousWidth != width()) {
		renderer->resize(width(), height());
		previousHeight = height();
		previousWidth = width();
	}
	auto frameStart = std::chrono::high_resolution_clock::now();

	context->makeCurrent(this);
	renderer->setLastFrameTime(frameTime);
	renderer->draw();
	context->swapBuffers(this);
	context->doneCurrent();

	auto frameEnd = std::chrono::high_resolution_clock::now();
	this->frameTime = (double) std::chrono::duration_cast<std::chrono::nanoseconds>(frameEnd - frameStart).count() /
					  1000000.0;
	SceneObject::unlockContext();
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



void QRenderer::connectInput() {
	this->setMouseGrabEnabled(true);

	connect(this, &QRenderer::mouseMoveEvent, [this](QMouseEvent *event) {

		this->input->mouseMove(event->pos().x(), event->pos().y());
		event->accept();
	});

	connect(this, &QRenderer::mousePressEvent, [this](QMouseEvent *event) {
		this->input->mousePress((InputHandler::MouseButton) event->button(), event->pos().x(),
								event->pos().y());
		event->accept();
	});

	connect(this, &QRenderer::mouseReleaseEvent, this, [this](QMouseEvent *event) {
		this->input->mouseRelease((InputHandler::MouseButton) event->button(), event->pos().x(),
								  event->pos().y());
	});

	connect(this, &QRenderer::wheelEvent, [this](QWheelEvent *event) {
		this->input->mouseWheel(event->angleDelta().x(), event->angleDelta().y());
		event->accept();

	});
	connect(this, &QRenderer::keyPressEvent, this, [&](QKeyEvent *event) {
		this->input->keyPress(InputHandler::fromQtKey(event->key()));
		event->accept();
	});

	connect(this, &QRenderer::keyReleaseEvent, [this](QKeyEvent *event) {
		this->input->keyRelease(InputHandler::fromQtKey(event->key()));
		event->accept();
	});

}

void QRenderer::setLightingModel(std::shared_ptr<LightingModel> lightingModel) {
	this->renderer->setLightingModel(lightingModel);
}

void QRenderer::addPostProcess(std::shared_ptr<IPostProcessEffect> postProcess) {
	this->renderer->addPostProcess(postProcess);
}

void QRenderer::setScene(std::shared_ptr<Scene> scene) {
	this->renderer->setScene(scene);
}

