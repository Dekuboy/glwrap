#include <glwrap/ObjAnimation.h>
#include <glwrap/ObjFrame.h>
#include <glwrap/VertexArray.h>
#include <glwrap/ObjPart.h>
#include <glwrap/FileManager.h>
#include <fstream>

namespace glwrap
{
	ObjAnimation::ObjAnimation()
	{
		m_model = nullptr;
	}

	ObjAnimation::ObjAnimation(VertexArray* _model)
	{
		m_model = _model;
	}

	ObjAnimation::~ObjAnimation()
	{

	}

	void ObjAnimation::parse(std::string _path)
	{
		if (_path == "")
		{
			m_frames.push_back(ObjFrame());
			m_name = "Default";
			m_time = 0;
			m_enabled = false;
			m_repeating = true;
		}
		else
		{
			std::ifstream file(FileManager::returnPath(_path).c_str());
			std::string line;
			std::vector<std::string> parameters;
			std::vector<ObjPart> parts = m_model->getParts();
			ObjPart* poi;
			glm::vec3 v, rotation;
			bool found = false;

			m_name = "";
			m_time = 0;
			m_enabled = false;
			m_repeating = true;
			
			if (file.is_open() == false)
			{
				throw std::exception();
			}

			while (file.eof() == false)
			{
				getline(file, line);
				splitString(line, ' ', &parameters);

				if (parameters.size() > 0)
				{
					if (parameters.at(0) == "n")
					{
						m_name = parameters.at(1);
					}
					else if (parameters.at(0) == "f")
					{
						m_frames.push_back(ObjFrame());
					}
					else if (parameters.at(0) == "t")
					{
						found = false;
						//float scale = ;
						v.x = (atof(parameters.at(2).c_str()) / 100.0f);
						v.y = (atof(parameters.at(3).c_str()) / 100.0f);
						v.z = (atof(parameters.at(4).c_str()) / 100.0f);

						rotation.x = glm::radians(atof(parameters.at(5).c_str()));
						rotation.y = glm::radians(atof(parameters.at(6).c_str()));
						rotation.z = glm::radians(atof(parameters.at(7).c_str()));

						for (int partIndex = 0; partIndex < parts.size(); partIndex++)
						{
							poi = &parts.at(partIndex);
							if (poi->getName() == parameters.at(1))
							{
								v.x *= poi->getSize().x;
								v.y *= poi->getSize().y;
								v.z *= poi->getSize().z;
								m_frames.back().m_translations.push_back(
									ObjTranslation(poi,
										v,
										rotation));
								found = true;
								break;
							}
						}

						if (found == false)
						{
							throw std::exception();
						}
					}
				}
			}
		}
		m_totalTime = m_frames.size();
	}

	ObjFrame* ObjAnimation::getFrame()
	{
		if (m_time < m_frames.size())
		{
			return &m_frames.at(m_time);
		}
		else
		{
			return &m_frames.back();
		}
	}

	ObjFrame* ObjAnimation::getMergeFrame()
	{
		double nearestFrame = (int)m_time;
		double weight = m_time - nearestFrame;

		if (m_time + 1 >= m_frames.size())
		{
			generateMergeFrame(&m_frames.at(m_frames.size() - 1), &m_frames.at(0),
				weight);
		}
		else
		{
			generateMergeFrame(&m_frames.at(nearestFrame), &m_frames.at(nearestFrame + 1),
				weight);
		}

		return &m_mergeFrame;
	}
	
	int ObjAnimation::getMaxFrames()
	{
		return m_frames.size();
	}

	void ObjAnimation::nextFrame(float _deltaTime)
	{
		setCurrentFrame(m_time + _deltaTime);
	}

	void ObjAnimation::splitString(std::string input, char splitter, std::vector<std::string>* output)
	{
		std::string current;

		output->clear();

		for (int charIndex = 0; charIndex < input.length(); charIndex++)
		{
			if (input[charIndex] != splitter && input[charIndex] != 13)
			{
				current += input[charIndex];
			}
			else
			{
				output->push_back(current);
				current = "";
			}
		}

		if (current.length() > 0)
		{
			output->push_back(current);
		}
	}

	void ObjAnimation::generateMergeFrame(ObjFrame* a,
		ObjFrame* b, double weight)
	{
		ObjFrame::copy(a, &m_mergeFrame);
		ObjFrame::merge(b, &m_mergeFrame, weight);
	}
}