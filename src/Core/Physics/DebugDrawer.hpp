//
// Created by kshitij on 10/8/25.
//

#pragma once


#include <btBulletDynamicsCommon.h>

namespace PhysicsDebug {
    class DebugDrawer : public btIDebugDraw {
    private:
        int m_debugMode;

    public:
        DebugDrawer();

        void drawLine(const btVector3& from, const btVector3& to, const btVector3& color) override;
        void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color) override;
        void reportErrorWarning(const char* warningString) override;
        // Optional
        void draw3dText(const btVector3& location, const char* textString) override;

        // Helpers
        void setDebugMode(int debugMode) override {
            m_debugMode = debugMode;
        }

        int getDebugMode() const override {
            return m_debugMode;
        }
    };
}
