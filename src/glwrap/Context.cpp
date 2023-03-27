#include <glwrap/glwrap.h>

namespace glwrap
{
	void Context::initialise(Context &_toInit)
	{
		if (glewInit() != GLEW_OK)
		{
			throw std::exception();
		}

		VertexBuffer* positions;

		VertexBuffer* texCoords;

		_toInit.m_self = &_toInit;

		_toInit.m_simpleShape = new VertexArray();
		_toInit.createMesh(*_toInit.m_simpleShape);
		positions = _toInit.m_simpleShape->setBuffer("in_Position");
		texCoords = _toInit.m_simpleShape->setBuffer("in_TexCoord");

		positions->add(glm::vec2(-1.0f, 1.0f));
		positions->add(glm::vec2(-1.0f, -1.0f));
		positions->add(glm::vec2(1.0f, -1.0f));
		positions->add(glm::vec2(1.0f, -1.0f));
		positions->add(glm::vec2(1.0f, 1.0f));
		positions->add(glm::vec2(-1.0f, 1.0f));

		texCoords->add(glm::vec2(0.0f, 0.0f));
		texCoords->add(glm::vec2(0.0f, -1.0f));
		texCoords->add(glm::vec2(1.0f, -1.0f));
		texCoords->add(glm::vec2(1.0f, -1.0f));
		texCoords->add(glm::vec2(1.0f, 0.0f));
		texCoords->add(glm::vec2(0.0f, 0.0f));

		m_live = true;
	}

	bool Context::m_live = false;

	Context::~Context()
	{
		if (m_simpleShape)
		{
			delete m_simpleShape;
		}
	}

	void Context::createTexture(Texture& _toInit, std::string _path)
	{
		_toInit = Texture(_path);
		_toInit.m_context = m_self;
	}

	void Context::createShader(ShaderProgram &_toInit, std::string _path)
	{
		if (_toInit.m_id < 0 || _toInit.m_uniforms)
		{
			throw std::exception();
		}
		_toInit = ShaderProgram();
		_toInit.m_context = m_self;
		_toInit.m_self = &_toInit;
		_toInit.parse(_path);
	}

	void Context::createBuffer(VertexBuffer& _toInit)
	{
		if (_toInit.m_id < 0)
		{
			throw std::exception();
		}
		rtn.m_context = m_self;
		return rtn;
	}

	void Context::createMesh(VertexArray& _toInit)
	{
		std::shared_ptr<VertexArray> rtn = std::make_shared<VertexArray>();
		rtn->m_context = m_self;
		rtn->m_self = rtn;
		return rtn;
	}

	void Context::createMesh(VertexArray& _toInit, std::string _path)
	{
		std::shared_ptr<VertexArray> rtn = std::make_shared<VertexArray>();
		rtn->m_context = m_self;
		rtn->m_self = rtn;
		rtn->parse(_path);
		return rtn;
	}

	void Context::createMesh(VertexArray& _toInit, std::string _path, bool _calcTanBitan)
	{
		VertexArray rtn = std::make_shared<VertexArray>();
		rtn->m_context = m_self;
		rtn->m_self = rtn;
		rtn->parse(_path, _calcTanBitan);
		return rtn;
	}

	void Context::createObjMtlMesh(ObjMtlModel& _toInit, std::string _path)
	{
		std::shared_ptr<ObjMtlModel> rtn = std::make_shared<ObjMtlModel>();
		rtn->m_context = m_self;
		rtn->m_self = rtn;
		rtn->parse(_path);
		return rtn;
	}

	void Context::createObjMtlMesh(ObjMtlModel& _toInit, std::string _path, bool _calcTanBitan)
	{
		std::shared_ptr<ObjMtlModel> rtn = std::make_shared<ObjMtlModel>();
		rtn->m_context = m_self;
		rtn->m_self = rtn;
		rtn->parse(_path, _calcTanBitan);
		return rtn;
	}

	void Context::createPart(ObjPart** _toInit, VertexArray* _mesh, std::string _name)
	{
		*_toInit = new ObjPart(_name);
		(* _toInit)->m_context = m_self;
		(*_toInit)->m_self = (*_toInit);
	}

	void Context::createModel(GltfModel& _toInit, std::string _path)
	{
		GltfModel rtn;
		rtn.m_context = m_self;
		rtn.parse(_path);
		return rtn;
	}

	void Context::createModel(GltfModel& _toInit, std::string _path, bool _calcTanBitan)
	{
		GltfModel rtn;
		rtn.m_context = m_self;
		rtn.parse(_path, _calcTanBitan);
		return rtn;
	}

	void Context::createModelJoint(ModelJoint& _toInit, std::string _name)
	{
		_toInit = ModelJoint(_name);
		_toInit.m_context = m_self;
	}

	void Context::createAnimation(ObjAnimation& _toInit, VertexArray* _model)
	{
		ObjAnimation rtn(_model);
		rtn.m_context = m_self;
		rtn.parse("");
		return rtn;
	}

	void Context::createAnimation(ObjAnimation& _toInit, VertexArray* _model, std::string _path)
	{
		ObjAnimation rtn(_model);
		rtn.m_context = m_self;
		rtn.parse(_path);
		return rtn;
	}

	void Context::createRenderTexture(RenderTexture& _toInit, int _width, int _height)
	{
		RenderTexture rtn(_width, _height);
		rtn.m_context = m_self;
		return rtn;
	}

	void Context::createRenderTexture(RenderTexture& _toInit, int _width, int _height, int _multisamples)
	{
		RenderTexture rtn(_width, _height, _multisamples);
		rtn.m_context = m_self;
		return rtn;
	}

	void Context::createDepthBuffer(DepthBuffer& _toInit, int _width, int _height)
	{
		DepthBuffer rtn(_width, _height);
		rtn.m_context = m_self;
		return rtn;
	}

	void Context::createDepthCube(DepthCube& _toInit, int _width, int _height)
	{
		DepthCube rtn(_width, _height);
		rtn.m_context = m_self;
		return rtn;
	}

	void Context::createGBuffer(GBuffer& _toInit, int _width, int _height)
	{
		GBuffer rtn(_width, _height);
		rtn.m_context = m_self;
		return rtn;
	}

	ShaderProgram* Context::getCurrentShader()
	{
		return m_currentShader;
	}

	void Context::setCurrentShader(ShaderProgram *_shader)
	{
		if (m_currentShader != _shader)
		{
			glUseProgram(_shader->m_id);
			m_currentShader = _shader;
		}
	}

	VertexArray* Context::getSimpleShape()
	{
		return m_simpleShape;
	}
}