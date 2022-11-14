//
// Created by tomas on 8.11.22.
//


#pragma once

#include <physics/interfaces/ICollider.h>

namespace Galax::Physics {
    class SphereCollider : public ICollider {
    public:
        explicit SphereCollider(float radius);
        ~SphereCollider();

        rp3d::CollisionShape* getRP3DCollisionShape() override;

    private:
        float radius;
        rp3d::CollisionShape* collisionShape;

    };
}