#pragma once

#include <glm/glm.hpp>

#include "utils/Image.hpp"
#include "materials/Material.hpp"

using Color = glm::vec3;

class PhongMaterial : public Material {
public:
  PhongMaterial(
    const glm::vec3& kd, 
    const glm::vec3& ks,
    double shininess,
    const glm::vec3& kr = glm::vec3(0.0f)
  );
  virtual ~PhongMaterial();

  // Getters for coefficients
  virtual glm::vec3 kd(const glm::vec2& uv) const { return m_kd; };
  virtual glm::vec3 ks(const glm::vec2& uv) const { return m_ks; };
  virtual glm::vec3 kr(const glm::vec2& uv) const { return m_kr; };
  double shininess() const { return m_shininess; };

protected:
  glm::vec3 m_kd;
  glm::vec3 m_ks;
  glm::vec3 m_kr;
  double m_shininess;
};

class PhongTexture: public PhongMaterial {
public:
  PhongTexture(
    const glm::vec3& kd, 
    const glm::vec3& ks,
    const glm::vec3& kr, 
    double shininess,
    std::string textureFile
  );
  virtual ~PhongTexture();

  virtual glm::vec3 kd(const glm::vec2& uv) const;
  virtual glm::vec3 ks(const glm::vec2& uv) const;
  virtual glm::vec3 kr(const glm::vec2& uv) const;

  Color getColorFromMap(const glm::vec2& uv) const;

private:
  Image textureMap;
};
