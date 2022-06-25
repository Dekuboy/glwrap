#include <glwrap/glwrap.h>

#include <glm/ext.hpp>

namespace glwrap
{
	void printShaderInfoLog(GLuint _obj)
	{
		int infologLength = 0;
		int charsWritten = 0;
		char* infoLog;

		glGetShaderiv(_obj, GL_INFO_LOG_LENGTH, &infologLength);

		if (infologLength > 0)
		{
			infoLog = (char*)malloc(infologLength);
			glGetShaderInfoLog(_obj, infologLength, &charsWritten, infoLog);
			printf("%s\n", infoLog);
			free(infoLog);
		}
	}

	void printProgramInfoLog(GLuint _obj)
	{
		int infologLength = 0;
		int charsWritten = 0;
		char* infoLog;

		glGetProgramiv(_obj, GL_INFO_LOG_LENGTH, &infologLength);

		if (infologLength > 0)
		{
			infoLog = (char*)malloc(infologLength);
			glGetProgramInfoLog(_obj, infologLength, &charsWritten, infoLog);
			printf("%s\n", infoLog);
			free(infoLog);
		}
	}

	ShaderProgram::ShaderProgram()
	{
		m_isDrawing = false;
		m_drawingCentre = glm::vec3(0);
		m_drawingSize = glm::vec3(0);
		m_drawingRotation = glm::mat4(1);
		m_uniforms = new TextureUniforms;
		m_id = -1;
	}

	ShaderProgram::ShaderProgram(std::string _path)
	{
		m_isDrawing = false;
		m_drawingCentre = glm::vec3(0);
		m_drawingSize = glm::vec3(0);
		m_drawingRotation = glm::mat4(1);
		m_uniforms = new TextureUniforms;
		m_id = -1;
		parse(_path);
	}

	ShaderProgram::~ShaderProgram()
	{

	}

	void ShaderProgram::parse(std::string _path)
	{
		std::string vertShader;
		std::string fragShader;
		std::string geomShader;

		std::string src = FileManager::loadWin(_path);

#if defined (__EMSCRIPTEN__)
		bool version = (src.compare(0, 8, "#version") == 0);
		if (version)
		{
			vertShader = src.substr(0, 15) + "\n#define VERTEX\n" + src.substr(15, std::string::npos);

			fragShader = src.substr(0, 15) + "\n#define FRAGMENT\n" + src.substr(15, std::string::npos);
		}
		else
		{
			vertShader = "\n#define VERTEX\n" + src;

			fragShader = "\n#define FRAGMENT\n" + src;
		}
#else
		bool version = (src.compare(0, 8, "#version") == 0);
		bool geometry = (src.find("GEOMETRY") != -1);
		if (version)
		{
			int def = src.find("#if");
			vertShader = src.substr(0, def) + "\n#define VERTEX\n" + src.substr(def, std::string::npos);

			fragShader = src.substr(0, def) + "\n#define FRAGMENT\n" + src.substr(def, std::string::npos);

			if (geometry)
			geomShader = src.substr(0, def) + "\n#define GEOMETRY\n" + src.substr(def, std::string::npos);
		}
		else
		{
			vertShader = "#version 140\n#define VERTEX\n" + src;

			fragShader = "#version 140\n#define FRAGMENT\n" + src;
		}
#endif

		const char* vertex = vertShader.c_str();
		const char* fragment = fragShader.c_str();

		GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShaderId, 1, &vertex, NULL);
		glCompileShader(vertexShaderId);
		GLint success = 0;
		glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			printShaderInfoLog(vertexShaderId);
			printProgramInfoLog(vertexShaderId);
			throw std::exception();
		}

		GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShaderId, 1, &fragment, NULL);
		glCompileShader(fragmentShaderId);
		success = 0;
		glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			printShaderInfoLog(fragmentShaderId);
			printProgramInfoLog(fragmentShaderId);
			throw std::exception();
		}

#if !defined(__EMSCRIPTEN__)
		GLuint geometryShaderId;
		if (geometry)
		{
			const char* geom = geomShader.c_str();
			geometryShaderId = glCreateShader(GL_GEOMETRY_SHADER);
			glShaderSource(geometryShaderId, 1, &geom, NULL);
			glCompileShader(geometryShaderId);
			success = 0;
			glGetShaderiv(geometryShaderId, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				throw std::exception();
			}
		}
#endif

		m_id = glCreateProgram();
		glAttachShader(m_id, vertexShaderId);
		glAttachShader(m_id, fragmentShaderId);
#if !defined(__EMSCRIPTEN__)
		if (geometry)
		{
			glAttachShader(m_id, geometryShaderId);
		}
#endif

		// Ensure the VAO "Position" attribute stream gets set as the first position
		// during the link.
		glBindAttribLocation(m_id, 0, "in_Position");
		glBindAttribLocation(m_id, 1, "in_Color");
		glBindAttribLocation(m_id, 2, "in_TexCoord");
		glBindAttribLocation(m_id, 3, "in_Normal");
		glBindAttribLocation(m_id, 4, "in_JointIDs");
		glBindAttribLocation(m_id, 5, "in_Weights");
		glBindAttribLocation(m_id, 6, "in_Tangent");
		glBindAttribLocation(m_id, 7, "in_Bitangent");

		// Perform the link and check for failure
		glLinkProgram(m_id);
		glGetProgramiv(m_id, GL_LINK_STATUS, &success);

		if (!success)
		{
			throw std::exception();
		}

		glDetachShader(m_id, vertexShaderId);
		glDeleteShader(vertexShaderId);
		glDetachShader(m_id, fragmentShaderId);
		glDeleteShader(fragmentShaderId);
#if !defined(__EMSCRIPTEN__)
		if (geometry)
		{
			glDetachShader(m_id, geometryShaderId);
			glDeleteShader(geometryShaderId);
		}
#endif
	}

	void ShaderProgram::draw()
	{
		draw(m_context->getSimpleShape());
	}

	void ShaderProgram::draw(RenderTexture* _renderTexture)
	{
		draw(_renderTexture, m_context->getSimpleShape());
	}

	void ShaderProgram::draw(Model3D* _vertexArray)
	{
		//  glUseProgram(id);
		//  glBindVertexArray(vertexArray->getId());
		//  
		//  glDrawArrays(GL_TRIANGLES, 0, 3);
		//  
		//  glBindVertexArray(0);
		//  glUseProgram(0);
		glViewport(m_viewport.x, m_viewport.y, m_viewport.z, m_viewport.w);
		m_context->setCurrentShader(m_self);

		setSamplers();

		m_isDrawing = true;
		_vertexArray->draw();
		m_isDrawing = false;

		//glBindVertexArray(_vertexArray->getId());
		//glDrawArrays(GL_TRIANGLES, 0, _vertexArray->getVertexCount());

		resetSamplers();

		glBindVertexArray(0);
	}

	void ShaderProgram::draw(RenderTexture* _renderTexture, Model3D* _vertexArray)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, _renderTexture->getFbId());
		glm::vec4 lastViewport = m_viewport;
		m_viewport = glm::vec4(0, 0, _renderTexture->getSize().x, _renderTexture->getSize().y);
		draw(_vertexArray);
		m_viewport = lastViewport;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void ShaderProgram::cullAndDraw(Model3D* _vertexArray,
		glm::vec3& _centre, glm::vec3& _size, glm::mat3& _rotation)
	{
		if (m_viewingFrustum)
		{
			m_drawingCentre = _centre;
			m_drawingSize = _size;
			m_drawingRotation = _rotation;

			m_modelInView = checkViewingFrustum
			(_centre + _vertexArray->getCentre(), _size * _vertexArray->getSize(), _rotation);
			if (m_modelInView || _vertexArray->getCullAnimation())
			{

				glViewport(m_viewport.x, m_viewport.y, m_viewport.z, m_viewport.w);
				m_context->setCurrentShader(m_self);

				setSamplers();

				m_isDrawing = true;
				_vertexArray->cullAndDraw();
				m_isDrawing = false;

				//glBindVertexArray(_vertexArray->getId());
				//glDrawArrays(GL_TRIANGLES, 0, _vertexArray->getVertexCount());

				resetSamplers();

				glBindVertexArray(0);
			}
		}
	}

	void ShaderProgram::cullAndDraw(RenderTexture* _renderTexture, Model3D* _vertexArray,
		glm::vec3& _centre, glm::vec3& _size, glm::mat3& _rotation)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, _renderTexture->getFbId());
		glm::vec4 lastViewport = m_viewport;
		m_viewport = glm::vec4(0, 0, _renderTexture->getSize().x, _renderTexture->getSize().y);
		cullAndDraw(_vertexArray, _centre, _size, _rotation);
		m_viewport = lastViewport;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	ViewingFrustum* ShaderProgram::getViewingFrustum()
	{
		return m_viewingFrustum;
	}

	void ShaderProgram::setViewingFrustum(ViewingFrustum* _frustum)
	{
		m_viewingFrustum = _frustum;
	}

	bool ShaderProgram::checkViewingFrustum(const glm::vec3& _centre, const glm::vec3& _size, const glm::mat3& _rotation)
	{
		if (m_isDrawing)
		{
			glm::vec3 partCentre = m_drawingCentre + _centre;
			glm::vec3 partSize = m_drawingSize * _size;
			glm::mat3 partRotation = m_drawingRotation * _rotation;
			if (partRotation == glm::mat3(1))
			{
				return m_viewingFrustum->checkCube(partCentre, partSize);
			}
			else
			{
				return m_viewingFrustum->checkCube(partCentre, partSize, partRotation);
			}
		}
		else
		{
			if (_rotation == glm::mat3(1))
			{
				return m_viewingFrustum->checkCube(_centre, _size);
			}
			else
			{
				return m_viewingFrustum->checkCube(_centre, _size, _rotation);
			}
		}
	}

	bool ShaderProgram::checkModelInView()
	{
		return m_modelInView;
	}

	TextureUniforms* ShaderProgram::getUniforms()
	{
		return m_uniforms;
	}

	void ShaderProgram::setUniform(std::string _uniform, int _value)
	{
		GLint uniformId = checkUniforms(_uniform);

		m_context->setCurrentShader(m_self);
		glUniform1i(uniformId, _value);
	}

	void ShaderProgram::setUniform(std::string _uniform, glm::vec4 _value)
	{
		GLint uniformId = checkUniforms(_uniform);

		m_context->setCurrentShader(m_self);
		glUniform4f(uniformId, _value.x, _value.y, _value.z, _value.w);
	}

	void ShaderProgram::setUniform(std::string _uniform, glm::vec3 _value)
	{
		GLint uniformId = checkUniforms(_uniform);

		m_context->setCurrentShader(m_self);
		glUniform3f(uniformId, _value.x, _value.y, _value.z);
	}

	void ShaderProgram::setUniform(std::string _uniform, glm::vec2 _value)
	{
		GLint uniformId = checkUniforms(_uniform);

		m_context->setCurrentShader(m_self);
		glUniform2f(uniformId, _value.x, _value.y);
	}

	void ShaderProgram::setUniform(std::string _uniform, std::vector<glm::vec4>& _vectors)
	{
		GLint uniformId = checkUniforms(_uniform);

		m_context->setCurrentShader(m_self);
		glUniform4fv(uniformId, _vectors.size(), glm::value_ptr(_vectors.at(0)));
	}

	void ShaderProgram::setUniform(std::string _uniform, std::vector<glm::vec3>& _vectors)
	{
		GLint uniformId = checkUniforms(_uniform);

		m_context->setCurrentShader(m_self);
		glUniform3fv(uniformId, _vectors.size(), glm::value_ptr(_vectors.at(0)));
	}

	void ShaderProgram::setUniform(std::string _uniform, float _value)
	{
		GLint uniformId = checkUniforms(_uniform);

		m_context->setCurrentShader(m_self);
		glUniform1f(uniformId, _value);
	}

	void ShaderProgram::setUniform(std::string _uniform, std::vector<float>& _floats)
	{
		GLint uniformId = checkUniforms(_uniform);

		m_context->setCurrentShader(m_self);
		glUniform1fv(uniformId, _floats.size(), reinterpret_cast<GLfloat*>(_floats.data()));
	}

	void ShaderProgram::setUniform(std::string _uniform, Texture* _texture)
	{
		GLint uniformId = checkUniforms(_uniform);

		m_context->setCurrentShader(m_self);

		for (int i = 0; i < m_samplers.size(); i++)
		{
			if (m_samplers.at(i).m_id == uniformId)
			{
				m_samplers.at(i).m_texture = _texture;
				m_samplers.at(i).m_depthCube = NULL;
				m_samplers.at(i).m_gBuffer = NULL;

				glUniform1i(uniformId, i);
				if (m_isDrawing)
				{
					glActiveTexture(GL_TEXTURE0 + i);

					glBindTexture(GL_TEXTURE_2D, m_samplers.at(i).m_texture->getId());
				}
				return;
			}
		}

		Sampler s;
		s.m_id = uniformId;
		s.m_texture = _texture;
		m_samplers.push_back(s);

		glUniform1i(uniformId, m_samplers.size() - 1);
		if (m_isDrawing)
		{
			glActiveTexture(GL_TEXTURE0 + m_samplers.size() - 1);

			glBindTexture(GL_TEXTURE_2D, s.m_texture->getId());
		}
	}

	void ShaderProgram::setUniform(std::string _uniform, glm::mat4& _value)
	{
		GLint uniformId = checkUniforms(_uniform);

		m_context->setCurrentShader(m_self);
		glUniformMatrix4fv(uniformId, 1, 0, glm::value_ptr(_value));
	}

	void ShaderProgram::setUniform(std::string _uniform, std::vector<glm::mat4>& _matrices)
	{
		GLint uniformId = checkUniforms(_uniform);

		m_context->setCurrentShader(m_self);
		glUniformMatrix4fv(uniformId, _matrices.size(), 0, glm::value_ptr(_matrices.at(0)));
	}

	void ShaderProgram::setUniform(std::string _uniform, DepthBuffer* _depth)
	{
		GLint uniformId = checkUniforms(_uniform);

		m_context->setCurrentShader(m_self);

		for (int i = 0; i < m_samplers.size(); i++)
		{
			if (m_samplers.at(i).m_id == uniformId)
			{
				m_samplers.at(i).m_texture = _depth;
				m_samplers.at(i).m_depthCube = NULL;
				m_samplers.at(i).m_gBuffer = NULL;

				glUniform1i(uniformId, i);
				return;
			}
		}

		Sampler s;
		s.m_id = uniformId;
		s.m_texture = _depth;
		m_samplers.push_back(s);

		glUniform1i(uniformId, m_samplers.size() - 1);
	}

	void ShaderProgram::setUniform(std::string _uniform, DepthCube* _depthCube)
	{
		GLint uniformId = checkUniforms(_uniform);

		m_context->setCurrentShader(m_self);

		for (int i = 0; i < m_samplers.size(); i++)
		{
			if (m_samplers.at(i).m_id == uniformId)
			{
				m_samplers.at(i).m_depthCube = _depthCube;
				m_samplers.at(i).m_texture = NULL;
				m_samplers.at(i).m_gBuffer = NULL;

				glUniform1i(uniformId, i);
				return;
			}
		}

		Sampler s;
		s.m_id = uniformId;
		s.m_depthCube = _depthCube;
		m_samplers.push_back(s);

		glUniform1i(uniformId, m_samplers.size() - 1);
	}

	void ShaderProgram::setUniform(std::string _uniform, std::vector<DepthCube*>& _cubes)
	{
		GLint uniformId = checkUniforms(_uniform);

		bool skip = false;
		int count = 0;
		std::string temp;
		m_context->setCurrentShader(m_self);

		for (std::vector<DepthCube*>::iterator itr = _cubes.begin();
			itr != _cubes.end(); itr++)
		{
			temp = _uniform + "[" + std::to_string(count) + "]";
			GLint uniformId = glGetUniformLocation(m_id, temp.c_str());

			for (int i = 0; i < m_samplers.size(); i++)
			{
				if (m_samplers.at(i).m_id == uniformId)
				{
					m_samplers.at(i).m_depthCube = (*itr);
					m_samplers.at(i).m_texture = NULL;
					m_samplers.at(i).m_gBuffer = NULL;

					glUniform1i(uniformId, i);
					skip = true;
					break;
				}
			}
			if (!skip)
			{
				Sampler s;
				s.m_id = uniformId;
				s.m_depthCube = (*itr);
				m_samplers.push_back(s);

				glUniform1i(uniformId, m_samplers.size() - 1);
			}
			count++;
		}
	}

	void ShaderProgram::setUniform(GBuffer* _gBuffer)
	{
		int buffer = 0;
		bool skip = false;
		GLint uniformId;
		std::string pos = "in_GPosition", norm = "in_GNormal", col = "in_GAlbedoSpec";
		m_context->setCurrentShader(m_self);
		while (buffer < 3)
		{
			if (buffer == 0) uniformId = checkUniforms(pos);
			else if (buffer == 1) uniformId = checkUniforms(norm);
			else if (buffer == 2) uniformId = checkUniforms(col);

			for (int i = 0; i < m_samplers.size(); i++)
			{
				if (m_samplers.at(i).m_id == uniformId)
				{
					m_samplers.at(i).m_gBuffer = _gBuffer;
					m_samplers.at(i).m_gType = (gType)buffer;
					m_samplers.at(i).m_texture = NULL;
					m_samplers.at(i).m_depthCube = NULL;

					glUniform1i(uniformId, i);
					skip = true;
					break;
				}
			}
			if (!skip)
			{
				Sampler s;
				s.m_id = uniformId;
				s.m_gBuffer = _gBuffer;
				s.m_gType = (gType)buffer;
				m_samplers.push_back(s);

				glUniform1i(uniformId, m_samplers.size() - 1);
			}
			buffer++;
		}
	}

	GLuint ShaderProgram::getId()
	{
		return m_id;
	}

	void ShaderProgram::setViewport(glm::vec4 _viewport)
	{
		this->m_viewport = _viewport;
	}

	int ShaderProgram::checkUniforms(std::string& _uniform)
	{
		int size = m_uniforms->m_uniformNames.size();
		for (int i = 0; i < size; i++)
		{
			if (m_uniforms->m_uniformNames.at(i) == _uniform)
			{
				return m_uniforms->m_uniformIds.at(i);
			}
		}

		GLint uniformId = glGetUniformLocation(m_id, _uniform.c_str());

		if (uniformId == -1)
		{
			throw std::exception();
		}

		m_uniforms->m_uniformNames.push_back(_uniform);
		m_uniforms->m_uniformIds.push_back(uniformId);

		return uniformId;
	}

	void ShaderProgram::setSamplers()
	{
		gType check;

		for (size_t i = 0; i < m_samplers.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);

			if (m_samplers.at(i).m_texture)
			{
				glBindTexture(GL_TEXTURE_2D, m_samplers.at(i).m_texture->getId());
			}
			else if (m_samplers.at(i).m_depthCube)
			{
				glBindTexture(GL_TEXTURE_CUBE_MAP, m_samplers.at(i).m_depthCube->getId());
			}
			else if (m_samplers.at(i).m_gBuffer)
			{
				check = m_samplers.at(i).m_gType;

				if (check == 0)
				{
					glBindTexture(GL_TEXTURE_2D, m_samplers.at(i).m_gBuffer->getId());
				}
				else if (check == 1)
				{
					glBindTexture(GL_TEXTURE_2D, m_samplers.at(i).m_gBuffer->getNId());
				}
				else if (check == 2)
				{
					glBindTexture(GL_TEXTURE_2D, m_samplers.at(i).m_gBuffer->getAsId());
				}
				else
				{
					glBindTexture(GL_TEXTURE_2D, 0);
				}
			}
			else
			{
				glBindTexture(GL_TEXTURE_2D, 0);
			}
		}
	}

	void ShaderProgram::resetSamplers()
	{
		for (size_t i = 0; i < m_samplers.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);

			if (m_samplers.at(i).m_texture || m_samplers.at(i).m_gBuffer)
			{
				glBindTexture(GL_TEXTURE_2D, 0);
			}
			else if (m_samplers.at(i).m_depthCube)
			{
				glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
			}
		}
	}
}