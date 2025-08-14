#include "ParticleNode.hpp"

#include <random>

#include <glm/gtx/io.hpp>

//---------------------------------------------------------------------------------------
/**
 * ParticleNode constructor
 * @param name Name of the node
 * @param pos Initial point to base the particle system around
 * @param area Side length of the area in which particles are spawned
 * @param spawnRate Number of particles to spawn per frame
 * @param particleRadius Radius of each particle
 * @param particleLifeSpan Life span of each particle in frames
 * @param particleDirection Direction in which particles move (X, Y, or Z)
 * @param particleSpeed Speed at which particles move
 * @param particleMaterial Material used for the particles' colour
 */
ParticleNode::ParticleNode(
    const std::string& name,
    const glm::vec3 pos,
    const float area,
    const int spawnRate,
    const float particleRadius,
    const float particleLifeSpan,
    const char particleDirection,
    const float particleSpeed,
    Material* particleMaterial
)
    : SceneNode(name),
      m_pos(pos),
      m_area(area),
      m_spawnRate(spawnRate),
      m_particleRadius(particleRadius),
      m_particleLifeSpan(particleLifeSpan),
      m_particleMaterial(particleMaterial)
{
    m_nodeType = NodeType::ParticleNode;

    // Set up bounding box dimensions and particle displacement
    float dist = particleSpeed * particleLifeSpan;
    switch (particleDirection)
    {
        case 'x':
            m_particleDirection = ParticleDirection::X;
            m_particleDisplacement = glm::vec3(dist, 0.0f, 0.0f);
            m_boundingBox = NonhierBox(
                glm::vec3(std::min(pos.x, pos.x + dist), pos.y, pos.z),
                glm::vec3(abs(dist) + 2.0f * particleRadius, area, area)
            );
            break;

        case 'y':
            m_particleDirection = ParticleDirection::Y;
            m_particleDisplacement = glm::vec3(0.0f, dist, 0.0f);
            m_boundingBox = NonhierBox(
                glm::vec3(pos.x, std::min(pos.y, pos.y + dist), pos.z),
                glm::vec3(area, abs(dist) + 2.0f * particleRadius, area)
            );
            break;

        case 'z':
            m_particleDirection = ParticleDirection::Z;
            m_particleDisplacement = glm::vec3(0.0f, 0.0f, dist);
            m_boundingBox = NonhierBox(
                glm::vec3(pos.x, pos.y, std::min(pos.z, pos.z + dist)),
                glm::vec3(area, area, abs(dist) + 2.0f * particleRadius)
            );
            break;

        default:
            break;
    }

    // Stop bounding box from moving
    m_boundingBox.m_startFrame = -1.0f;
    m_boundingBox.m_endFrame = -1.0f;
    m_boundingBox.m_translation = glm::vec3(0.0f);
}

//---------------------------------------------------------------------------------------
/**
 * createParticle spawns a new particle at a random point on the spawn plane.
 * @param currFrame Current frame of the animation
 */
void ParticleNode::createParticle(const int currFrame)
{
    // Initialize with random seed
    std::mt19937 randomEngine(std::random_device{}());
    std::uniform_real_distribution<float> m_distribution(0.0f, 1.0f);

    // Generate a random position on the starting plane
    // Adds some wiggle room for the particles so things don't get clipped
    float i = m_distribution(randomEngine)
              * (m_area - (2.0f * m_particleRadius)) + m_particleRadius;
    float j = m_distribution(randomEngine)
              * (m_area - (2.0f * m_particleRadius)) + m_particleRadius;

    glm::vec3 particleSpawnPos;
    float startPos;
    switch (m_particleDirection)
    {
        case X:
            startPos = (m_particleDisplacement.x > 0)
                           ? m_pos.x + m_particleRadius
                           : m_pos.x - m_particleRadius;
            particleSpawnPos = glm::vec3(startPos, m_pos.y + i, m_pos.z + j);
            break;

        case Y:
            startPos = (m_particleDisplacement.y > 0)
                           ? m_pos.y + m_particleRadius
                           : m_pos.y - m_particleRadius;
            particleSpawnPos = glm::vec3(m_pos.x + i, startPos, m_pos.z + j);
            break;

        case Z:
            startPos = (m_particleDisplacement.z > 0)
                           ? m_pos.z + m_particleRadius
                           : m_pos.z - m_particleRadius;
            particleSpawnPos = glm::vec3(m_pos.x + i, m_pos.y + j, startPos);
            break;

        default:
            break;
    }

    // Set up the particle to spawn at this position and move
    NonhierSphere p = NonhierSphere(particleSpawnPos, m_particleRadius);
    p.m_startFrame = currFrame;
    p.m_endFrame = static_cast<int>(m_particleLifeSpan) + currFrame;
    p.m_translation = m_particleDisplacement;
    m_particles.push_back(p);
}

//---------------------------------------------------------------------------------------
/**
 * preprocessParticles spawns new particles and removes expired ones at the start of
 * each frame
 * @param currFrame Current frame of the animation
 */
void ParticleNode::preprocessParticles(const int currFrame)
{
    // Create particles this frame based on spawn rate
    for (int i = 0; i < m_spawnRate; i++)
    {
        createParticle(currFrame);
    }

    // Clear out any particles that have expired
    m_particles.remove_if(
        [currFrame](const NonhierSphere& particle) {
            return (particle.m_endFrame == currFrame);
        }
    );
}

//---------------------------------------------------------------------------------------
// Checks
/**
 * intersect if this particle system intersects with the ray. Since particles are
 * restricted to the bounding volume, only proceed if ray intersects with the bounding
 * volume.
 * @param ray Ray to check for intersection with the particle system
 * @return Intersection object containing information about the intersection
 */
Intersection ParticleNode::intersect(const Ray& ray) const
{
    Intersection intersection;

#ifdef RENDER_PARTICLE_BOUNDING_VOLUMES
    // Render bounding volume instead of the particles
    if (m_boundingBox.intersect(ray, intersection))
    {
        // Update intersection values of bounding box if ray intersected
        intersection.m_material = static_cast<PhongMaterial*>(m_particleMaterial);
        m_boundingBox.getUV(intersection.m_point, ray.time(), intersection.m_uv);
        intersection.m_foundIntersection = true;
    }
#else
    // Bounding box check, if it doesn't hit, we skip
    if (!m_boundingBox.intersect(ray, intersection))
    {
        return intersection;
    }

    // Otherwise, check for intersection with all particles
    for (const NonhierSphere& particle: m_particles)
    {
        // If this node intersected with the ray, update intersection values
        Intersection temp;
        if (particle.intersect(ray, temp))
        {
            // Check if we have a closer intersection first
            if (!intersection.m_foundIntersection ||
                (intersection.m_foundIntersection &&
                 (glm::length(temp.m_point - ray.transformedOrigin()) <
                  glm::length(intersection.m_point - ray.transformedOrigin())))
            )
            {
                // Found valid intersection, update material, normal, uv coordinates
                intersection.m_material = dynamic_cast<PhongMaterial*>(m_particleMaterial);
                particle.normal(intersection.m_point, ray.time(), intersection.m_normal);
                particle.getUV(intersection.m_point, ray.time(), intersection.m_uv);
                intersection.m_foundIntersection = true;
            }
        }
    }
#endif

    return intersection;
}
