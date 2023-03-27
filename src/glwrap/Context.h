#include <string>

namespace glwrap
{
	class VertexBuffer;
	class VertexArray;
	class ObjPart;
	class ObjAnimation;
	class Texture;
	class RenderTexture;
	class GBuffer;
	class DepthBuffer;
	class DepthCube;
	class ShaderProgram;
	class ObjMtlModel;
	class GltfModel;
	class ModelJoint;

	/**
	* \brief The OpenGL Context
	* -loads:   Obj models
	*             + mtl files optionally
	*           Obj Animation Files
	*           Textures
	*           Shaders
	* -creates: RenderTextures (fbo objects)
	*           DepthBuffer
	*/
	class Context
	{
	public:
		/**
		* \brief Initialise the OpenGL Context
		* -calls glewInit()
		*/
		static void initialise(Context &_toInit);

		/**
		* \brief Destructor clean up memory
		*/
		~Context();

		/**
		* \brief Create Buffer within glwrap Context
		*/
		void createBuffer(VertexBuffer& _toInit);
		/**
		* \brief Create Mesh within glwrap Context
		*/
		void createMesh(VertexArray& _toInit);
		/**
		* \brief Create Mesh within glwrap Context
		* -parses as .obj
		*/
		void createMesh(VertexArray& _toInit, std::string _path);
		/**
		* \brief Create Mesh within glwrap Context
		* -parses as .obj
		* -precalculate tangentBitangentNormal matrix if _calcTanBitan
		*/
		void createMesh(VertexArray& _toInit, std::string _path, bool _calcTanBitan);
		/**
		* \brief Create WavefrontModel within glwrap Context
		*/
		void createObjMtlMesh(ObjMtlModel& _toInit, std::string _path);
		/**
		* \brief Create WavefrontModel within glwrap Context
		* -precalculate tangentBitangentNormal matrix if _calcTanBitan
		*/
		void createObjMtlMesh(ObjMtlModel& _toInit, std::string _path, bool _calcTanBitan);
		/**
		* \brief Create ObjPart within glwrap Context
		*/
		void createPart(ObjPart** _toInit, VertexArray* _mesh, std::string _name);
		/**
		* \brief Create Model within glwrap Context using .gltf
		*/
		void createModel(GltfModel& _toInit, std::string _path);
		/**
		* \brief Create Model within glwrap Context using .gltf
		* -precalculate tangentBitangentNormal matrix if _calcTanBitan
		*/
		void createModel(GltfModel& _toInit, std::string _path, bool _calcTanBitan);
		/**
		* \brief Create ModelJoint within glwrap Context
		*/
		void createModelJoint(ModelJoint& _toInit, std::string _name);
		/**
		* \brief Create ObjAnimation within glwrap Context
		*/
		void createAnimation(ObjAnimation& _toInit, VertexArray* _model);
		/**
		* \brief Create ObjAnimation within glwrap Context
		*/
		void createAnimation(ObjAnimation& _toInit, VertexArray* _model, std::string _path);
		/**
		* \brief Create Texture within glwrap Context
		*/
		void createTexture(Texture& _toInit, std::string _path);
		/**
		* \brief Create RenderTexture within glwrap Context
		*/
		void createRenderTexture(RenderTexture& _toInit, int _width, int _height);
		/**
		* \brief Create Multisampling RenderTexture within glwrap Context
		* -_multiSamples = input numder of samples to generate
		*/
		void createRenderTexture(RenderTexture& _toInit, int _width, int _height, int _multisamples);
		/**
		* \brief Create DepthBuffer within glwrap Context
		*/
		void createDepthBuffer(DepthBuffer& _toInit, int _width, int _height);
		/**
		* \brief Create DepthCube within glwrap Context
		*/
		void createDepthCube(DepthCube& _toInit, int _width, int _height);
		/**
		* \brief Create GBuffer within glwrap Context
		*/
		void createGBuffer(GBuffer& _toInit, int _width, int _height);
		/**
		* \brief Create Shader within glwrap Context
		*/
		void createShader(ShaderProgram& _toInit, std::string _path);

		/**
		* \brief Gets ShaderProgram currently being used by Context
		*/
		ShaderProgram* getCurrentShader();
		/**
		* \brief Sets new ShaderProgram to be used by Context
		*/
		void setCurrentShader(ShaderProgram *_shader);

		/**
		* \brief Gets the quad used for drawing to screen
		*/
		VertexArray* getSimpleShape();

	private:
		ShaderProgram* m_currentShader = nullptr; //!< Stores ShaderProgram currently in use
		VertexArray* m_simpleShape = nullptr; //!< Simple quad for drawing to screens entirety

		Context* m_self; //!< Pointer to self to be passed into created object

		static bool m_live;

		Context();

	};
}