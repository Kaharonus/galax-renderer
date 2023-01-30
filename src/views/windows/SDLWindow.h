//
// Created by tomas on 27.12.22.
//

#pragma once

#include <SDL.h>
#include <thread>
#include <memory>
#include <renderer/Renderer.h>
#include <physics/PhysicsEngine.h>
#include <assets/SceneLoader.h>
#include <views/windows/DebugWindow.h>
#include <views/windows/IWindow.h>
using namespace Galax::Renderer;
using namespace Galax::Assets;
using namespace Galax::Physics;

namespace Galax::Windowing{
    class SDLWindow : public IWindow {


    public:
        SDLWindow(int width, int height, const char* title);
        virtual ~SDLWindow();
        void show(bool blocking) override;
        void hide();
    private:

        std::shared_ptr<Galax::Renderer::Renderer> renderer;
        std::shared_ptr<InputHandler> inputHandler;
        std::shared_ptr<SceneLoader> sceneLoader;
        std::shared_ptr<PhysicsEngine> physicsEngine;
        bool init();
        void loop();
        void cleanup();

        void handleEvents();

        void render();

        const char* title;
        int width;
        int height;
        bool isShowing = false;
        std::thread renderThread;
        SDL_Window* window;
        SDL_GLContext context;
        uint32_t flags;
        bool isFullscreen = false;
        void loadScene(const std::string &filename);
        double frameTime;
        void handleInputEvents(SDL_Event event);
        void handleMouseEvents(SDL_Event event);
        void handleWindowEvents(SDL_Event event);
        void show();
        DebugWindow* renderOptionsWindow;
    };

}
