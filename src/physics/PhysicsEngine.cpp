//
// Created by tomas on 5.11.22.
//

#include <physics/PhysicsEngine.h>

using namespace Galax::Physics;

PhysicsEngine::PhysicsEngine(std::shared_ptr<InputHandler> handler, float step, QObject *parent) : QObject(parent),
                                                                                                   step(step) {
    world = PhysicsObject::createWorld();
    world->setIsGravityEnabled(false);
    world->enableSleeping(false);
    timer = std::make_shared<QTimer>();
    timer->setInterval(10);
    connect(timer.get(), &QTimer::timeout, this, &PhysicsEngine::update);
    timer->start();
    this->inputHandler = handler;
}

void PhysicsEngine::addRigidBody(std::shared_ptr<IRigidBody> rigidBody) {
    bodies.push_back(rigidBody);
    rp3d::RigidBody *body = world->createRigidBody(rp3d::Transform());
    body->setType(rp3d::BodyType::DYNAMIC);
    body->setMass(rigidBody->getBodyMass());
    body->setTransform(IRigidBody::toRp3dTransform(rigidBody->getBodyPosition(), rigidBody->getBodyRotation()));
    for (auto &force: rigidBody->getForces()) {
        body->applyLocalForceAtCenterOfMass(force->getDirection());
    }


    rigidBody->setRP3DBody(body);
}


void PhysicsEngine::update() {
	if(!camera){
		return;
	}

    //world->update(1.0 / 100.0);

    auto ray = createRayFromMousePosition();
    auto hits = std::vector<std::shared_ptr<IRigidBody>>();

    for (auto &body: bodies) {
        for (auto &collider: body->getColliders()) {
            if (collider->getRP3DCollider() == nullptr) {
                continue;
            }
            RaycastInfo info;
            auto hit = collider->getRP3DCollider()->raycast(ray, info);
            if (hit) {
                hits.push_back(body);
                break;
            }
        }
        body->update();
    }
    auto cameraPosition = camera->getPosition();
    auto min = std::numeric_limits<float>::max();
    std::shared_ptr<IRigidBody> closestBody = nullptr;
    for(const auto& hit: hits){
        auto distance = glm::distance(cameraPosition, hit->getBodyPosition());
        if(distance < min){
            min = distance;
            closestBody = hit;
        }
    }
    for(const auto& body : bodies){
        body->setIsMouseOver(body == closestBody);
    }
}

void PhysicsEngine::setCamera(std::shared_ptr<ICamera> camera) {
    this->camera = camera;
}

Ray PhysicsEngine::createRayFromMousePosition() {

    auto mousePosition = inputHandler->getAbsolutePosition();

    glm::vec4 rayClip = glm::vec4((2.0f * mousePosition.x) / camera->getWidth() - 1.0f,
                                  1.0f - (2.0f * mousePosition.y) / camera->getHeight(),
                                  -1.0,
                                  1.0);
    glm::vec4 rayEye = glm::inverse(camera->getProjectionMatrix()) * rayClip;
    rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0, 0.0);
    glm::vec3 rayWorld = glm::vec3(glm::inverse(camera->getViewMatrix()) * rayEye);
    rayWorld = glm::normalize(rayWorld);

    auto start = camera->getPosition();
    auto end = start + rayWorld * 1000.0f;
    Ray ray = Ray(rp3d::Vector3(start.x, start.y, start.z), rp3d::Vector3(end.x, end.y, end.z));
    return ray;
}
