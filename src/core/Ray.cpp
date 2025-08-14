#include "Ray.hpp"

#include <glm/ext.hpp>
#include <glm/gtx/io.hpp>

//---------------------------------------------------------------------------------------
Ray::Ray(glm::vec3 origin, glm::vec3 direction, float time)
    : m_origin(origin),
      m_direction(direction),
      m_time(time)
{}

//---------------------------------------------------------------------------------------
glm::vec3 Ray::origin() const
{
    return m_origin;
}

//---------------------------------------------------------------------------------------
glm::vec3 Ray::direction() const
{
    return m_direction;
}

//---------------------------------------------------------------------------------------
glm::vec3 Ray::transformedOrigin() const
{
    return m_modelOrigin;
}

//---------------------------------------------------------------------------------------
glm::vec3 Ray::transformedDirection() const
{
    return m_modelDirection;
}

//---------------------------------------------------------------------------------------
float Ray::time() const
{
    return m_time;
}

//---------------------------------------------------------------------------------------
// Transforms the ray origin and direction to model coordinates using T
void Ray::transform(const glm::mat4& T)
{
    m_modelOrigin = glm::vec3(T * glm::vec4(m_origin, 1.0f));
    m_modelDirection = glm::vec3(T * glm::vec4(m_direction, 0.0f));
}

//---------------------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& os, const Ray& ray)
{
    os << "Ray " << ray.m_id
        << " from origin " << ray.origin()
        << " in direction " << ray.direction();
    return os;
}

//---------------------------------------------------------------------------------------
// Checks if newPoint is closer than an existing intersection in m_point from ray origin
// Returns true if newPoint is closer, false otherwise
bool Intersection::isPointCloser(const glm::vec3& newPoint, const Ray& ray) const
{
    // If there is no intersection yet then this point is closer by default
    // In the case of a light sources, we check with m_point (which stores light pos)
    if (!m_foundIntersection && !m_isLight)
    {
        return true;
    }

    // Check distances and return true if newPoint is closer (in world coordinates)
    float d_new = glm::length(newPoint - ray.origin());
    float d_prev = glm::length(m_point - ray.origin());
    return (d_new < d_prev);
}

//---------------------------------------------------------------------------------------
// Transforms the intersection point and normal by T
void Intersection::transformIntersection(const glm::mat4& T)
{
    m_normal = glm::normalize(glm::mat3(glm::transpose(glm::inverse(T))) * m_normal);
    m_point = glm::vec3(T * glm::vec4(m_point, 1.0f));
}
