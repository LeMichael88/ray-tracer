/*
 * Name: Animation
 * Description: Handles initializing animation objects to define geometry movement in
 * world coordinates. This file defines hard-coded animation functions that are set in the
 * lua files.
 */

#pragma once

#include <glm/glm.hpp>

/**
 * AnimationType defines the type of animation that an animation will perform.
 */
enum class AnimationType {
    None,
    Translate,
    Rotate,
    VertexDisplacement
};

/**
 * Animation class contains all data needed to define an animation on an object.
 */
class Animation {
public:
    Animation();
    Animation(
        float start,
        float end,
        char type,
        const char* functionName
    );

    float m_start;
    float m_end;
    AnimationType m_type;

    // Based on the type of animation, only one of the following should be used
    glm::vec3 (*m_animation)(float);
    float (*m_scalarAnimation)(float);
    glm::vec3 (*m_vertexDisplacement)(glm::vec3, float);
};
