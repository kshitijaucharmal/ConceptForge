//
// Created by kshitij on 30/6/25.
//

#include "PhysicsSystem.hpp"

// In PhysicsSystem.cpp or PhysicsEngine.cpp
#include <btBulletDynamicsCommon.h>

// Globals or members of your PhysicsSystem
btBroadphaseInterface* broadphase;
btDefaultCollisionConfiguration* collisionConfiguration;
btCollisionDispatcher* dispatcher;
btSequentialImpulseConstraintSolver* solver;
btDiscreteDynamicsWorld* dynamicsWorld;

void InitPhysics() {
    broadphase = new btDbvtBroadphase();
    collisionConfiguration = new btDefaultCollisionConfiguration();
    dispatcher = new btCollisionDispatcher(collisionConfiguration);
    solver = new btSequentialImpulseConstraintSolver;

    dynamicsWorld = new btDiscreteDynamicsWorld(
        dispatcher, broadphase, solver, collisionConfiguration
    );
    dynamicsWorld->setGravity(btVector3(0, -9.81f, 0));
}
