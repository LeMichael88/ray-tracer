#include <iostream>

#include <glm/ext.hpp>

#include "Light.hpp"

//---------------------------------------------------------------------------------------
Light::Light()
  : m_colour(0.0, 0.0, 0.0),
    m_position(0.0, 0.0, 0.0)
{
  m_falloff[0] = 1.0;
  m_falloff[1] = 0.0;
  m_falloff[2] = 0.0;
}

//---------------------------------------------------------------------------------------
// Animates a light for this frame by displacing it using m_animations
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
// Resets this light's transformation to its original position
void Light::resetAnimation()
{
	m_position = m_originalPosition;
}

std::ostream& operator<<(std::ostream& out, const Light& l)
{
  out << "L[" << glm::to_string(l.m_colour)
  	  << ", " << glm::to_string(l.m_position) << ", ";
  for (int i = 0; i < 3; i++) {
    if (i > 0) out << ", ";
    out << l.m_falloff[i];
  }
  out << "]";
  return out;
}
