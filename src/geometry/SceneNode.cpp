#include "SceneNode.hpp"

#include <sstream>
using namespace std;

#include <glm/ext.hpp>
#include <glm/gtx/transform.hpp>
using namespace glm;

#include "graphics-framework/MathUtils.hpp"

// Static class variable
unsigned int SceneNode::nodeInstanceCount = 0;


//---------------------------------------------------------------------------------------
SceneNode::SceneNode(std::string name)
    : m_name(std::move(name)),
      m_nodeType(NodeType::SceneNode),
      trans(mat4()),
      invtrans(mat4()),
      m_nodeId(nodeInstanceCount++)
{}

//---------------------------------------------------------------------------------------
// Deep copy
SceneNode::SceneNode(const SceneNode& other)
    : m_nodeType(other.m_nodeType),
      m_name(other.m_name),
      trans(other.trans),
      invtrans(other.invtrans)
{
    for (SceneNode* child: other.children)
    {
        this->children.push_front(new SceneNode(*child));
    }
}

//---------------------------------------------------------------------------------------
SceneNode::~SceneNode()
{
    for (SceneNode* child: children)
    {
        delete child;
    }
}

//---------------------------------------------------------------------------------------
void SceneNode::set_transform(const glm::mat4& m)
{
    trans = m;
    invtrans = glm::inverse(m);
}

//---------------------------------------------------------------------------------------
const glm::mat4& SceneNode::get_transform() const
{
    return trans;
}

//---------------------------------------------------------------------------------------
const glm::mat4& SceneNode::get_inverse() const
{
    return invtrans;
}

//---------------------------------------------------------------------------------------
void SceneNode::add_child(SceneNode* child)
{
    children.push_back(child);
}

//---------------------------------------------------------------------------------------
void SceneNode::remove_child(SceneNode* child)
{
    children.remove(child);
}

//---------------------------------------------------------------------------------------
void SceneNode::rotate(char axis, float angle)
{
    vec3 rot_axis;

    switch (axis)
    {
        case 'x':
            rot_axis = vec3(1, 0, 0);
            break;
        case 'y':
            rot_axis = vec3(0, 1, 0);
            break;
        case 'z':
            rot_axis = vec3(0, 0, 1);
            break;
        default:
            break;
    }
    mat4 rot_matrix = glm::rotate(degreesToRadians(angle), rot_axis);
    set_transform(rot_matrix * trans);
}

//---------------------------------------------------------------------------------------
void SceneNode::scale(const glm::vec3& amount)
{
    set_transform(glm::scale(amount) * trans);
}

//---------------------------------------------------------------------------------------
void SceneNode::translate(const glm::vec3& amount)
{
    set_transform(glm::translate(amount) * trans);
}

//---------------------------------------------------------------------------------------
// Adds an animation to this node
void SceneNode::setAnimation(Animation* animation)
{
    m_animations.push_back(animation);
}

//---------------------------------------------------------------------------------------
// Animates a node for this frame by displacing it using m_animations
void SceneNode::animateNode(const float t)
{
    // Store the transformation matrix before animation so we can reset it
    m_originalTrans = trans;

    // Loop through vector of animations (assumed to be in chronological order)
    for (const Animation* animation: m_animations)
    {
        // Translations
        if (animation->m_type == AnimationType::Translate)
        {
            if (t < animation->m_start) // Stop if this animation is not reached yet
            {
                break;
            }

            if (t > animation->m_end) // Full translation if animation is completed
            {
                translate(animation->m_animation(animation->m_end - animation->m_start));
            }
            else // We are in the middle of this animation
            {
                translate(animation->m_animation(t - animation->m_start));
            }
        }
        else if (animation->m_type == AnimationType::Rotate) // Rotations
        {
            if (t < animation->m_start) // Stop if this animation is not reached yet
            {
                break;
            }
            else if (t > animation->m_end) // Full translation if animation is completed
            {
                rotate('y', animation->m_scalarAnimation(animation->m_end - animation->m_start));
            }
            else // We are in the middle of this animation
            {
                rotate('y', animation->m_scalarAnimation(t - animation->m_start));
            }
        }
    }
}

//---------------------------------------------------------------------------------------
// Resets this node's transformation to its original position
void SceneNode::resetAnimation()
{
    set_transform(m_originalTrans);
}

//---------------------------------------------------------------------------------------
int SceneNode::totalSceneNodes() const
{
    return nodeInstanceCount;
}

//---------------------------------------------------------------------------------------
std::ostream& operator <<(std::ostream& os, const SceneNode& node)
{
    //os << "SceneNode:[NodeType: ___, name: ____, id: ____, isSelected: ____, transform: ____"
    switch (node.m_nodeType)
    {
        case NodeType::SceneNode:
            os << "SceneNode";
            break;
        case NodeType::GeometryNode:
            os << "GeometryNode";
            break;
        case NodeType::JointNode:
            os << "JointNode";
            break;
        case NodeType::ParticleNode:
            os << "ParticleNode";
            break;
        default:
            break;
    }
    os << ":[";

    os << "name:" << node.m_name << ", ";
    os << "id:" << node.m_nodeId;

    os << "]\n";
    return os;
}

//---------------------------------------------------------------------------------------
// Checks if this geometry intersects with the ray
// For a SceneNode without geometry, no intersection is possible
Intersection SceneNode::intersect(const Ray& ray) const
{
    return Intersection();
}
