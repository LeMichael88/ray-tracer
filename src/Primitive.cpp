#include "Primitive.hpp"

#include <iostream>

#include <glm/gtx/io.hpp>
#include <graphics-framework/polyroots.hpp>

// Epsilon check
const double E = 1e-2;

//---------------------------------------------------------------------------------------
// Computes the uv coordinates of a point on the sphere between 0 and 1, given a point p
void getUVSphere(
  const glm::vec3& p, const glm::vec3& pos, glm::vec2& uv
) {
  glm::vec3 d = glm::normalize(pos - p);
  float u = 0.5f + (atan2(d.z, d.x) / (2.0f * M_PI));
  float v = 0.5f + (asin(d.y) / M_PI);

  uv = glm::vec2(1.0f - u, v);
}

//---------------------------------------------------------------------------------------
// Computes general ray-sphere intersection
bool sphereIntersect(
  const Ray& ray,
  const glm::vec3& pos,
  const float radius,
  Intersection& intersection
) {
  glm::vec3 ca = ray.transformedOrigin() - pos;

  // Note: (b - a) in notes is just the direction of ray
  float A = glm::dot(ray.transformedDirection(), ray.transformedDirection());
  float B = 2.0f * glm::dot(ray.transformedDirection(), ca);
  float C = glm::dot(ca, ca) - pow(radius, 2);
  double roots[2];
  size_t numRoots = quadraticRoots(A, B, C, roots);

  if (numRoots == 0) // Case 1: No intersection
  {
    return false;
  }
  else if (numRoots == 1) // Case 2: One intersection
  {
    if (roots[0] < 0) // Make sure the intersection is valid (in front of the ray)
    {
      return false;
    }

    // Return the intersection point
    float t = static_cast<float>(roots[0]);
    intersection.m_point = ray.transformedOrigin() + t * ray.transformedDirection();    
  }
  else // Case 3: Two intersections
  {   
    if (roots[0] < 0 && roots[1] < 0)
    {
      return false;
    }

    // Get the smaller positive t
    float t = (std::min(roots[0], roots[1]) > 0) ? 
              std::min(roots[0], roots[1]) : 
              std::max(roots[0], roots[1]);

    // Return the intersection point        
    intersection.m_point = ray.transformedOrigin() + t * ray.transformedDirection();
  }
  return true;
}

//---------------------------------------------------------------------------------------
// Computes the normal of a point on a sphere and stores it in n
void sphereNormal(const glm::vec3 p, const glm::vec3& pos, glm::vec3& n)
{
  // Set vector
  n.x = 2 * (p.x - pos.x);
  n.y = 2 * (p.y - pos.y);
  n.z = 2 * (p.z - pos.z);

  // Normalize
  n /= length(n);
}

//---------------------------------------------------------------------------------------
// Computes the uv coordinates of a point p on a face of an axis aligned box, where the
// face of intersection is defined by a relative point pos and size of cube
void getUVBox(
  const glm::vec3& p, const glm::vec3& pos, const glm::vec3& size, glm::vec2& uv
) {
  glm::vec3 pFace = p - pos;

  // One value should be close to 0
  if (abs(pFace.x) < 1e-1)
  {
    uv = glm::vec2(pFace.y / size.y, pFace.z / size.z);
  }
  else if (abs(pFace.y) < 1e-1)
  {
    uv = glm::vec2(pFace.x / size.x, pFace.z / size.z);
  }
  else
  {
    uv = glm::vec2(pFace.x / size.x, pFace.y / size.y);
  }

  // Make sure coordinates are positive and between 0, 1
  uv = glm::abs(uv);
}

//---------------------------------------------------------------------------------------
// Computes general ray-box intersection
bool boxIntersect(
  const Ray& ray,
  const glm::vec3& pos,
  const glm::vec3& size,
  Intersection& intersection
) {
  // First, set the points and normals of the planes to iterate through
  // We assume that cubes are world axis aligned
  glm::vec3 planePoints[6];
  glm::vec3 normals[6];

  // Planes (first three share a point with pos, last three with opposite corner)
  for (int i = 0; i < 3; i++) {
    planePoints[i] = pos;
  }
  for (int i = 3; i < 6; i++) {
    planePoints[i] = pos + size;
  }

  // Normals point to the inside of box
  // Since this is the constructor, no transformations have been applied.
  // We can assume it is aligned to world axis
  normals[0] = glm::vec3(1.0f, 0.0f, 0.0f);
  normals[1] = glm::vec3(0.0f, 1.0f, 0.0f);
  normals[2] = glm::vec3(0.0f, 0.0f, 1.0f);
  normals[3] = glm::vec3(-1.0f, 0.0f, 0.0f);
  normals[4] = glm::vec3(0.0f, -1.0f, 0.0f);
  normals[5] = glm::vec3(0.0f, 0.0f, -1.0f);

  // Iterate through each plane
  bool intersectionFound = false;
  glm::vec3 intersectionPoint;
  int intersectionPlane;
  glm::vec2 intersectionUV;
  for (int i = 0; i < 6; i++) {
    float wecB = glm::dot(ray.transformedDirection(), normals[i]);

    if (abs(wecB) < 1e-8) // Avoid division by 0
    {
      continue;
    }

    // Get intersection point
    float wecA = glm::dot((planePoints[i] - ray.transformedOrigin()), normals[i]);
    float t = wecA / wecB;
    glm::vec3 p = ray.transformedOrigin() + t * ray.transformedDirection();

    // Verify that t >= 0 (point is in front of ray) and within boundary of box
    if (
      (t <= 0) ||
      (p.x > (pos.x + size.x + E) || p.x < (pos.x - E)) ||
      (p.y > (pos.y + size.y + E) || p.y < (pos.y - E)) ||
      (p.z > (pos.z + size.z + E) || p.z < (pos.z - E))
    ) {
      continue;
    }

    // If an intersection has already been found check which point is closer
    // Also check which face is looking towards the eye based on normals
    if (!intersectionFound ||
        (intersectionFound &&
          ((glm::length(p - ray.transformedOrigin()) < 
            glm::length(intersectionPoint - ray.transformedOrigin())) &&
          (dot(ray.transformedDirection(), normals[i]) > 
            dot(ray.transformedDirection(), normals[intersectionPlane]))))
    ) {
      intersectionPlane = i;
      intersectionPoint = p;
      getUVBox(p, planePoints[intersectionPlane], size, intersectionUV);
    }
    intersectionFound = true;
  }

  // Output result
  if (intersectionFound)
  {
    intersection.m_point = intersectionPoint;
    intersection.m_uv = intersectionUV;

    // Flip normal as the stored normal faces inwards
    intersection.m_normal = glm::normalize(-1.0f * normals[intersectionPlane]);
  }
  return intersectionFound;
}

//---------------------------------------------------------------------------------------
Primitive::~Primitive()
{
}

//---------------------------------------------------------------------------------------
// Checks if this geometry intersects with the ray
// For a default primative, just return false (no intersection)
bool Primitive::intersect(const Ray& ray, Intersection& intersection) const
{
  return false;
}

//---------------------------------------------------------------------------------------
// Computes the normal of a point on the primitive
// For a default primative, don't do anything 
void Primitive::normal(const glm::vec3& p, const float t, glm::vec3& n) const
{
}

//---------------------------------------------------------------------------------------
// Computes the uv coordinates of a point on the primitive
// For a default primative, don't do anything
void Primitive::getUV(const glm::vec3& p, const float t, glm::vec2& uv) const
{
}

//---------------------------------------------------------------------------------------
// Computes the position of a point of the primitive based on the animation
void Primitive::getFramePosition(
  const float t,
  const glm::vec3& initialPos,
  glm::vec3& newPos
) const {
  if (t < m_startFrame) // Stay at initial position if we haven't translated yet
  {
    newPos = initialPos;
    return;
  }
  else if (t > m_endFrame) // Full translation if animation is completed
  {
    newPos = initialPos + m_translation;
    return;
  }

  // Linearly interpolate the position
  glm::vec3 displacementPerFrame = m_translation
                                   / static_cast<float>(m_endFrame - m_startFrame);
  newPos = initialPos + (t - m_startFrame) * displacementPerFrame;
}

//---------------------------------------------------------------------------------------
Sphere::~Sphere()
{
}

//---------------------------------------------------------------------------------------
bool Sphere::intersect(const Ray& ray, Intersection& intersection) const
{
  // Get position of point at this frame after animation
  glm::vec3 updatedPos;
  getFramePosition(ray.time(), glm::vec3(0.0f), updatedPos);

  return sphereIntersect(ray, updatedPos, 1.0f, intersection);
}

//---------------------------------------------------------------------------------------
void Sphere::normal(const glm::vec3& p, const float t, glm::vec3& n) const
{
  // Get position of point at this frame after animation
  glm::vec3 updatedPos;
  getFramePosition(t, glm::vec3(0.0f), updatedPos);

  sphereNormal(p, updatedPos, n);
}

//---------------------------------------------------------------------------------------
void Sphere::getUV(const glm::vec3& p, const float t, glm::vec2& uv) const
{
  // Get position of point at this frame after animation
  glm::vec3 updatedPos;
  getFramePosition(t, glm::vec3(0.0f), updatedPos);

  getUVSphere(p, updatedPos, uv);
}

//---------------------------------------------------------------------------------------
Cube::~Cube()
{
}

//---------------------------------------------------------------------------------------
bool Cube::intersect(const Ray& ray, Intersection& intersection) const
{
  // Get position of point at this frame after animation
  glm::vec3 updatedPos;
  getFramePosition(ray.time(), glm::vec3(0.0f), updatedPos);

  return boxIntersect(ray, updatedPos, glm::vec3(1.0f), intersection);
}


//---------------------------------------------------------------------------------------
NonhierSphere::~NonhierSphere()
{
}

//---------------------------------------------------------------------------------------
bool NonhierSphere::intersect(const Ray& ray, Intersection& intersection) const
{
  // Get position of point at this frame after animation
  glm::vec3 updatedPos;
  getFramePosition(ray.time(), m_pos, updatedPos);

  return sphereIntersect(ray, updatedPos, m_radius, intersection);
}

//---------------------------------------------------------------------------------------
void NonhierSphere::normal(const glm::vec3& p, const float t, glm::vec3& n) const
{
  // Get position of point at this frame after animation
  glm::vec3 updatedPos;
  getFramePosition(t, m_pos, updatedPos);

  sphereNormal(p, updatedPos, n);
}

//---------------------------------------------------------------------------------------
void NonhierSphere::getUV(const glm::vec3& p, const float t, glm::vec2& uv) const
{
  // Get position of point at this frame after animation
  glm::vec3 updatedPos;
  getFramePosition(t, m_pos, updatedPos);

  getUVSphere(p, updatedPos, uv);
}

//---------------------------------------------------------------------------------------
NonhierBox::~NonhierBox()
{
}

//---------------------------------------------------------------------------------------
bool NonhierBox::intersect(const Ray& ray, Intersection& intersection) const
{
  // Get position of point at this frame after animation
  glm::vec3 updatedPos;
  getFramePosition(ray.time(), m_pos, updatedPos);

  return boxIntersect(ray, updatedPos, glm::vec3(m_size), intersection);
}
