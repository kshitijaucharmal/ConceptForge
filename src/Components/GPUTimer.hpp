//
// Created by kshitij on 2/20/26.
//

#ifndef CONCEPTFORGE_GPUTIMER_HPP
#define CONCEPTFORGE_GPUTIMER_HPP
#include "glad/glad.h"

struct GPUTimer {
    GLuint queries[2] = {0, 0};
    uint64_t timeElapsed = 0;

    void Init() { glGenQueries(2, queries); }

    void Start() const {
        glBeginQuery(GL_TIME_ELAPSED, queries[0]);
    }

    void Stop() {
        glEndQuery(GL_TIME_ELAPSED);
        // Retrieve result (this might cause a slight stall, but is fine for debug)
        glGetQueryObjectui64v(queries[0], GL_QUERY_RESULT, &timeElapsed);
    }

    float GetMS() const { return timeElapsed / 1000000.0f; } // Convert nanoseconds to ms
};

#endif //CONCEPTFORGE_GPUTIMER_HPP