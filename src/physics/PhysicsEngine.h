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
        PhysicsEngine(float step = 1.0f / 60.0f, QObject *parent = nullptr);

        void addRigidBody(std::shared_ptr<IRigidBody> rigidBody);

        void addCollider(std::shared_ptr<ICollider> collider, std::shared_ptr<IRigidBody> rigidBody);

        void setSimulationStep(float step);

    private:

        void update();

        float step;
        std::shared_ptr<QTimer> timer;
        std::vector<std::shared_ptr<IRigidBody>> bodies;
        PhysicsWorld *world;

    };
}