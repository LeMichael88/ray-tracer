/*
 * Name: Mesh
 * Description: Defines a local polygonal mesh object whose geometry can be imported from
 * an OBJ file in the lua script.
 */

#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "core/Ray.hpp"
#include "geometry/Primitive.hpp"

// Compile option to render bounding volumes instead of actual mesh geometry.
// #define RENDER_BOUNDING_VOLUMES

/**
 * Triangle struct defines a triangular face for a mesh.
 */
struct Triangle {
    size_t v1;
    size_t v2;
    size_t v3;

    Triangle(size_t pv1, size_t pv2, size_t pv3)
        : v1(pv1),
          v2(pv2),
          v3(pv3)
    {}
};

/*
 * Mesh class defines a polygonal mesh composed of triangular faces.
 */
class Mesh : public Primitive {
public:
    explicit Mesh(const std::string& name);

    bool intersect(const Ray& ray, Intersection& intersection) const override;

private:
    std::vector<glm::vec3> m_vertices;
    std::vector<Triangle> m_faces;
    NonhierBox m_boundingBox;

    friend std::ostream& operator<<(std::ostream& out, const Mesh& mesh);
};
