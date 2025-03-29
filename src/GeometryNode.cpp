#include "GeometryNode.hpp"

//---------------------------------------------------------------------------------------
GeometryNode::GeometryNode(
	const std::string & name, Primitive *prim, Material *mat )
	: SceneNode(name),
		m_material(mat),
		m_primitive(prim)
{
	m_nodeType = NodeType::GeometryNode;
}

void GeometryNode::setMaterial( Material *mat )
{
	// Obviously, there's a potential memory leak here.  A good solution
	// would be to use some kind of reference counting, as in the 
	// C++ shared_ptr.  But I'm going to punt on that problem here.
	// Why?  Two reasons:
	// (a) In practice we expect the scene to be constructed exactly
	//     once.  There's no reason to believe that materials will be
	//     repeatedly overwritten in a GeometryNode.
	// (b) A ray tracer is a program in which you compute once, and 
	//     throw away all your data.  A memory leak won't build up and
	//     crash the program.

	m_material = mat;
}

//---------------------------------------------------------------------------------------
// Sets the displacement map of this node's primitive
void GeometryNode::setDisplacementMap(Animation *displacementMap)
{
	m_primitive->m_displacementMap = *displacementMap;
}

//---------------------------------------------------------------------------------------
// Checks if this geometry's m_primitive intersects with the ray
// Returns an Intersection (in model coords) if an intersection exists
Intersection GeometryNode::intersect(const Ray& ray) const
{
	Intersection intersection;
	
	// If this node intersected with the ray, update intersection values
	if(m_primitive->intersect(ray, intersection))
	{
		// Found intersection, update material, normal, uv coordinates
		intersection.m_material = static_cast<PhongMaterial*>(m_material);
		m_primitive->normal(intersection.m_point, ray.time(), intersection.m_normal);
		m_primitive->getUV(intersection.m_point, ray.time(), intersection.m_uv);
		intersection.m_foundIntersection = true;
	}

	return intersection;
}