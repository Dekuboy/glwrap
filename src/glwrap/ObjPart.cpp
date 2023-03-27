#include <glwrap/ObjPart.h>
#include <glwrap/TriFace.h>
#include <glwrap/VertexBuffer.h>
#include <glwrap/VertexArray.h>
#include <glwrap/ObjFrame.h>
#include <glwrap/ObjAnimation.h>
#include <glwrap/Material.h>
#include <glwrap/Context.h>
#include <glwrap/ShaderProgram.h>
#include <glwrap/TextureUniforms.h>
#include <glwrap/ViewingFrustum.h>
#include <glm/ext.hpp>

namespace glwrap
{
	ObjPart::ObjPart(std::string _name)
	{
		m_name = _name;
		m_animationUniform = glm::mat4(1);
		m_useMaterial = false;

		generateArrays();
	}

	ObjPart::~ObjPart()
	{
		glDeleteVertexArrays(m_idList.size(), &m_idList.at(0));
	}

	std::string ObjPart::getName()
	{
		return m_name;
	}

	const std::vector<TriFace*>& ObjPart::getFaces()
	{
		return m_faces;
	}

	void ObjPart::addFace(TriFace* _face)
	{
		m_faces.push_back(_face);

		if (m_faces.size() == 1)
		{
			m_maxX = std::numeric_limits<float>::min();
			m_maxY = std::numeric_limits<float>::min();
			m_maxZ = std::numeric_limits<float>::min();

			m_minX = std::numeric_limits<float>::max();
			m_minY = std::numeric_limits<float>::max();
			m_minZ = std::numeric_limits<float>::max();
		}

		if (m_faces.back()->getMaxX() > m_maxX)
		{
			m_maxX = _face->getMaxX();
		}

		if (m_faces.back()->getMaxY() > m_maxY)
		{
			m_maxY = _face->getMaxY();
		}

		if (m_faces.back()->getMaxZ() > m_maxZ)
		{
			m_maxZ = _face->getMaxZ();
		}

		if (m_faces.back()->getMinX() < m_minX)
		{
			m_minX = _face->getMinX();
		}

		if (m_faces.back()->getMinY() < m_minY)
		{
			m_minY = _face->getMinY();
		}

		if (m_faces.back()->getMinZ() < m_minZ)
		{
			m_minZ = _face->getMinZ();
		}

		m_offsetX = (m_minX + m_maxX) / 2;
		m_offsetY = (m_minY + m_maxY) / 2;
		m_offsetZ = (m_minZ + m_maxZ) / 2;

		m_dirty = true;
	}

	VertexBuffer* ObjPart::setBuffer(std::string _attribute, int _materialId)
	{
		int pos = 0;
		if (_attribute == "in_Position")
		{	
		}
		else if (_attribute == "in_Color")
		{
			pos = 1;
		}
		else if (_attribute == "in_TexCoord")
		{
			pos = 2;
		}
		else if (_attribute == "in_Normal")
		{
			pos = 3;
		}
		else if (_attribute == "in_JointIDs")
		{
			pos = 4;
		}
		else if (_attribute == "in_Weights")
		{
			pos = 5;
		}
		else if (_attribute == "in_Tangent")
		{
			pos = 6;

		}
		else if (_attribute == "in_Bitangent")
		{
			pos = 7;
		}
		else
		{
			throw std::exception();
		}

		m_dirty = true;
		return &m_buffers.at(_materialId).at(pos);
	}

	int ObjPart::getVertexCount(int _materialId)
	{
		if (m_buffers.size() < 1)
		{
			throw std::exception();
		}

		return m_buffers.at(_materialId).at(0).getDataSize() / m_buffers.at(_materialId).at(0).getComponents();
	}

	GLuint ObjPart::getId(int _materialId)
	{
		if (m_dirty)
		{
			for (int matId = 0; matId < m_idList.size(); matId++)
			{
				glBindVertexArray(m_idList.at(matId));
				for (int i = 0; i < m_buffers.at(matId).size(); i++)
				{
					if (m_buffers.at(matId).at(i).getComponents() != 0)
					{
						glBindBuffer(GL_ARRAY_BUFFER, m_buffers.at(matId).at(i).getId());

						glVertexAttribPointer(i, m_buffers.at(matId).at(i).getComponents(), GL_FLOAT, GL_FALSE,
							m_buffers.at(matId).at(i).getComponents() * sizeof(GLfloat), (void*)0);

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

	void ObjPart::draw()
	{
		glm::vec3 translateVector(m_offsetX, m_offsetY, m_offsetZ);

		m_animationUniform = glm::translate(m_animationUniform, translateVector);
		translate();
		m_animationUniform = glm::translate(m_animationUniform, -translateVector);

		drawArrays();
	}

	void ObjPart::cullAndDraw(bool _cullAnimated)
	{
		glm::vec3 translateVector(m_offsetX, m_offsetY, m_offsetZ);
		ShaderProgram* shader = m_context->getCurrentShader();

		m_animationUniform = glm::translate(m_animationUniform, translateVector);
		translate();
		glm::vec3 partCentre = m_animationUniform * glm::vec4(0, 0, 0, 1);
		m_animationUniform = glm::translate(m_animationUniform, -translateVector);

		if (_cullAnimated)
		{
			bool inView = shader->checkModelInView(), 
				isIdentity = m_animationUniform == glm::mat4(1);
			if (isIdentity)
			{
				if (inView)
				{
					drawArrays();
				}
			}
			else
			{
				glm::mat3 partRotation = glm::mat3(m_animationUniform);
				glm::vec3 partSize = getSize();

				if (shader->
					checkViewingFrustum(partCentre, partSize, partRotation))
				{
					drawArrays();
				}
				else
				{
					m_animationUniform = glm::mat4(1);
				}
			}
		}
		else
		{
			glm::mat3 partRotation = glm::mat3(m_animationUniform);
			glm::vec3 partSize = getSize();

			if (shader->
				checkViewingFrustum(partCentre, partSize, partRotation))
			{
				drawArrays();
			}
			else
			{
				m_animationUniform = glm::mat4(1);
			}
		}
	}

	glm::vec3 ObjPart::getSize()
	{
		return glm::vec3(m_maxX - m_minX, m_maxY - m_minY, m_maxZ - m_minZ);
	}

	glm::vec3 ObjPart::getCentre()
	{
		return glm::vec3(m_offsetX, m_offsetY, m_offsetZ);
	}

	void ObjPart::translate()
	{
		ObjFrame* frame;
		ObjTranslation* translation;

		glm::vec3 translateVector(0);

		for (std::vector<ObjAnimation>::iterator itr = m_animationList->begin();
			itr != m_animationList->end(); itr++)
		{
			if (itr->getEnabled())
			{
				frame = itr->getMergeFrame();
				translation = frame->getTranslation(m_self);

				if (translation)
				{
					translateVector = glm::vec3(translation->getX(), translation->getY(),
						translation->getZ());
					m_animationUniform = glm::translate(m_animationUniform, translateVector);

					m_animationUniform = glm::rotate(m_animationUniform,
						translation->getZRotation(), glm::vec3(0, 0, 1));
					m_animationUniform = glm::rotate(m_animationUniform,
						translation->getYRotation(), glm::vec3(0, 1, 0));
					m_animationUniform = glm::rotate(m_animationUniform,
						translation->getXRotation(), glm::vec3(1, 0, 0));
				}
			}
		}
	}

	void ObjPart::drawArrays()
	{
		ShaderProgram* shader = m_context->
			getCurrentShader();

		bool check = false;

		if (m_animationUniform != glm::mat4(1))
		{
			shader->setUniform("in_Animate", m_animationUniform);
			check = true;
		}

		if (m_useMaterial)
		{
			std::string textureCheck;

			shader->getUniforms()->getUniformName(0, textureCheck);

			int listItr = 0;
			for (std::vector<Material*>::iterator itr = m_materials.begin();
				itr != m_materials.end(); itr++)
			{
				if ((*itr)->m_colourMap)
				{
					if (textureCheck != "")
					{
						shader->setUniform(textureCheck, (*itr)->m_colourMap);
					}
				}

				glBindVertexArray(getId(listItr));
				glDrawArrays(GL_TRIANGLES, 0, getVertexCount(listItr));
				listItr++;
			}
		}
		else
		{
			glBindVertexArray(getId(0));
			glDrawArrays(GL_TRIANGLES, 0, getVertexCount(0));
		}

		if (check)
		{
			m_animationUniform = glm::mat4(1);
			shader->setUniform("in_Animate", m_animationUniform);
		}
	}

	void ObjPart::generateArrays()
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