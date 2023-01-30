//
// Created by tomas on 5.11.22.
//


#pragma once

#include <reactphysics3d/reactphysics3d.h>
#include <renderer/Scene.h>
#include <physics/interfaces/IRigidBody.h>
#include <QObject>
#include <QTimer>
#include <physics/Timer.h>

namespace Galax::Physics {
    using namespace reactphysics3d;
	using namespace Galax::Renderer;

    class PhysicsEngine : public PhysicsObject {

    public:
        PhysicsEngine(std::shared_ptr<InputHandler> inputHandler, long step = 16);
		void setScene(std::shared_ptr<Scene> scene);
		void update();

    protected:
        Ray createRayFromMousePosition();

		void setCamera(std::shared_ptr<ICamera> camera);

        long step;
        std::shared_ptr<InputHandler> inputHandler;
        std::shared_ptr<Timer> timer;

		std::shared_ptr<Scene> scene;

        std::shared_ptr<ICamera> camera;
        PhysicsWorld *world;

		void createRigidBody(std::shared_ptr<IRigidBody> rigidBody);
	};
}