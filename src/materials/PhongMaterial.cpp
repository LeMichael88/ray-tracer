#include "PhongMaterial.hpp"

#include <glm/gtx/io.hpp>

//---------------------------------------------------------------------------------------
/**
 * PhongMaterial constructor initializes the material with given diffuse, specular,
 * and reflection coefficients, and shininess exponent.
 * @param kd Diffuse coefficient
 * @param ks Specular coefficient
 * @param shininess Shininess exponent
 * @param kr Reflection coefficient (default is black)
 */
PhongMaterial::PhongMaterial(
    const glm::vec3& kd, const glm::vec3& ks, double shininess, const glm::vec3& kr)
    : m_kd(kd),
      m_ks(ks),
      m_kr(kr),
      m_shininess(shininess)
{}

//---------------------------------------------------------------------------------------
/**
 * Default destructor for PhongMaterial class.
 */
PhongMaterial::~PhongMaterial()
= default;

//---------------------------------------------------------------------------------------
/**
 * PhongTexture constructor initializes the material using the PhongMaterial constructor
 * and loads the texture map from the specified file.
 * @param kd Diffuse coefficient
 * @param ks Specular coefficient
 * @param kr Reflection coefficient
 * @param shininess Shininess exponent
 * @param textureFile Path to the texture file
 */
PhongTexture::PhongTexture(
    const glm::vec3& kd,
    const glm::vec3& ks,
    const glm::vec3& kr,
    const double shininess,
    const std::string& textureFile
)
    : PhongMaterial(kd, ks, shininess, kr),
      textureMap(Image())
{
    textureMap.readPng(textureFile);
}

//---------------------------------------------------------------------------------------
/**
 * Default destructor for PhongTexture class.
 */
PhongTexture::~PhongTexture()
= default;

//---------------------------------------------------------------------------------------
/**
 * kd gets the diffuse coefficient for the material, which is a product of the base
 * diffuse coefficient and the color from the texture map at the given (u, v) coordinates.
 * @param uv Texture coordinates in [0, 1]
 * @return Diffuse coefficient
 */
glm::vec3 PhongTexture::kd(const glm::vec2& uv) const
{
    return m_kd * getColorFromMap(uv);
}

//---------------------------------------------------------------------------------------
/**
 * ks gets the specular coefficient for the material, which is a product of the base
 * specular coefficient and the color from the texture map at the given (u, v) coordinates.
 * @param uv Texture coordinates in [0, 1]
 * @return Specular coefficient
 */
glm::vec3 PhongTexture::ks(const glm::vec2& uv) const
{
    return m_ks * getColorFromMap(uv);
}

//---------------------------------------------------------------------------------------
/**
 * kr gets the reflection coefficient for the material, which is a product of the base
 * reflection coefficient and the color from the texture map at the given (u, v) coordinates.
 * @param uv Texture coordinates in [0, 1]
 * @return Reflection coefficient
 */
glm::vec3 PhongTexture::kr(const glm::vec2& uv) const
{
    return m_kr * getColorFromMap(uv);
}

//---------------------------------------------------------------------------------------
/**
 * getColorFromMap retrieves the color from the texture map at the given (u, v) coordinates.
 * It performs bi-linear interpolation to obtain the color value.
 * @param uv Texture coordinates in [0, 1]
 * @return Color from the texture map
 */
Color PhongTexture::getColorFromMap(const glm::vec2& uv) const
{
    // Given u, v in [0, 1], get i, j in [0, 1] for the texture map
    const float d_i = (static_cast<float>(textureMap.width()) - 1.0f) * uv.x;
    const float d_j = (static_cast<float>(textureMap.height()) - 1.0f) * uv.y;
    const int i = static_cast<int>(d_i);
    const int j = static_cast<int>(d_j);
    const float u_p = d_i - static_cast<float>(i);
    const float v_p = d_j - static_cast<float>(j);

    // Obtain color of surrounding pixels
    const Color C_00 = textureMap(i, j);
    const Color C_01 = textureMap(i, j + 1);
    const Color C_10 = textureMap(i + 1, j);
    const Color C_11 = textureMap(i + 1, j + 1);

    // Interpolate color and return
    const Color interpolated = C_00 * (1.0f - u_p) * (1.0f - v_p) +
                               C_01 * (1.0f - u_p) * v_p +
                               C_10 * u_p * (1.0f - v_p) +
                               C_11 * u_p * v_p;
    return interpolated;
}
