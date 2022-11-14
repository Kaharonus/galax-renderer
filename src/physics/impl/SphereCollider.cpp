//
// Created by tomas on 8.11.22.
//

#include <physics/impl/SphereCollider.h>

using namespace Galax::Physics;

SphereCollider::SphereCollider(float radius) : ICollider() {
    this->radius = radius;
    this->collisionShape = nullptr;
}


rp3d::CollisionShape *SphereCollider::getRP3DCollisionShape() {
    if(collisionShape == nullptr) {
        collisionShape = createSphereShape(radius);
    }
    return collisionShape;
}

SphereCollider::~SphereCollider() {
    deleteSphereShape((rp3d::SphereShape*)collisionShape);
}

