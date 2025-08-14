#pragma once

#include <iostream>

#include <glm/glm.hpp>

#include "materials/PhongMaterial.hpp"

class Ray {
public:
    Ray(glm::vec3 origin, glm::vec3 direction, float time);

    glm::vec3 origin() const;
    glm::vec3 direction() const;
    glm::vec3 transformedOrigin() const;
    glm::vec3 transformedDirection() const;
    float time() const;
    void transform(const glm::mat4& T);

    friend std::ostream& operator<<(std::ostream& os, const Ray& ray);

    int m_id = 0; // Mainly to debug a specific ray

private:
    glm::vec3 m_origin;
    glm::vec3 m_direction;

    glm::vec3 m_modelOrigin;
    glm::vec3 m_modelDirection;

    float m_time;
};

class Intersection {
public:
    Intersection()
        : m_isLight(false),
          m_foundIntersection(false)
    {};

    bool isPointCloser(const glm::vec3& newPoint, const Ray& ray) const;
    void transformIntersection(const glm::mat4& T);

    bool m_foundIntersection;
    bool m_isLight;
    glm::vec3 m_point;
    glm::vec2 m_uv;
    glm::vec3 m_normal;
    PhongMaterial* m_material;
};
