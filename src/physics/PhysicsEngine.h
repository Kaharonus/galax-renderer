//
// Created by tomas on 5.11.22.
//


#pragma once

#include <reactphysics3d/reactphysics3d.h>
#include <physics/interfaces/IRigidBody.h>
#include <QObject>
#include <QTimer>

namespace Galax::Physics {
    using namespace reactphysics3d;

    class PhysicsEngine : public QObject, public PhysicsObject {
        Q_OBJECT

    public:
        PhysicsEngine(std::shared_ptr<InputHandler> inputHandler, float step = 1.0f / 60.0f, QObject *parent = nullptr);

        void addRigidBody(std::shared_ptr<IRigidBody> rigidBody);

        void setCamera(std::shared_ptr<ICamera> camera);

    private:

        void update();

        Ray createRayFromMousePosition();

        float step;
        std::shared_ptr<InputHandler> inputHandler;
        std::shared_ptr<QTimer> timer;
        std::vector<std::shared_ptr<IRigidBody>> bodies;
        std::shared_ptr<ICamera> camera;
        PhysicsWorld *world;

    };
}