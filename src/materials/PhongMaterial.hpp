/**
 * Name: PhongMaterial.hpp
 * Description: Material class for rendering that uses the Phong shading model.
 */

#pragma once

#include <glm/glm.hpp>

#include "utils/Image.hpp"
#include "materials/Material.hpp"

// In the context of this file, vec3 is used to represent colors.
using Color = glm::vec3;

/**
 * PhongMaterial class implements the Phong shading model, which is a local illumination
 * model using a diffuse, specular, and reflection component.
 */
class PhongMaterial : public Material {
public:
    PhongMaterial(
        const glm::vec3& kd,
        const glm::vec3& ks,
        double shininess,
        const glm::vec3& kr = glm::vec3(0.0f)
    );
    ~PhongMaterial() override;

    // Getters for coefficients
    [[nodiscard]] virtual glm::vec3 kd(const glm::vec2& uv) const { return m_kd; };
    [[nodiscard]] virtual glm::vec3 ks(const glm::vec2& uv) const { return m_ks; };
    [[nodiscard]] virtual glm::vec3 kr(const glm::vec2& uv) const { return m_kr; };
    [[nodiscard]] double shininess() const { return m_shininess; };

protected:
    glm::vec3 m_kd;
    glm::vec3 m_ks;
    glm::vec3 m_kr;
    double m_shininess;
};

/**
 * PhongTexture class extends the PhongMaterial class to include texture mapping.
 * It uses a texture map to fetch the colour, and applies it to the diffuse, specular,
 * and reflection components.
 */
class PhongTexture final : public PhongMaterial {
public:
    PhongTexture(
        const glm::vec3& kd,
        const glm::vec3& ks,
        const glm::vec3& kr,
        double shininess,
        const std::string& textureFile
    );
    ~PhongTexture() override;

    [[nodiscard]] glm::vec3 kd(const glm::vec2& uv) const override;
    [[nodiscard]] glm::vec3 ks(const glm::vec2& uv) const override;
    [[nodiscard]] glm::vec3 kr(const glm::vec2& uv) const override;
    [[nodiscard]] Color getColorFromMap(const glm::vec2& uv) const;

private:
    Image textureMap;
};
