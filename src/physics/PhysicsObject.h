//
// Created by tomas on 8.11.22.
//


#pragma once

#include <reactphysics3d/reactphysics3d.h>

namespace Galax::Physics{
    class PhysicsObject {
    protected:
        static rp3d::PhysicsWorld* createWorld();
        static rp3d::SphereShape* createSphereShape(float radius);
        static void deleteSphereShape(rp3d::SphereShape* shape);

    private:
        static rp3d::PhysicsCommon common;
    };

}

