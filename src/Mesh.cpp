#include <iostream>
#include <fstream>

#include <glm/ext.hpp>
#include <glm/gtx/io.hpp>

#include "Mesh.hpp"

//---------------------------------------------------------------------------------------
Mesh::Mesh( const std::string& fname )
	: m_vertices(),
		m_faces(),
		m_boundingBox()
{
	std::string code;
	double vx, vy, vz;
	size_t s1, s2, s3;

	// Store minimum values for the bounding box
	double min_x = std::numeric_limits<double>::max();
	double min_y = std::numeric_limits<double>::max();
	double min_z = std::numeric_limits<double>::max();
	double max_x = -1.0f * std::numeric_limits<double>::max();
	double max_y = -1.0f * std::numeric_limits<double>::max();
	double max_z = -1.0f * std::numeric_limits<double>::max();

	std::ifstream ifs( fname.c_str() );
	while( ifs >> code ) {
		if( code == "v" ) {
			ifs >> vx >> vy >> vz;
			glm::vec3 v = glm::vec3( vx, vy, vz );
			m_vertices.push_back( v );

			min_x = std::min(min_x, vx);
			min_y = std::min(min_y, vy);
			min_z = std::min(min_z, vz);
			max_x = std::max(max_x, vx);
			max_y = std::max(max_y, vy);
			max_z = std::max(max_z, vz);
		} else if( code == "f" ) {
			ifs >> s1 >> s2 >> s3;
			m_faces.push_back( Triangle( s1 - 1, s2 - 1, s3 - 1 ) );
		}
	}

	// Set bounding box dimensions
	glm::vec3 max_point(max_x, max_y, max_z);
	glm::vec3 min_point(min_x, min_y, min_z);
	glm::vec3 distance = max_point - min_point;
	float size = std::max(distance.x, std::max(distance.y, distance.z));
	m_boundingBox = NonhierBox(min_point, glm::vec3(size));
	// Note: Bounding boxes need the same animation as the mesh
}

//---------------------------------------------------------------------------------------
// Computes ray-mesh intersection and updates intersection if necessary
bool Mesh::intersect(const Ray& ray, Intersection& intersection) const
{
#ifdef RENDER_BOUNDING_VOLUMES
	// Render bounding volume instead of the mesh
	return m_boundingBox.intersect(ray, intersection);
#endif

	// Bounding box check, if it doesn't hit, we skip
	Intersection temp;
	if (!m_boundingBox.intersect(ray, temp))
	{
		return false;
	}

	// Iterate through each triangular face on the mesh
	bool intersectionFound = false;
	glm::vec3 intersectionPoint;
	glm::vec3 intersectionNormal;
	int intersectionFace;
	glm::vec2 intersectionUV;
	for (int i = 0; i < m_faces.size(); i++) {
		const Triangle& face = m_faces[i];

		// Get the verticies of this face
		glm::vec3 v1 = m_vertices[face.v1];
		glm::vec3 v2 = m_vertices[face.v2];
		glm::vec3 v3 = m_vertices[face.v3];

		// Apply displacement map if exists
		if (m_displacementMap.m_type == AnimationType::VertexDisplacement)
		{
			v1 = m_displacementMap.m_vertexDisplacement(v1, ray.time());
			v2 = m_displacementMap.m_vertexDisplacement(v2, ray.time());
			v3 = m_displacementMap.m_vertexDisplacement(v3, ray.time());
		}

		// Get position of point at this frame after animation
		glm::vec3 updatedPos;
		getFramePosition(ray.time(), glm::vec3(0.0f), updatedPos);

		glm::vec3 r = ray.transformedOrigin() - v1 - updatedPos;
		glm::vec3 x = v2 - v1;
		glm::vec3 y = v3 - v1;
		glm::vec3 z = -1.0f * ray.transformedDirection();
		
		float D = glm::determinant(glm::transpose(glm::mat3(x, y, z)));
		float D_1 = glm::determinant(glm::transpose(glm::mat3(r, y, z)));
		float D_2 = glm::determinant(glm::transpose(glm::mat3(x, r, z)));
		float D_3 = glm::determinant(glm::transpose(glm::mat3(x, y, r)));

		float beta = D_1 / D;
		float gamma = D_2 / D;
		float t = D_3 / D;

		// Check for intersection
		glm::vec3 p;
		if ((beta >= 0) && (gamma >= 0) && (beta + gamma <= 1.0f) && (t >= 0))
		{
			p = ray.transformedOrigin() + t * ray.transformedDirection();
		}
		else
		{
			continue;
		}
		
		// If an intersection has already been found check which one is closer
		if (!intersectionFound ||
				(intersectionFound &&
					(glm::length(p - ray.transformedOrigin()) < 
					glm::length(intersectionPoint - ray.transformedOrigin())))
		) {
				intersectionFace = i;
				intersectionPoint = p;
				intersectionUV = glm::vec2(beta + gamma, beta);
				intersectionNormal = glm::normalize(glm::cross(v2 - v1, v3 - v1));
		}
		intersectionFound = true;
	}

	// Output result
	if (intersectionFound)
	{
			intersection.m_point = intersectionPoint;
			intersection.m_normal = intersectionNormal;
			intersection.m_uv = intersectionUV;
	}
	return intersectionFound;
}

//---------------------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& out, const Mesh& mesh)
{
  out << "mesh {\n";

  for( size_t idx = 0; idx < mesh.m_faces.size(); ++idx ) {
  	const Triangle& v = mesh.m_faces[idx];
  	out << "  " << idx << ": {"
				<< v.v1 << " " << v.v2 << " " << v.v3
				<< "}\n";
  }

  out << "}";
  return out;
}
