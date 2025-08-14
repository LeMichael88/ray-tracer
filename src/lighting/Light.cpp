#include "Light.hpp"

#include <iostream>

#include <glm/ext.hpp>

//---------------------------------------------------------------------------------------
/**
 * Default Constructor for Light
 */
Light::Light()
    : m_colour(0.0, 0.0, 0.0),
      m_position(0.0, 0.0, 0.0)
{
    m_falloff[0] = 1.0;
    m_falloff[1] = 0.0;
    m_falloff[2] = 0.0;
}

//---------------------------------------------------------------------------------------
/**
 * animateLight is used to translate light position based on the attached animation
 * @param t The frame time to calculate the translation
 */
void Light::animateLight(const float t)
{
    // Store the transformation matrix before animation so we can reset it
    m_originalPosition = m_position;

    // Translations
    if (m_animation.m_type == AnimationType::Translate)
    {
        if (t < m_animation.m_start) // Stop if this animation is not reached yet
        {
            return;
        }
        else if (t > m_animation.m_end) // Full translation if animation is completed
        {
            float deltaT = m_animation.m_end - m_animation.m_start;
            glm::vec3 trans = m_animation.m_animation(deltaT);
            m_position = trans + m_position;
        }
        else // We are in the middle of this animation
        {
            float deltaT = t - m_animation.m_start;
            glm::vec3 trans = m_animation.m_animation(deltaT);
            m_position = trans + m_position;
        }
    }
}

//---------------------------------------------------------------------------------------
/**
 * resetAnimation resets the light position to the original position
 */
void Light::resetAnimation()
{
    m_position = m_originalPosition;
}

//---------------------------------------------------------------------------------------
/**
 * operator<< overload for Light for printing purposes
 * @param out The output stream
 * @param l The light to be printed
 * @return The output stream
 */
std::ostream& operator<<(std::ostream& out, const Light& l)
{
    out << "L[" << glm::to_string(l.m_colour)
        << ", " << glm::to_string(l.m_position) << ", ";
    for (int i = 0; i < 3; i++)
    {
        if (i > 0) out << ", ";
        out << l.m_falloff[i];
    }
    out << "]";
    return out;
}
