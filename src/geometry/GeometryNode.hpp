#pragma once

#include "animation/Animation.hpp"
#include "materials/Material.hpp"
#include "geometry/Primitive.hpp"
#include "geometry/SceneNode.hpp"

class GeometryNode : public SceneNode {
public:
	GeometryNode(
		const std::string& name,
		Primitive* prim, 
		Material* mat = nullptr
	);

	void setMaterial(Material* material);
	void setDisplacementMap(Animation* displacementMap);

	virtual Intersection intersect(const Ray& ray) const;

	Material* m_material;
	Primitive* m_primitive;
};
