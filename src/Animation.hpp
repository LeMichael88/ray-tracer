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
    const float start, 
    const float end, 
    const char type,
    const char* functionName
  );

	float m_start;
  float m_end;
  AnimationType m_type;
  glm::vec3 (*m_animation)(const float);
  float (*m_scalarAnimation)(const float);
  glm::vec3 (*m_vertexDisplacement)(const glm::vec3, const float);
};
