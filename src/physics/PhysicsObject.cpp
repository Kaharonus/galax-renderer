//
// Created by tomas on 8.11.22.
//

#include <physics/PhysicsObject.h>

using namespace Galax::Physics;

rp3d::PhysicsCommon PhysicsObject::common = rp3d::PhysicsCommon();

rp3d::PhysicsWorld* Galax::Physics::PhysicsObject::createWorld() {
    return common.createPhysicsWorld();
}

rp3d::SphereShape *PhysicsObject::createSphereShape(float radius) {
    return common.createSphereShape(radius);
}

void PhysicsObject::deleteSphereShape(rp3d::SphereShape *shape) {
    common.destroySphereShape(shape);
}
