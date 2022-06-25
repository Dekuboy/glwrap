#include "Material.h"


namespace glwrap
{
	std::string Material::getTexturePath()
	{
		return m_colourPath;
	}

	void Material::setTexture(Texture* _texture)
	{
		m_colourMap = _texture;
	}

	std::string Material::getMdetalRoughPath()
	{
		return m_metalRoughPath;
	}

	void Material::setMetalRough(Texture* _texture)
	{
		m_metalRoughMap = _texture;
	}

	std::string Material::getNormalPath()
	{
		return m_normalPath;
	}
	void Material::setNormal(Texture* _texture)
	{
		m_normalMap = _texture;
	}

	std::string Material::getOcclusionPath()
	{
		return m_occlusionPath;
	}

	void Material::setOcclusion(Texture* _texture)
	{
		m_occlusionMap = _texture;
	}

	std::string Material::getEmissivePath()
	{
		return m_emissivePath;
	}

	void Material::setEmissive(Texture* _texture)
	{
		m_emissiveMap = _texture;
	}
}