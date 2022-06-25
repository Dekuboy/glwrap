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
		m_animationUniform = nullptr;
	}

	ModelJoint::~ModelJoint()
	{
		glDeleteVertexArrays(m_idList.size(), &m_idList.at(0));
	}

	std::string ModelJoint::getName()
	{
		return m_name;
	}

	const std::vector<TriFace*>& ModelJoint::getFaces()
	{
		return m_faces;
	}

	void ModelJoint::addFace(TriFace* _face)
	{
		m_faces.push_back(_face);

		m_dirty = true;
	}

	VertexBuffer* ModelJoint::setBuffer(std::string _attribute, int _materialId)
	{
		if (_attribute == "in_Position")
		{
			return &m_buffers.at(_materialId).at(0);
		}
		else if (_attribute == "in_Color")
		{
			return &m_buffers.at(_materialId).at(1);
		}
		else if (_attribute == "in_TexCoord")
		{
			return &m_buffers.at(_materialId).at(2);
		}
		else if (_attribute == "in_Normal")
		{
			return &m_buffers.at(_materialId).at(3);
		}
		else if (_attribute == "in_JointIDs")
		{
			return &m_buffers.at(_materialId).at(4);
		}
		else if (_attribute == "in_Weights")
		{
			return &m_buffers.at(_materialId).at(5);
		}
		else if (_attribute == "in_Tangent")
		{
			return &m_buffers.at(_materialId).at(6);
		}
		else if (_attribute == "in_Bitangent")
		{
			return &m_buffers.at(_materialId).at(7);
		}
		else
		{
			throw std::exception();
		}

		m_dirty = true;
	}

	int ModelJoint::getVertexCount(int _materialId)
	{
		if (m_buffers.size() < 1)
		{
			throw std::exception();
		}

		return m_buffers.at(_materialId).at(0).getDataSize() / m_buffers.at(_materialId).at(0).getComponents();
	}

	GLuint ModelJoint::getId(int _materialId)
	{
		if (m_dirty)
		{
			VertexBuffer check;
			for (int matId = 0; matId < m_idList.size(); matId++)
			{
				glBindVertexArray(m_idList.at(matId));
				for (int i = 0; i < m_buffers.at(matId).size(); i++)
				{
					check = m_buffers.at(matId).at(i);
					if (check.getComponents() != 0)
					{
						glBindBuffer(GL_ARRAY_BUFFER, check.getId());

						glVertexAttribPointer(i, check.getComponents(), GL_FLOAT, GL_FALSE,
							check.getComponents() * sizeof(GLfloat), (void*)0);

						glEnableVertexAttribArray(i);
					}
					else
					{
						glDisableVertexAttribArray(i);
					}
				}
			}

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
			m_dirty = false;
		}

		return m_idList.at(_materialId);
	}

	void ModelJoint::draw()
	{
		drawArrays();
	}

	void ModelJoint::cullAndDraw()
	{
		bool check = false;
		glm::vec3 partCentre = m_minPoint + 0.5f * m_size;
		glm::vec3 partSize = getSize();
		ShaderProgram* shader = m_context->getCurrentShader();
		if (m_animationUniform)
		{
			glm::mat4 partMatrix = *m_animationUniform;
			partCentre = partMatrix * glm::vec4(partCentre, 1);
			glm::mat3 partRotation = glm::mat3(*m_animationUniform);

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

	glm::vec3 ModelJoint::getSize()
	{
		return m_size;
	}

	glm::vec3 ModelJoint::getCentre()
	{
		return m_minPoint + 0.5f * m_size;
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

	void ModelJoint::generateArrays()
	{
		m_buffers.resize(m_buffers.size() + 1);
		m_idList.resize(m_idList.size() + 1);

		glGenVertexArrays(1, &m_idList.back());

		if (!m_idList.back())
		{
			throw std::exception();
		}

		m_buffers.back().resize(8);
	}
}