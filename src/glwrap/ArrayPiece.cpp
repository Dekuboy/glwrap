#include <glwrap/ArrayPiece.h>

namespace glwrap
{
	std::string ArrayPiece::getName()
	{
		return m_name;
	}

	const std::vector<TriFace*>& ArrayPiece::getFaces()
	{
		return m_faces;
	}

	int ArrayPiece::getVertexCount(int _materialId)
	{
		if (m_buffers.size() < 1)
		{
			throw std::exception();
		}

		return m_buffers.at(_materialId).at(0).getDataSize() / m_buffers.at(_materialId).at(0).getComponents();
	}

	GLuint ArrayPiece::getId(int _materialId)
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
					if (check.getId())
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

	glm::vec3 ArrayPiece::getSize()
	{
		return m_maxPoint - m_minPoint;
	}

	glm::vec3 ArrayPiece::getCentre()
	{
		return m_offset;
	}

	ArrayPiece::ArrayPiece()
	{
		m_context = nullptr;
		m_dirty = false;
		m_animationUniform = glm::mat4(1);
	}

	ArrayPiece::~ArrayPiece()
	{

	}

	void ArrayPiece::addFace(TriFace* _face)
	{
		m_faces.push_back(_face);

		m_dirty = true;
	}
	
	void ArrayPiece::drawArrays()
	{

	}

	void ArrayPiece::generateArrays()
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

	VertexBuffer* ArrayPiece::setBuffer(std::string _attribute, int _materialId)
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

	void ArrayPiece::draw()
	{
		drawArrays();
	}

	void ArrayPiece::cullAndDraw()
	{

	}
}
