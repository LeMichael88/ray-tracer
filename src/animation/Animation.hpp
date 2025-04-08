/*
 * Name: Animation
 * Description: Handles initializing animation objects to define geometry movement in
 * world coordinates. This file defines hard-coded animation functions that are set in the
 * lua files.
 */

#pragma once

#include <glm/glm.hpp>

enum class AnimationType {
  None,
  Translate,
  Rotate,
  VertexDisplacement
};

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
  glm::vec3 (*m_animation)(float);
  float (*m_scalarAnimation)(float);
  glm::vec3 (*m_vertexDisplacement)(glm::vec3, float);
};
