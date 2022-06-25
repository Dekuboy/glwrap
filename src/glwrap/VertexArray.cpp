#include <glwrap/VertexArray.h>
#include <glwrap/VertexBuffer.h>
#include <glwrap/FileManager.h>
#include <glwrap/ObjPart.h>
#include <glwrap/TriFace.h>
#include <glwrap/ObjAnimation.h>
#include <glwrap/Context.h>

#include <fstream>
#include <iostream>

namespace glwrap
{
	void VertexArray::splitStringWhitespace(std::string& _input, std::vector<std::string>& _output)
	{
		std::string curr;

		_output.clear();

		for (size_t i = 0; i < _input.length(); i++)
		{
			if (_input.at(i) == ' ' ||
				_input.at(i) == '\r' ||
				_input.at(i) == '\n' ||
				_input.at(i) == '\t')
			{
				if (curr.length() > 0)
				{
					_output.push_back(curr);
					curr = "";
				}
			}
			else
			{
				curr += _input.at(i);
			}
		}

		if (curr.length() > 0)
		{
			_output.push_back(curr);
		}
	}

	void VertexArray::splitString(std::string& _input, char _splitter, std::vector<std::string>& _output)
	{
		std::string curr;

		_output.clear();

		for (size_t i = 0; i < _input.length(); i++)
		{
			if (_input.at(i) == _splitter)
			{
				_output.push_back(curr);
				curr = "";
			}
			else
			{
				curr += _input.at(i);
			}
		}

		if (curr.length() > 0)
		{
			_output.push_back(curr);
		}
	}

	void VertexArray::checkMinMax(glm::vec3& _vertexPosition)
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

	VertexArray::VertexArray()
	{
		m_dirty = false;
		m_minPoint = glm::vec3(std::numeric_limits<float>::max());
		m_maxPoint = glm::vec3(std::numeric_limits<float>::min());
		m_cullAnimated = false;
	}

	VertexArray::~VertexArray()
	{

	}

	void VertexArray::parse(std::string _path)
	{
		parse(_path, false);
	}

	void VertexArray::parse(std::string _path, bool _tanBitan)
	{
		std::ifstream file(FileManager::returnPath(_path).c_str());

		if (!file.is_open())
		{
			throw std::exception();
		}

		std::string line;
		std::vector<std::string> splitLine;
		std::vector<int> countTris;
		int fcounter = 0;
		std::vector<glm::vec3> positions;
		std::vector<glm::vec2> texCoords;
		std::vector<glm::vec3> normals;
		//std::vector<glm::vec2> lightMaps;

		ObjPart* currentPart = nullptr;

		VertexBuffer* positionBuffer = nullptr;
		VertexBuffer* texCoordBuffer = nullptr;
		VertexBuffer* normalBuffer = nullptr;
		VertexBuffer* tangentBuffer = nullptr;
		VertexBuffer* bitangentBuffer = nullptr;
		//std::shared_ptr<VertexBuffer> lightMapBuffer;

		TriFace f;
		TriFace fq;

		glm::vec3 vertexPosition, edge1, edge2, normal;
		glm::vec2 deltaUV1, deltaUV2;
		float factor;

		while (!file.eof())
		{
			std::getline(file, line);
			if (line.length() < 1) continue;
			splitStringWhitespace(line, splitLine);
			if (splitLine.size() < 1) continue;

			if (splitLine.at(0) == "o" || splitLine.at(0) == "g")
			{
				m_parts.resize(m_parts.size() + 1);
				m_context->createPart(m_parts.back(), m_self, splitLine.at(1));
				countTris.push_back(fcounter);
				currentPart = &m_parts.back();
				fcounter = 0;

				currentPart->generateArrays();
				positionBuffer = currentPart->setBuffer("in_Position", 0);
				if (texCoordBuffer)
				{
					
					texCoordBuffer = currentPart->setBuffer("in_TexCoord", 0);
				}
				if (normalBuffer)
				{
					
					normalBuffer = currentPart->setBuffer("in_Normal", 0);
					if (_tanBitan)
					{
						tangentBuffer = currentPart->setBuffer("in_Tangent", 0);
						bitangentBuffer = currentPart->setBuffer("in_Bitangent", 0);
					}
				}
				//if (lightMapBuffer) currentPart->setBuffer("in_LightMapCoord", lightMapBuffer);
			}
			else if (splitLine.at(0) == "v")
			{
				if (!positionBuffer)
				{
					positionBuffer = currentPart->setBuffer("in_Position", 0);
				}
				vertexPosition = glm::vec3(
					atof(splitLine.at(1).c_str()),
					atof(splitLine.at(2).c_str()),
					atof(splitLine.at(3).c_str()));
				positions.push_back(vertexPosition);

				checkMinMax(vertexPosition);
			}
			else if (splitLine.at(0) == "vt")
			{
				if (!texCoordBuffer)
				{
					texCoordBuffer = currentPart->setBuffer("in_TexCoord", 0);
				}

				texCoords.push_back(glm::vec2(
					atof(splitLine.at(1).c_str()),
					1.0f - atof(splitLine.at(2).c_str())));
			}
			else if (splitLine.at(0) == "vn")
			{
				if (!normalBuffer)
				{
					normalBuffer = currentPart->setBuffer("in_Normal", 0);
					if (_tanBitan)
					{
						tangentBuffer = currentPart->setBuffer("in_Tangent", 0);
						bitangentBuffer = currentPart->setBuffer("in_Bitangent", 0);
					}
				}

				normals.push_back(glm::vec3(
					atof(splitLine.at(1).c_str()),
					atof(splitLine.at(2).c_str()),
					atof(splitLine.at(3).c_str())));
			}
			else if (splitLine.at(0) == "f")
			{
				std::vector<std::string> subsplit;
				splitString(splitLine.at(1), '/', subsplit);

				if (subsplit.size() >= 1)
				{
					f.pa = positions.at(atoi(subsplit.at(0).c_str()) - 1);
					positionBuffer->add(f.pa);
				}
				if (subsplit.size() >= 2)
				{
					f.tca = texCoords.at(atoi(subsplit.at(1).c_str()) - 1);
					texCoordBuffer->add(f.tca);
				}
				if (subsplit.size() >= 3)
				{
					f.na = normals.at(atoi(subsplit.at(2).c_str()) - 1);
					normalBuffer->add(f.na);
				}
				//if (subsplit.size() >= 4)
				//{
				//	f.lmca = lightMaps.at(atoi(subsplit.at(3).c_str()) - 1);
				//	lightMapBuffer->add(f.lmca);
				//}
				splitString(splitLine.at(2), '/', subsplit);
				if (subsplit.size() >= 1)
				{
					f.pb = positions.at(atoi(subsplit.at(0).c_str()) - 1);
					positionBuffer->add(f.pb);
				}
				if (subsplit.size() >= 2)
				{
					f.tcb = texCoords.at(atoi(subsplit.at(1).c_str()) - 1);
					texCoordBuffer->add(f.tcb);
				}
				if (subsplit.size() >= 3)
				{
					f.nb = normals.at(atoi(subsplit.at(2).c_str()) - 1);
					normalBuffer->add(f.nb);
				}
				//if (subsplit.size() >= 4)
				//{
				//	f.lmcb = lightMaps.at(atoi(subsplit.at(3).c_str()) - 1);
				//	lightMapBuffer->add(f.lmcb);
				//}
				splitString(splitLine.at(3), '/', subsplit);
				if (subsplit.size() >= 1)
				{
					f.pc = positions.at(atoi(subsplit.at(0).c_str()) - 1);
					positionBuffer->add(f.pc);
				}
				if (subsplit.size() >= 2)
				{
					f.tcc = texCoords.at(atoi(subsplit.at(1).c_str()) - 1);
					texCoordBuffer->add(f.tcc);
				}
				if (subsplit.size() >= 3)
				{
					f.nc = normals.at(atoi(subsplit.at(2).c_str()) - 1);
					normalBuffer->add(f.nc);

					if (_tanBitan)
					{
						edge1 = f.pb - f.pa;
						edge2 = f.pc - f.pa;
						deltaUV1 = f.tcb - f.tca;
						deltaUV2 = f.tcc - f.tca;
						factor = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

						f.tan.x = factor * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
						f.tan.y = factor * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
						f.tan.z = factor * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
						f.tan = glm::normalize(f.tan);

						f.bitan.x = factor * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
						f.bitan.y = factor * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
						f.bitan.z = factor * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
						f.bitan = glm::normalize(f.bitan);

						normal = glm::cross(f.tan, f.bitan);
						factor = glm::dot(normal, f.na);
						if (factor < 0)
						{
							f.tan = -f.tan;
						}

						tangentBuffer->add(f.tan);
						bitangentBuffer->add(f.bitan);
					}
				}
				//if (subsplit.size() >= 4)
				//{
				//	f.lmcc = lightMaps.at(atoi(subsplit.at(3).c_str()) - 1);
				//	lightMapBuffer->add(f.lmcc);
				//}

				m_faces.push_back(f);
				fcounter++;
				if (!currentPart)
				{
					m_parts.resize(m_parts.size() + 1); 
					m_context->createPart(m_parts.back(), m_self, "Default");
					currentPart = &m_parts.back();
				}
				currentPart->addFace(&m_faces.back());

				if (splitLine.size() < 5) continue;

				fq.pa = f.pc;
				positionBuffer->add(fq.pa);
				fq.tca = f.tcc;
				texCoordBuffer->add(fq.tca);
				fq.na = f.nc;
				if (normalBuffer) normalBuffer->add(fq.na);
				//fq.lmca = f.lmcc;
				//if (lightMapBuffer) lightMapBuffer->add(fq.lmca);

				splitString(splitLine.at(4), '/', subsplit);
				if (subsplit.size() >= 1)
				{
					fq.pb = positions.at(atoi(subsplit.at(0).c_str()) - 1);
					positionBuffer->add(fq.pb);
				}
				if (subsplit.size() >= 2)
				{
					fq.tcb = texCoords.at(atoi(subsplit.at(1).c_str()) - 1);
					texCoordBuffer->add(fq.tcb);
				}
				if (subsplit.size() >= 3)
				{
					fq.nb = normals.at(atoi(subsplit.at(2).c_str()) - 1);
					normalBuffer->add(fq.nb);
				}
				//if (subsplit.size() >= 4)
				//{
				//	fq.lmcb = lightMaps.at(atoi(subsplit.at(3).c_str()) - 1);
				//	lightMapBuffer->add(fq.lmcb);
				//}

				splitString(splitLine.at(1), '/', subsplit);
				fq.pc = f.pa;
				positionBuffer->add(fq.pc);
				fq.tcc = f.tca;
				texCoordBuffer->add(fq.tcc);
				fq.nc = f.na;
				if (normalBuffer)
				{
					normalBuffer->add(fq.nc);

					if (_tanBitan)
					{
						edge1 = fq.pb - fq.pa;
						edge2 = fq.pc - fq.pa;
						deltaUV1 = fq.tcb - fq.tca;
						deltaUV2 = fq.tcc - fq.tca;
						factor = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

						fq.tan.x = factor * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
						fq.tan.y = factor * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
						fq.tan.z = factor * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
						fq.tan = glm::normalize(fq.tan);

						fq.bitan.x = factor * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
						fq.bitan.y = factor * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
						fq.bitan.z = factor * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
						fq.bitan = glm::normalize(fq.bitan);

						normal = glm::cross(fq.tan, fq.bitan);
						factor = glm::dot(normal, fq.na);
						if (factor < 0)
						{
							fq.tan = -fq.tan;
						}

						tangentBuffer->add(fq.tan);
						bitangentBuffer->add(fq.bitan);
					}
				}
				//fq.lmcc = f.lmca;
				//if (lightMapBuffer) lightMapBuffer->add(fq.lmcc);

				m_faces.push_back(fq);
				fcounter++;
			}
		}

		// Place references in individual parts
		countTris.push_back(fcounter);
		countTris.erase(countTris.begin());
		int total;
		fcounter = 0;
		if (countTris.size() != m_parts.size())
		{
			throw std::exception();
		}
		for (int itr = 0; itr < countTris.size(); itr++)
		{
			total = countTris.at(itr);
			currentPart = &m_parts.at(itr);
			for (int i = 0; fcounter < total; fcounter++)
			{
				currentPart->addFace(&m_faces.at(fcounter));
			}
		}

		m_size = m_maxPoint - m_minPoint;
	}

	VertexBuffer* VertexArray::setBuffer(std::string _attribute)
	{
		if (m_parts.size() == 0)
		{
			m_parts.resize(m_parts.size() + 1);
			m_context->createPart(m_parts.back(), m_self, "Default");
		}

		if (m_parts.at(0).getName() == "Default")
		{
			return m_parts.at(0).setBuffer(_attribute, 0);
		}
		return nullptr;
	}

	void VertexArray::draw()
	{
		for (std::vector<ObjPart>::iterator itr = m_parts.begin();
			itr != m_parts.end(); itr++)
		{
			itr->draw();
		}
	}

	void VertexArray::cullAndDraw()
	{
		for (std::vector<ObjPart>::iterator itr = m_parts.begin();
			itr != m_parts.end(); itr++)
		{
			itr->cullAndDraw(m_cullAnimated);
		}
	}

	void VertexArray::drawPart(std::string _partName)
	{
		for (std::vector<ObjPart>::iterator itr = m_parts.begin();
			itr != m_parts.end(); itr++)
		{
			if (itr->getName() == _partName)
			{
				itr->draw();
				return;
			}
		}
	}

	ObjAnimation* VertexArray::addAnimation(std::string _path)
	{
		m_animations.resize(m_animations.size() + 1);
		m_context->createAnimation(m_animations.back(), m_self, _path);
		return &m_animations.back();
	}

	int VertexArray::playAnimationOnce(std::string _name)
	{
		bool found = false;

		for (int index = 0; index < m_animations.size(); index++)
		{
			if (m_animations.at(index).getName() == _name)
			{
				m_animations.at(index).setRepeating(false);
				m_animations.at(index).setEnabled(true);
				return index;
			}
		}

		throw std::exception();
	}

	void VertexArray::playAnimationOnce(int _index)
	{
		if (_index < m_animations.size())
		{
			m_animations.at(_index).setRepeating(false);
			m_animations.at(_index).setEnabled(true);
		}
	}

	int VertexArray::enableAnimation(std::string _name)
	{
		bool found = false;

		for (int index = 0; index < m_animations.size(); index++)
		{
			if (m_animations.at(index).getName() == _name)
			{
				m_animations.at(index).setRepeating(true);
				m_animations.at(index).setEnabled(true);
				return index;
			}
		}

		throw std::exception();
	}

	void VertexArray::enableAnimation(int _index)
	{
		if (_index < m_animations.size())
		{
			m_animations.at(_index).setRepeating(true);
			m_animations.at(_index).setEnabled(true);
		}
	}

	int VertexArray::enableOnlyAnimation(std::string _name)
	{
		int id = -1;

		for (int index = 0; index < m_animations.size(); index++)
		{
			if (m_animations.at(index).getName() == _name)
			{
				m_animations.at(index).setEnabled(true);
				id = index;
			}
			else
			{
				m_animations.at(index).setEnabled(false);
			}
		}

		return id;
	}

	void VertexArray::enableOnlyAnimation(int _index)
	{
		for (int index = 0; index < m_animations.size(); index++)
		{
			if (index == _index)
			{
				m_animations.at(index).setEnabled(true);
			}
			else
			{
				m_animations.at(index).setEnabled(false);
			}
		}
	}

	int VertexArray::disableAnimation(std::string _name)
	{
		bool found = false;

		for (int index = 0; index < m_animations.size(); index++)
		{
			if (m_animations.at(index).getName() == _name)
			{
				m_animations.at(index).setEnabled(false);
				return index;
			}
		}

		throw std::exception();
	}

	void VertexArray::disableAnimation(int _index)
	{
		if (_index < m_animations.size())
		{
			m_animations.at(_index).setEnabled(false);
		}
	}

	void VertexArray::disableAllAnimations()
	{
		for (int index = 0; index < m_animations.size(); index++)
		{
			m_animations.at(index).setEnabled(false);
		}
	}

	const std::vector<ObjPart>& VertexArray::getParts()
	{
		return m_parts;
	}

	ObjPart* VertexArray::getPart(std::string _name)
	{
		for (int partIndex = 0; partIndex < m_parts.size(); partIndex++)
		{
			if (m_parts.at(partIndex).getName() == _name)
			{
				return &m_parts.at(partIndex);
			}
		}

		throw std::exception();
	}

	const std::vector<ObjAnimation>& VertexArray::getAnimations()
	{
		return m_animations;
	}
}
