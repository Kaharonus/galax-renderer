//
// Created by tomas on 27.12.22.
//

#include <glbinding/glbinding.h>
#include <glbinding/gl/gl.h>


#include <orbital/SolarSystem.h>
#include <generators/SolarSystemGenerator.h>
#include <SDL_events.h>

#include "SDLWindow.h"
#include "DebugWindow.h"

using namespace Galax::Windowing;


SDLWindow::SDLWindow(int width, int height, const char *title) : IWindow() {
	this->width = width;
	this->height = height;
	this->title = title;

	this->sceneLoader = std::make_shared<SceneLoader>("assets.data", "scenes");
	this->sceneLoader->registerGenerator(std::make_shared<Galax::Generators::SolarSystemGenerator>());
}


SDLWindow::~SDLWindow() {
	this->renderThread.join();
}

void SDLWindow::handleInputEvents(SDL_Event event){
	switch(event.type){
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_F11) {
				isFullscreen = !isFullscreen;
			}
			this->inputHandler->keyPress(this->inputHandler->fromSDLKey(event.key.keysym.sym));
			break;
		case SDL_KEYUP:
			this->inputHandler->keyRelease(this->inputHandler->fromSDLKey(event.key.keysym.sym));
			break;
	}
}

void SDLWindow::handleMouseEvents(SDL_Event event){
	static auto initialMouse = glm::vec2(0);
	static auto delta = glm::vec2(0);
	switch (event.type){
		case SDL_MOUSEMOTION:
			if(this->inputHandler->isMouseButtonPressed(InputHandler::MOUSE_LEFT)){
				delta -= (glm::vec2(initialMouse.x - event.motion.x, initialMouse.y - event.motion.y) * 0.1f);
				SDL_WarpMouseInWindow(window, initialMouse.x,initialMouse.y);
				this->inputHandler->mouseMove(initialMouse.x + delta.x, initialMouse.y + delta.y);
				break;
			}
			this->inputHandler->mouseMove(event.motion.x, event.motion.y);
			break;
		case SDL_MOUSEBUTTONDOWN:
			initialMouse = glm::vec2(event.button.x,event.button.y);
			this->inputHandler->mousePress((InputHandler::MouseButton) event.button.button, event.button.x,event.button.y);
			SDL_SetRelativeMouseMode(SDL_TRUE);
			break;
		case SDL_MOUSEBUTTONUP:
			this->inputHandler->mouseRelease((InputHandler::MouseButton) event.button.button, event.button.x,event.button.y);
			SDL_SetRelativeMouseMode(SDL_FALSE);
			delta = glm::vec2(0);
			break;
		case SDL_MOUSEWHEEL:
			this->inputHandler->mouseWheel(event.wheel.x*240, event.wheel.y*240);
			break;
	}
}

void SDLWindow::handleWindowEvents(SDL_Event event){
	switch (event.window.event) {
		case SDL_WINDOWEVENT_CLOSE:
			isShowing = false;
		case SDL_WINDOWEVENT_MAXIMIZED:
			isFullscreen = true;
			break;
		case SDL_WINDOWEVENT_RESIZED:
			this->width = event.window.data1;
			this->height = event.window.data2;
			renderer->resize(width, height);

            break;
	}

}

void SDLWindow::handleEvents() {
	SDL_Event event;
	auto fullScreen = isFullscreen;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_KEYDOWN:
			case SDL_KEYUP:
				handleInputEvents(event);
				break;
			case SDL_MOUSEMOTION:
			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
			case SDL_MOUSEWHEEL:
				handleMouseEvents(event);
                break;
			case SDL_WINDOWEVENT:
				handleWindowEvents(event);
				break;
			case SDL_QUIT:
                renderOptionsWindow->close();
				isShowing = false;
				break;
			default:
				break;
		}
	}
	if (isFullscreen != fullScreen) {
        SDL_SetWindowFullscreen(window, flags |(SDL_WINDOW_FULLSCREEN_DESKTOP & isFullscreen));
        if(isFullscreen){
            //If I dont do this under x11 on manjaro with the current update its fucked.. with NV drivers..
            auto index = SDL_GetWindowDisplayIndex(window);
            SDL_DisplayMode mode;
            SDL_GetDesktopDisplayMode(index, &mode);
            SDL_SetWindowDisplayMode(window, &mode);
        }
    }

}


void SDLWindow::loop() {
	while (isShowing) {
		handleEvents();
		physicsEngine->update();
		render();
	}
}

bool SDLWindow::init() {
	SceneObject::addUniqueContext("SDLWindow");
	flags = SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
	window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
	if (window == nullptr) {
		return false;
	}

	SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	context = SDL_GL_CreateContext(window);

	glbinding::initialize([](const char *name) {
		return reinterpret_cast<glbinding::ProcAddress>(SDL_GL_GetProcAddress(name));
	}, false);

	SDL_GL_MakeCurrent(window, context);
	SceneObject::selectAndLockContext("SDLWindow");
	this->renderer = std::make_shared<Galax::Renderer::Renderer>();
	this->renderer->init();

	this->inputHandler = std::make_shared<InputHandler>();
	this->renderer->setInputHandler(this->inputHandler);

	this->physicsEngine = std::make_shared<PhysicsEngine>(inputHandler, 15);

	SceneObject::unlockContext();
	SDL_GL_SwapWindow(window);
	return true;
}

void SDLWindow::render() {
	auto frameStart = std::chrono::high_resolution_clock::now();
	SceneObject::selectAndLockContext("SDLWindow");
	SDL_GL_MakeCurrent(window, context);

	renderer->setLastFrameTime(frameTime);
	renderer->draw();
	SDL_GL_SwapWindow(window);
	SceneObject::unlockContext();

	auto frameEnd = std::chrono::high_resolution_clock::now();
	this->frameTime = (double) std::chrono::duration_cast<std::chrono::nanoseconds>(frameEnd - frameStart).count() /
					  1000000.0;

}


void SDLWindow::cleanup() {
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
}


void SDLWindow::show(bool blocking) {
	if (isShowing) {
		return;
	}
	this->renderOptionsWindow = new DebugWindow(nullptr);
	this->renderOptionsWindow->resize(1280, 720);
	this->renderOptionsWindow->show();
	isShowing = true;
	if (blocking) {
		this->show();
	} else {
		this->renderThread = std::thread([&]() {
			this->show();
		});
	}

}

void SDLWindow::show(){


	this->init();
	loadScene("scenes/solar-system.json");
	this->renderer->resize(width, height);
	this->loop();
	this->cleanup();
}


void SDLWindow::hide() {
	isShowing = false;
}


void SDLWindow::loadScene(const std::string &filename) {
	SceneObject::selectAndLockContext("SDLWindow");
	auto sceneData = this->sceneLoader->loadScene(filename);
	if (!sceneData) {
		SceneObject::unlockContext();
		return;
	}
	auto scene = sceneData->scene;
	auto effects = sceneData->postProcessEffects;

	renderer->setScene(scene);
	renderer->setLightingModel(scene->getLightingModel());
	for (auto &effect: sceneData->postProcessEffects) {
		renderer->addPostProcess(effect);
	}
	renderOptionsWindow->setScene(scene, scene->getLightingModel(), effects);
	physicsEngine->setScene(scene);

	SceneObject::unlockContext();
}