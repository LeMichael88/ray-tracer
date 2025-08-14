#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/io.hpp>

#include "core/Ray.hpp"
#include "geometry/SceneNode.hpp"
#include "lighting/Light.hpp"
#include "particles/ParticleNode.hpp"
#include "utils/Image.hpp"

// Use this #define to selectively compile your code to print the
// debug logs. Uncomment this option to turn it on.
#define DEBUG_LOGS

using Color = glm::vec3;

// Supersampling size
const int SAMPLE_SIZE = 8;

// Max depth of recursion
const int MAX_DEPTH = 5;

class RayTracer {
public:
    RayTracer(
        SceneNode* root,
        const glm::mat4& screenToWorld,
        const glm::vec3& eye,
        const glm::vec3& ambient,
        const std::list<Light*>& lights,
        Image* image
    );

    void preprocessAnimation(SceneNode* node, const float t);
    void resetAnimation(SceneNode* node);
    void traverseSceneGraph(
        const SceneNode* node,
        Ray ray,
        Intersection& intersection,
        glm::mat4 trans,
        glm::mat4 invTrans
    ) const;
    Color raytrace(Ray& ray, int currDepth) const;
    void render(
        int frameNum,
        int threadNum,
        int startWidth,
        int endWidth
    ) const;

private:
    SceneNode* m_root;
    glm::mat4 m_screenToWorld;
    glm::vec3 m_eye;
    glm::vec3 m_ambient;
    std::list<Light*> m_lights;
    Image* m_image;
};

void A5_Render(
    // What to render
    SceneNode* root,

    // Image to write to, set to a given width and height
    Image& image,
    const char* fileName,
    int startFrame,
    int numFrames,

    // Viewing parameters
    const glm::vec3& eye,
    const glm::vec3& view,
    const glm::vec3& up,
    double fovy,

    // Lighting parameters
    const glm::vec3& ambient,
    const std::list<Light*>& lights,

    // Particle systems
    const std::list<ParticleNode*>& particleSpawners
);
