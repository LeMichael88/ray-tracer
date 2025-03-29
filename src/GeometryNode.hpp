#pragma once

#include "Animation.hpp"
#include "Material.hpp"
#include "Primitive.hpp"
#include "SceneNode.hpp"

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
