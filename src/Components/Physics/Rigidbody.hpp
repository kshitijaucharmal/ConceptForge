//
// Created by kshitij on 30/6/25.
//

#ifndef RIGIDBODY_HPP
#define RIGIDBODY_HPP

#include "Components/Physics/BulletPhysics.hpp"

class btMotionState;
class btCollisionShape;
class btRigidBody;

struct Rigidbody {
    btRigidBody* body = nullptr;
    btCollisionShape* shape = nullptr;
    btMotionState* motionState = nullptr;
    float mass = 1.0f;
};

#endif //RIGIDBODY_HPP
