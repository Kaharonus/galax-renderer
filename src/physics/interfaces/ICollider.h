//
// Created by tomas on 8.11.22.
//

#pragma once

#include <memory>
#include <reactphysics3d/reactphysics3d.h>
#include <physics/PhysicsObject.h>

namespace Galax::Physics {
    class ICollider : public PhysicsObject {
        friend class IRigidBody;
    public:
        virtual rp3d::CollisionShape* getRP3DCollisionShape() = 0;

        virtual void update() = 0;

        void setRP3DCollider(rp3d::Collider *c) {
            this->collider = c;
        };

        rp3d::Collider* getRP3DCollider() const {
            return this->collider;
        };


    private:
        rp3d::Collider* collider;

    };

}
