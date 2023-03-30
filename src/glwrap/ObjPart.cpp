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
		m_useMaterial = false;

		generateArrays();
	}

	ObjPart::~ObjPart()
	{
		glDeleteVertexArrays(m_idList.size(), &m_idList.at(0));
	}

	void ObjPart::addFace(TriFace* _face)
	{
		if (!m_faces.size())
		{
			m_maxPoint.x = std::numeric_limits<float>::min();
			m_maxPoint.y = std::numeric_limits<float>::min();
			m_maxPoint.z = std::numeric_limits<float>::min();

			m_minPoint.x = std::numeric_limits<float>::max();
			m_minPoint.y = std::numeric_limits<float>::max();
			m_minPoint.z = std::numeric_limits<float>::max();
		}

		m_faces.push_back(_face);

		if (m_faces.back()->getMaxX() > m_maxPoint.x)
		{
			m_maxPoint.x = _face->getMaxX();
		}

		if (m_faces.back()->getMaxY() > m_maxPoint.y)
		{
			m_maxPoint.y = _face->getMaxY();
		}

		if (m_faces.back()->getMaxZ() > m_maxPoint.z)
		{
			m_maxPoint.z = _face->getMaxZ();
		}

		if (m_faces.back()->getMinX() < m_minPoint.x)
		{
			m_minPoint.x = _face->getMinX();
		}

		if (m_faces.back()->getMinY() < m_minPoint.y)
		{
			m_minPoint.y = _face->getMinY();
		}

		if (m_faces.back()->getMinZ() < m_minPoint.z)
		{
			m_minPoint.z = _face->getMinZ();
		}

		m_offset = (m_minPoint + m_maxPoint) / 2.0f;

		m_dirty = true;
	}

	void ObjPart::draw()
	{
		m_animationUniform = glm::translate(m_animationUniform, m_offset);
		translate();
		m_animationUniform = glm::translate(m_animationUniform, -m_offset);

		drawArrays();
	}

	void ObjPart::cullAndDraw(bool _cullAnimated)
	{
		ShaderProgram* shader = m_context->getCurrentShader();

		m_animationUniform = glm::translate(m_animationUniform, m_offset);
		translate();
		glm::vec3 partCentre = m_animationUniform * glm::vec4(0, 0, 0, 1);
		m_animationUniform = glm::translate(m_animationUniform, -m_offset);

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
}