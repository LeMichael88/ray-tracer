#pragma once

#include "geometry/GeometryNode.hpp"
#include "geometry/SceneNode.hpp"
#include "materials/Material.hpp"

// Use this #define to selectively compile your code to render the
// bounding boxes around your mesh objects. Uncomment this option
// to turn it on.
// #define RENDER_PARTICLE_BOUNDING_VOLUMES

enum ParticleDirection {
	X,
	Y,
	Z
};

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

  void createParticle(const int currFrame);
  void preprocessParticles(const int currFrame);

  virtual Intersection intersect(const Ray& ray) const;

private:
  // System details
  glm::vec3 m_pos;
  float m_area; // Spawn area dimensions (a square starting from m_pos as one corner)
	NonhierBox m_boundingBox; // Particles restricted to this area, for optimization
  int m_spawnRate; // Rate in num particles per frame
  std::list<NonhierSphere> m_particles; // List of particles active in this system
  ParticleDirection m_particleDirection; // Just the axis that particles move

  // Particle details
  float m_particleRadius;
  float m_particleLifeSpan;
  glm::vec3 m_particleDisplacement; // Particle direction vector
	Material *m_particleMaterial;
};
