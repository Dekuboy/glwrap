#include <glwrap/ObjMtlModel.h>
#include <glwrap/VertexBuffer.h>
#include <glwrap/FileManager.h>
#include <glwrap/ObjPart.h>
#include <glwrap/TriFace.h>
#include <glwrap/ObjAnimation.h>
#include <glwrap/Context.h>
#include <glwrap/Material.h>

#include <fstream>
#include <iostream>

namespace glwrap
{
	ObjMtlModel::ObjMtlModel()
	{

	}

	ObjMtlModel::~ObjMtlModel()
	{

	}

	void ObjMtlModel::parse(std::string _path)
	{
		parse(_path, false);
	}

	void ObjMtlModel::parse(std::string _path, bool _tanBitan)
	{
		std::string mtlPath = _path.substr(0, _path.length() - 3) + "mtl";

		parseMtl(mtlPath);

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

		Material* currentMaterial = nullptr;
		int materialCount = 0;

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
				m_parts.push_back(m_context->createPart(m_self, splitLine.at(1)));
				countTris.push_back(fcounter);
				currentPart = &m_parts.back();
				currentPart->m_useMaterial = true;
				currentMaterial = 0;
				materialCount = 0;
				fcounter = 0;

				currentPart->generateArrays();
				currentPart->setBuffer("in_Position", VertexBuffer(), materialCount);
				positionBuffer = &currentPart->m_buffers.at(materialCount).at(0);
				if (texCoordBuffer)
				{
					currentPart->setBuffer("in_TexCoord", VertexBuffer(), materialCount);
					texCoordBuffer = &currentPart->m_buffers.at(materialCount).at(2);
				}
				if (normalBuffer)
				{
					currentPart->setBuffer("in_Normal", VertexBuffer(), materialCount);
					normalBuffer = &currentPart->m_buffers.at(materialCount).at(3);
					if (_tanBitan)
					{
						currentPart->setBuffer("in_Tangent", VertexBuffer(), materialCount);
						tangentBuffer = &currentPart->m_buffers.at(materialCount).at(6);
						currentPart->setBuffer("in_Bitangent", VertexBuffer(), materialCount);
						bitangentBuffer = &currentPart->m_buffers.at(materialCount).at(7);
					}
				}
				//if (lightMapBuffer) currentPart->setBuffer("in_LightMapCoord", lightMapBuffer);
			}
			else if (splitLine.at(0) == "usemtl")
			{
				currentMaterial = getMaterial(splitLine.at(1));
				if (!currentMaterial)
				{
					throw std::exception();
				}
				currentPart->m_materials.push_back(currentMaterial);

				currentPart->generateArrays();
				currentPart->setBuffer("in_Position", VertexBuffer(), materialCount);
				positionBuffer = &currentPart->m_buffers.at(materialCount).at(0);
				if (texCoordBuffer)
				{
					currentPart->setBuffer("in_TexCoord", VertexBuffer(), materialCount);
					texCoordBuffer = &currentPart->m_buffers.at(materialCount).at(2);
				}
				if (normalBuffer)
				{
					currentPart->setBuffer("in_Normal", VertexBuffer(), materialCount);
					normalBuffer = &currentPart->m_buffers.at(materialCount).at(3);
					if (_tanBitan)
					{
						currentPart->setBuffer("in_Tangent", VertexBuffer(), materialCount);
						tangentBuffer = &currentPart->m_buffers.at(materialCount).at(6);
						currentPart->setBuffer("in_Bitangent", VertexBuffer(), materialCount);
						bitangentBuffer = &currentPart->m_buffers.at(materialCount).at(7);
					}
				}
				materialCount++;
			}
			else if (splitLine.at(0) == "v")
			{
				if (!positionBuffer)
				{
					currentPart->setBuffer("in_Position", VertexBuffer(), materialCount);
					positionBuffer = &currentPart->m_buffers.at(materialCount).at(0);
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
					currentPart->setBuffer("in_TexCoord", VertexBuffer(), materialCount);
					texCoordBuffer = &currentPart->m_buffers.at(materialCount).at(2);
				}
				texCoords.push_back(glm::vec2(
					atof(splitLine.at(1).c_str()),
					1.0f - atof(splitLine.at(2).c_str())));
			}
			else if (splitLine.at(0) == "vn")
			{
				if (!normalBuffer)
				{
					currentPart->setBuffer("in_Normal", VertexBuffer(), materialCount);
					normalBuffer = &currentPart->m_buffers.at(materialCount).at(3);
					if (_tanBitan)
					{
						currentPart->setBuffer("in_Tangent", VertexBuffer(), materialCount);
						tangentBuffer = &currentPart->m_buffers.at(materialCount).at(6);
						currentPart->setBuffer("in_Bitangent", VertexBuffer(), materialCount);
						bitangentBuffer = &currentPart->m_buffers.at(materialCount).at(7);
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

						f.bitan.x = factor * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
						f.bitan.y = factor * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
						f.bitan.z = factor * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);

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
					m_parts.push_back(m_context->createPart(m_self, "Default"));
					currentPart = &m_parts.back();
				}

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

						fq.bitan.x = factor * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
						fq.bitan.y = factor * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
						fq.bitan.z = factor * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);

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

	void ObjMtlModel::parseMtl(std::string _path)
	{
		std::ifstream file(FileManager::returnPath(_path).c_str());

		if (!file.is_open())
		{
			throw std::exception();
		}

		std::string line;
		std::vector<std::string> splitLine;

		std::string filePath = "";
		int pos = 0;
		pos = _path.find_last_of('\\');
		if (!(pos == std::string::npos))
		{
			filePath = _path.substr(0, pos + 1);
		}

		Material* currentMaterial = nullptr;

		while (!file.eof())
		{
			std::getline(file, line);
			if (line.length() < 1) continue;
			splitStringWhitespace(line, splitLine);
			if (splitLine.size() < 1) continue;
			int pos = 0;

			if (splitLine.at(0) == "newmtl")
			{
				m_materialList.push_back(Material());
				currentMaterial = &m_materialList.back();
				currentMaterial->m_name = splitLine.at(1);
			}
			else if (splitLine.at(0) == "map_Kd")
			{
				currentMaterial->m_colourPath = filePath + line.substr(7, line.length());
				pos = currentMaterial->m_colourPath.find("\\\\", 0);
				while (pos != std::string::npos)
				{
					currentMaterial->m_colourPath.replace(pos, 2, "\\");
					pos++;
					pos = currentMaterial->m_colourPath.find("\\\\", pos);
				}
			}
		}
	}

	Material* ObjMtlModel::getMaterial(std::string _material)
	{
		for (std::vector<Material>::iterator itr = m_materialList.begin();
			itr != m_materialList.end(); itr++)
		{
			if (itr->m_name == _material)
			{
				return &(*itr);
			}
		}
		return 0;
	}

	std::vector<Material>& ObjMtlModel::getMatList()
	{
		return m_materialList;
	}
}