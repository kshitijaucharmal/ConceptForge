//
// Created by kshitij on 30/6/25.
//

#ifndef BULLETPHYSICS_HPP
#define BULLETPHYSICS_HPP

class btDiscreteDynamicsWorld;
class btSequentialImpulseConstraintSolver;
class btCollisionDispatcher;
class btDefaultCollisionConfiguration;
class btBroadphaseInterface;

struct BulletPhysics {
    btBroadphaseInterface* broadphase = nullptr;
    btDefaultCollisionConfiguration* collisionConfiguration = nullptr;
    btCollisionDispatcher* dispatcher = nullptr;
    btSequentialImpulseConstraintSolver* solver = nullptr;
    btDiscreteDynamicsWorld* dynamicsWorld = nullptr;
};

#endif //BULLETPHYSICS_HPP
