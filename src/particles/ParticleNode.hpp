/**
 * Name: ParticleNode
 * Description: Contains the definition of the ParticleNode class, which is used to
 * represent a particle system in a 3D scene. The class inherits from SceneNode
 * and provides functionality for creating and managing particles.
 */

#pragma once

#include "geometry/GeometryNode.hpp"
#include "geometry/SceneNode.hpp"
#include "materials/Material.hpp"

// Use this #define to selectively compile code to render the bounding boxes around your
// mesh objects. Uncomment this option to turn it on.
// #define RENDER_PARTICLE_BOUNDING_VOLUMES

/**
 * ParticleDirection is the axis in which particles move within the particle system in 3D.
 */
enum ParticleDirection {
	X,
	Y,
	Z
};

/**
 * ParticleNode class holds the particle system definition. The spawn plane is defined by
 * an initial position pos, a direction, and the length of the sides of a square built
 * from the initial pos. The particles size, life span, speed, and material. Particles are
 * only rendered in the bounding box defined by the base area and the lifespan * speed.
 */
class ParticleNode : public SceneNode {
public:
  ParticleNode(
    const std::string& name,
    glm::vec3 pos,
    float area,
    int spawnRate,
    float particleRadius,
    float particleLifeSpan,
    char particleDirection,
    float particleSpeed,
    Material *particleMaterial
  );

  void createParticle(int currFrame);
  void preprocessParticles(int currFrame);

  [[nodiscard]] Intersection intersect(const Ray& ray) const override;

private:
  // Particle system configuration
  glm::vec3 m_pos;
  float m_area; // Spawn area dimensions (a square starting from m_pos as one corner)
  NonhierBox m_boundingBox; // Particles restricted to this area, for optimization
  int m_spawnRate; // Rate in num particles per frame
  std::list<NonhierSphere> m_particles; // List of particles active in this system
  ParticleDirection m_particleDirection; // Just the axis that particles move

  // Individual particle details
  float m_particleRadius;
  float m_particleLifeSpan;
  glm::vec3 m_particleDisplacement; // Particle direction vector
	Material *m_particleMaterial;
};
