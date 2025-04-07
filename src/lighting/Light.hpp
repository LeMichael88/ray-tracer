#pragma once

#include <iosfwd>
#include <vector>

#include <glm/glm.hpp>

#include "animation/Animation.hpp"

// Represents a simple point light.
class Light {
public:
  Light();

  void animateLight(const float t);
	void resetAnimation();

  glm::vec3 m_colour;
  glm::vec3 m_position;
  glm::vec3 m_originalPosition;
  double m_falloff[3];

  Animation m_animation;
};

std::ostream& operator<<(std::ostream& out, const Light& l);
