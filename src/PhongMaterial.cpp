#include "PhongMaterial.hpp"

#include <iostream>

#include <glm/gtx/io.hpp>

//---------------------------------------------------------------------------------------
PhongMaterial::PhongMaterial(
  const glm::vec3& kd, const glm::vec3& ks, double shininess, const glm::vec3& kr)
	: m_kd(kd),
		m_ks(ks),
		m_shininess(shininess),
		m_kr(kr)
{}

//---------------------------------------------------------------------------------------
PhongMaterial::~PhongMaterial()
{}

//---------------------------------------------------------------------------------------
PhongTexture::PhongTexture(
	const glm::vec3& kd, 
	const glm::vec3& ks,
	const glm::vec3& kr, 
	double shininess,
	std::string textureFile
) 
	: PhongMaterial(kd, ks, shininess, kr),
		textureMap(Image())
{
	textureMap.readPng(textureFile);
}

//---------------------------------------------------------------------------------------
PhongTexture::~PhongTexture()
{}

//---------------------------------------------------------------------------------------
glm::vec3 PhongTexture::kd(const glm::vec2& uv) const
{
	return m_kd * getColorFromMap(uv);
}

//---------------------------------------------------------------------------------------
glm::vec3 PhongTexture::ks(const glm::vec2& uv) const
{
	return m_ks * getColorFromMap(uv);
}

//---------------------------------------------------------------------------------------
glm::vec3 PhongTexture::kr(const glm::vec2& uv) const
{
	return m_kr * getColorFromMap(uv);
}

//---------------------------------------------------------------------------------------
// Gets the color from texture map given (u, v) coordinates (where u, v in [0, 1])
Color PhongTexture::getColorFromMap(const glm::vec2& uv) const
{
	// Given u, v in [0, 1], get i, j in [0, 1] for the texture map
	float d_i = (textureMap.width() - 1) * uv.x;
	float d_j = (textureMap.height() - 1) * uv.y;
	int i = static_cast<int>(d_i);
	int j = static_cast<int>(d_j);
	float u_p = d_i - i;
	float v_p = d_j - j;

	// Obtain color of surrounding pixels
	Color C_00 = textureMap(i, j);
	Color C_01 = textureMap(i, j + 1);
	Color C_10 = textureMap(i + 1, j);
	Color C_11 = textureMap(i + 1, j + 1);

	// Interpolate color and return
	Color interpolated = C_00 * (1.0f - u_p) * (1.0f - v_p) +
											 C_01 * (1.0f - u_p) * v_p +
											 C_10 * u_p * (1.0f - v_p) +
											 C_11 * u_p * v_p;
	return interpolated;
}

