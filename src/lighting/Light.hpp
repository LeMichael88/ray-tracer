/*
 * Name: Light
 * Description: Defines the position and colour of a point light source in the scene with
 * optional animation.
 */

#pragma once

#include <ostream>

#include <glm/glm.hpp>

#include "animation/Animation.hpp"

/**
 * Light class represents a point light source in the scene, and has optional animation
 * allowing it to change positions
 */
class Light {
public:
    Light();

    void animateLight(float t);

    void resetAnimation();

    glm::vec3 m_colour;
    glm::vec3 m_position;
    glm::vec3 m_originalPosition;
    double m_falloff[3]{};

    Animation m_animation;
};

std::ostream& operator<<(std::ostream& out, const Light& l);
