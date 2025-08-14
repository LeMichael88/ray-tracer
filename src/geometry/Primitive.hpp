#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "animation/Animation.hpp"
#include "core/Ray.hpp"

//---------------------------------------------------------------------------------------
class Primitive {
public:
    virtual ~Primitive();

    virtual bool intersect(const Ray& ray, Intersection& intersection) const;
    virtual void normal(const glm::vec3& p, const float t, glm::vec3& n) const;
    virtual void getUV(const glm::vec3& p, const float t, glm::vec2& uv) const;

    void getFramePosition(
        float t,
        const glm::vec3& initialPos,
        glm::vec3& newPos
    ) const;

    // Simple, singular linear interpolation for now
    int m_startFrame = -1;
    int m_endFrame = -1;
    glm::vec3 m_translation = glm::vec3(0.0f, 0.0f, 0.0f);
    Animation m_displacementMap;
};

//---------------------------------------------------------------------------------------
class Sphere : public Primitive {
public:
    virtual ~Sphere();

    virtual bool intersect(const Ray& ray, Intersection& intersection) const;
    virtual void normal(const glm::vec3& p, const float t, glm::vec3& n) const;
    virtual void getUV(const glm::vec3& p, const float t, glm::vec2& uv) const;
};

//---------------------------------------------------------------------------------------
class Cube : public Primitive {
public:
    virtual ~Cube();

    virtual bool intersect(const Ray& ray, Intersection& intersection) const;
};

//---------------------------------------------------------------------------------------
class NonhierSphere : public Primitive {
public:
    NonhierSphere(const glm::vec3& pos, double radius)
        : m_pos(pos), m_radius(radius)
    {}
    virtual ~NonhierSphere();

    virtual bool intersect(const Ray& ray, Intersection& intersection) const;
    virtual void normal(const glm::vec3& p, const float t, glm::vec3& n) const;
    virtual void getUV(const glm::vec3& p, const float t, glm::vec2& uv) const;

private:
    glm::vec3 m_pos;
    double m_radius;
};

//---------------------------------------------------------------------------------------
class NonhierBox : public Primitive {
public:
    NonhierBox() {};
    NonhierBox(const glm::vec3& pos, glm::vec3 size)
        : m_pos(pos), m_size(size)
    {}
    virtual ~NonhierBox();

    virtual bool intersect(const Ray& ray, Intersection& intersection) const;

    // private:
    glm::vec3 m_pos;
    glm::vec3 m_size;
};
