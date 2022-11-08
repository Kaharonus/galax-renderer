//
// Created by tomas on 5.11.22.
//

#include <physics/PhysicsEngine.h>

using namespace Galax::Physics;

PhysicsEngine::PhysicsEngine(float step, QObject* parent) : QObject(parent), step(step) {
    world = PhysicsObject::createWorld();
    timer = std::make_shared<QTimer>();
    timer->setInterval(this->step * 1000);
    connect(timer.get(), &QTimer::timeout, this, &PhysicsEngine::update);
    timer->start();
}

void PhysicsEngine::addRigidBody(std::shared_ptr<IRigidBody> rigidBody) {
    bodies.push_back(rigidBody);
    rp3d::RigidBody* body = world->createRigidBody(rp3d::Transform());
    body->setType(rp3d::BodyType::DYNAMIC);
    body->setMass(rigidBody->getBodyMass());
    body->setTransform(IRigidBody::toRp3dTransform(rigidBody->getBodyPosition(), rigidBody->getBodyRotation()));
    for(auto& force : rigidBody->getForces()){
        body->applyLocalForceAtCenterOfMass(force);
    }


    rigidBody->setRP3DBody(body);
}




void PhysicsEngine::update() {
    world->update(step);
    for (auto &body : bodies) {
        body->update();
    }
}

void PhysicsEngine::addCollider(std::shared_ptr<ICollider> collider, std::shared_ptr<IRigidBody> rigidBody) {
    auto* body = rigidBody->getRP3DBody();
}
