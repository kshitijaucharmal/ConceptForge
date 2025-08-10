//
// Created by kshitij on 10/8/25.
//

#include "DebugDrawer.hpp"

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace PhysicsDebug{
    DebugDrawer::DebugDrawer() : m_debugMode(DBG_DrawWireframe) {};

    void DebugDrawer::drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &color) {
        // TODO: Draw lines with glDrawLines
        glDisable(GL_LIGHTING); // Ensure lighting doesn’t mess with colors
        glColor3f(color.getX(), color.getY(), color.getZ());

        glBegin(GL_LINES);
        glVertex3f(from.getX(), from.getY(), from.getZ());
        glVertex3f(to.getX(), to.getY(), to.getZ());
        glEnd();

        glEnable(GL_LIGHTING);
    }

    void DebugDrawer::drawContactPoint(const btVector3 &PointOnB, const btVector3 &normalOnB, btScalar distance, int lifeTime, const btVector3 &color) {
        // TODO: Implement small points
    }

    void DebugDrawer::reportErrorWarning(const char *warningString) {
        // Just simple error message for now
        std::cerr << "ERROR::BULLET::" << warningString << std::endl;
    }

    void DebugDrawer::draw3dText(const btVector3 &location, const char *textString) {
        // TODO: Implement this after figuring out fonts
    }
}
