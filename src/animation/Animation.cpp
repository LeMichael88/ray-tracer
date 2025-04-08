#include "Animation.hpp"

#include <cstring>

//---------------------------------------------------------------------------------------
// right is used for linearly moving an object to the right in world coordinates
glm::vec3 right(const float t)
{
	return {0.12f * t, 0.0f, 0.0f};
}

//---------------------------------------------------------------------------------------
// left is used for linearly moving an object to the left in world coordinates
glm::vec3 left(const float t)
{
  return {-0.14f * t, 0.0f, 0.0f};
}

//---------------------------------------------------------------------------------------
// left2 is used for linearly moving an object to the left in world coordinates
glm::vec3 left2(const float t)
{
  return {-0.12f * t, 0.0f, 0.0f};
}

//---------------------------------------------------------------------------------------
// up is used for linearly moving an object up in world coordinates
glm::vec3 up(const float t)
{
  return {0.0f, 0.72f * t, 0.0f};
}

//---------------------------------------------------------------------------------------
// sunTrans is used for linearly moving an object up in world coordinates
glm::vec3 sunTrans(const float t)
{
  return {0.0f, -25.0f, -40.0f};
}


//---------------------------------------------------------------------------------------
// down is used for linearly moving an object down in world coordinates
glm::vec3 down(const float t)
{
  return {0.0f, -0.65f * t, 0.0f};
}

//---------------------------------------------------------------------------------------
// rightFall is used for parabolic motion falling to the right
glm::vec3 rightFall(const float t)
{
  float deltaT = t * 0.1f;
  return {deltaT, -1.0f * pow(deltaT, 2), 0.0f};
}

//---------------------------------------------------------------------------------------
// leftFall is used for parabolic motion falling to the left
glm::vec3 leftFall(const float t)
{
  float deltaT = t * 0.1f;
  return {-deltaT, -1.0f * pow(-deltaT, 2), 0.0f};
}

//---------------------------------------------------------------------------------------
// camera is used for the camera movement
glm::vec3 camera(const float t)
{
  return {0.0f, -1.0f * (sin(0.131f * t) + (0.209f * t)), -0.8f * t};
}

//---------------------------------------------------------------------------------------
// view is used for the view point movement
glm::vec3 view(const float t)
{
  return {0.0f, 0.104f * t, 0.0f};
}

//---------------------------------------------------------------------------------------
// spin is used to rotate an object (output is in degrees)
float spin(const float t)
{
  return t * 1.5f;
}

//---------------------------------------------------------------------------------------
// waves is used for displacement mapping for waves of a mesh
glm::vec3 waves(const glm::vec3 p, const float t)
{
  // To calm the waves at a certain frame
  float scalingFactor;
  if (t > 248)
  {
    scalingFactor = 0.0f;
  }
  else if (t > 200)
  {
    scalingFactor = 1.0f - ((t - 200.0f) / 48.0f);
  }
  else
  {
    scalingFactor = 1.0f;
  }

	float deltaT = t * 0.003f;
	return {
    p.x,
    p.y + scalingFactor * 0.5f * sin((p.z + deltaT) * 15.0f),
    p.z
  };
}

//---------------------------------------------------------------------------------------
// Basic constructor
Animation::Animation()
  : m_start(0),
    m_end(0),
    m_type(AnimationType::None),
    m_animation(nullptr),
    m_scalarAnimation(nullptr),
    m_vertexDisplacement(nullptr) {
}

//---------------------------------------------------------------------------------------
// Constructor that defines the animation type, start/end frame, and function
Animation::Animation(
  const float start, 
  const float end, 
  const char type,
  const char* functionName
)
  : m_start(start),
    m_end(end)
{
  // Set animation type based on char
  switch(type)
  {
    case 't':
      m_type = AnimationType::Translate;
      break;
    
    case 'r':
      m_type = AnimationType::Rotate;
      break;
    
    case 'd':
      m_type = AnimationType::VertexDisplacement;
      break;
    
    default:
      m_type = AnimationType::None;
      break;
  }

  // Set animation function
  if (m_type == AnimationType::VertexDisplacement)
  {
    if (std::strcmp(functionName, "waves") == 0)
    {
      m_vertexDisplacement = waves;
    }
    else
    {
      m_type = AnimationType::None;
    }
  }
  else if (m_type == AnimationType::Rotate)
  {
    if (std::strcmp(functionName, "spin") == 0)
    {
      m_scalarAnimation = spin;
    }
    else
    {
      m_type = AnimationType::None;
    }
  }
  else if (m_type == AnimationType::Translate)
  {
    if (std::strcmp(functionName, "right") == 0)
    {
      m_animation = right;
    }
    else if (std::strcmp(functionName, "left") == 0)
    {
      m_animation = left;
    }
    else if (std::strcmp(functionName, "rightFall") == 0)
    {
      m_animation = rightFall;
    }
    else if (std::strcmp(functionName, "leftFall") == 0)
    {
      m_animation = leftFall;
    }
    else if (std::strcmp(functionName, "left2") == 0)
    {
      m_animation = left2;
    }
    else if (std::strcmp(functionName, "up") == 0)
    {
      m_animation = up;
    }
    else if (std::strcmp(functionName, "sunTrans") == 0)
    {
      m_animation = sunTrans;
    }
    else if (std::strcmp(functionName, "down") == 0)
    {
      m_animation = down;
    }
    else if (std::strcmp(functionName, "view") == 0)
    {
      m_animation = view;
    }
    else if (std::strcmp(functionName, "camera") == 0)
    {
      m_animation = camera;
    }
    else
    {
      m_type = AnimationType::None;
    }
  }
}
