#include <glwrap/ModelJoint.h>
#include <glwrap/TriFace.h>
#include <glwrap/VertexBuffer.h>
#include <glwrap/GltfModel.h>
//#include <glwrap/ModelFrame.h>
//#include <glwrap/ModelAnimation.h>
#include <glwrap/Material.h>
#include <glwrap/Context.h>
#include <glwrap/ShaderProgram.h>
#include <glwrap/TextureUniforms.h>
#include <glwrap/ViewingFrustum.h>
#include <glm/ext.hpp>

namespace glwrap
{
	ModelJoint::ModelJoint(std::string _name)
	{
		m_name = _name;
		m_minPoint = glm::vec3(std::numeric_limits<float>::max());
		m_maxPoint = glm::vec3(std::numeric_limits<float>::min());
	}

	ModelJoint::~ModelJoint()
	{
		glDeleteVertexArrays(m_idList.size(), &m_idList.at(0));
	}

	void ModelJoint::checkMinMax(glm::vec3& _vertexPosition)
	{
		if (m_minPoint.x > m_maxPoint.x)
		{
			m_minPoint = _vertexPosition;
			m_maxPoint = _vertexPosition;
			return;
		}
		if (_vertexPosition.x < m_minPoint.x)
		{
			m_minPoint.x = _vertexPosition.x;
		}
		else if (_vertexPosition.x > m_maxPoint.x)
		{
			m_maxPoint.x = _vertexPosition.x;
		}
		if (_vertexPosition.y < m_minPoint.y)
		{
			m_minPoint.y = _vertexPosition.y;
		}
		else if (_vertexPosition.y > m_maxPoint.y)
		{
			m_maxPoint.y = _vertexPosition.y;
		}
		if (_vertexPosition.z < m_minPoint.z)
		{
			m_minPoint.z = _vertexPosition.z;
		}
		else if (_vertexPosition.z > m_maxPoint.z)
		{
			m_maxPoint.z = _vertexPosition.z;
		}
	}

	void ModelJoint::checkMin(glm::vec3& _vertexPosition)
	{
		if (_vertexPosition.x < m_minPoint.x)
		{
			m_minPoint.x = _vertexPosition.x;
		}
		if (_vertexPosition.y < m_minPoint.y)
		{
			m_minPoint.y = _vertexPosition.y;
		}
		if (_vertexPosition.z < m_minPoint.z)
		{
			m_minPoint.z = _vertexPosition.z;
		}
	}

	void ModelJoint::checkMax(glm::vec3& _vertexPosition)
	{
		if (_vertexPosition.x > m_maxPoint.x)
		{
			m_maxPoint.x = _vertexPosition.x;
		}
		if (_vertexPosition.y > m_maxPoint.y)
		{
			m_maxPoint.y = _vertexPosition.y;
		}
		if (_vertexPosition.z > m_maxPoint.z)
		{
			m_maxPoint.z = _vertexPosition.z;
		}
	}

	void ModelJoint::drawArrays()
	{
		ShaderProgram* shader = m_context->
			getCurrentShader();

		std::string textureCheck[5];

		for (int i = 0; i < 5; i++)
		{
			shader->getUniforms()->getUniformName(i, textureCheck[i]);
		}

		int listItr = 0;
		for (std::vector<Material*>::iterator itr = m_materials.begin();
			itr != m_materials.end(); itr++)
		{
			if (*itr)
			{
				if ((*itr)->m_colourMap)
				{
					if (textureCheck[0] != "")
					{
						shader->setUniform(textureCheck[0], (*itr)->m_colourMap);
					}
				}
			}

			glBindVertexArray(getId(listItr));
			glDrawArrays(GL_TRIANGLES, 0, getVertexCount(listItr));
			listItr++;
		}
	}

	void ModelJoint::cullAndDraw()
	{
		bool check = false;
		glm::vec3 partCentre = m_minPoint + 0.5f * getSize();
		glm::vec3 partSize = getSize();
		ShaderProgram* shader = m_context->getCurrentShader();
		if (m_animationUniform != glm::mat4(1))
		{
			partCentre = m_animationUniform * glm::vec4(partCentre, 1);
			glm::mat3 partRotation = glm::mat3(m_animationUniform);

			check = shader->
				checkViewingFrustum(partCentre, partSize, partRotation);
		}
		else
		{
			check = shader->
				getViewingFrustum()->checkCube(partCentre, partSize);
		}

		if (check)
		{
			drawArrays();
		}
	}
}