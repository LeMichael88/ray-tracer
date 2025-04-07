#pragma once

#include <iostream>
#include <list>
#include <string>
#include <vector>

#include <glm/glm.hpp>

#include "animation/Animation.hpp"
#include "core/Ray.hpp"
#include "materials/Material.hpp"

enum class NodeType {
	SceneNode,
	GeometryNode,
	JointNode,
  ParticleNode,
};

class SceneNode {
public:
  SceneNode(const std::string & name);

	SceneNode(const SceneNode & other);

  virtual ~SceneNode();
    
	int totalSceneNodes() const;
    
  const glm::mat4& get_transform() const;
  const glm::mat4& get_inverse() const;
  
  void set_transform(const glm::mat4& m);
  
  void add_child(SceneNode* child);
  
  void remove_child(SceneNode* child);

	//-- Transformations:
  void rotate(char axis, float angle);
  void scale(const glm::vec3& amount);
  void translate(const glm::vec3& amount);

	void setAnimation(Animation* animation);
	void animateNode(const float t);
	void resetAnimation();

	friend std::ostream & operator << (std::ostream & os, const SceneNode & node);

  virtual Intersection intersect(const Ray& ray) const;

  // Transformations
  glm::mat4 trans;
  glm::mat4 invtrans;

  glm::mat4 m_originalTrans;
	std::vector<Animation*> m_animations;

  std::list<SceneNode*> children;

	NodeType m_nodeType;
	std::string m_name;
	unsigned int m_nodeId;

private:
	// The number of SceneNode instances.
	static unsigned int nodeInstanceCount;
};
