#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glwrap/TextureUniforms.h>
#include <fstream>

namespace glwrap
{
	class Context;
	class VertexArray;
	class ObjMtlModel;
	class Texture;
	class RenderTexture;
	class GBuffer;
	class DepthBuffer;
	class DepthCube;
	class ViewingFrustum;

	/**
	* \brief Type of buffer if sample is a GBuffer
	*/
	enum gType
	{
		pos, //!< Position Texture in GBuffer 
		norm, //!< Normal Texture in GBuffer
		albed //!<  Colour Albedo and Specular highlights texture in GBuffer
	};

	/**
	* \brief Texture sample to be used in ShaderProgram
	*/
	struct Sampler
	{
		GLint m_id; //!< Id of texture buffer in GL
		Texture* m_texture; //!< Pointer to Texture
		DepthCube* m_depthCube; //!< Pointer to DepthCube
		GBuffer* m_gBuffer; //!< Pointer to GBuffer
		gType m_gType; //!< Current samples type if points to GBuffer
	};

	/**
	* \brief Manages GL shader and its properties
	* -holds Shader Id
	* -allows culling if ViewingFrustum is set
	* -allows uniform name retrieval if TextureUniforms is set to a derived class
	*/
	class ShaderProgram
	{
	public:
		ShaderProgram();
		ShaderProgram(std::string _path);

		~ShaderProgram();

		/**
		* \brief Draw ShaderProgram with current set uniforms
		*/
		void draw();
		/**
		* \brief Draw ShaderProgram with current set uniforms
		* -draws to input frame buffer
		*/
		void draw(RenderTexture* _renderTexture);
		/**
		* \brief Draw ShaderProgram with current set uniforms
		* -draws input model to current set frame buffer and viewport
		*/
		void draw(Model3D* _vertexArray);
		/**
		* \brief Draw ShaderProgram with current set uniforms
		* -draws input model to input frame buffer
		*/
		void draw(RenderTexture* _renderTexture, Model3D* _vertexArray);

		/**
		* \brief Draw ShaderProgram with current set uniforms if model is in view
		* -draws input model to current set frame buffer and viewport
		* -culls based on input values applied to model
		*/
		void cullAndDraw(Model3D* _vertexArray, 
			glm::vec3 &_centre, glm::vec3 &_size, glm::mat3 &_rotation);
		/**
		* \brief Draw ShaderProgram with current set uniforms if model is in view
		* -draws input model to input frame buffer
		* -culls based on input values applied to model
		*/
		void cullAndDraw(RenderTexture* _renderTexture, 
			Model3D* _vertexArray, 
			glm::vec3 &_centre, glm::vec3 &_size, glm::mat3 &_rotation);

		/**
		* \brief Retrieve the ViewingFrustum used to cull objects in shader
		*/
		ViewingFrustum* getViewingFrustum();
		/**
		* \brief Set the ViewingFrustum to cull objects when prompted
		*/
		void setViewingFrustum(ViewingFrustum* _frustum);
		/**
		* \brief Check the current ViewingFrustum if an object is in view
		* -if currently drawing, will assume object is offset from current draw cycle
		*        if(m_isDrawing) { ... _centre + m_drawingCentre; ... } 
		*/
		bool checkViewingFrustum(const glm::vec3 &_centre, const glm::vec3 &_size, const glm::mat3 &_rotation);

		/**
		* \brief When drawing a model, check if the inanimate model is in View
		*/
		bool checkModelInView();
		
		/**
		* \brief Retrieves TextureUniforms used
		*/
		TextureUniforms* getUniforms();

		/**
		* \brief Updates the uniforms holder to a derived class
		* -allows objects to query for uniform names used in the shader e.g:
		*     Materials query for where to put certain Texture maps using PBRUniforms
		*/
		template <class T>
		T* setTextureUniformsType()
		{
			T* test = new T;
			TextureUniforms* suTest = test;
			suTest->m_uniformIds = m_uniforms->m_uniformIds;
			suTest->m_uniformNames = m_uniforms->m_uniformNames;
			delete m_uniforms;
			m_uniforms = test;
			return test;
		}

		/**
		* \brief Set uniform of name _uniform to _value
		*/
		void setUniform(std::string _uniform, int _value);
		/**
		* \brief Set uniform of name _uniform to _value
		*/
		void setUniform(std::string _uniform, glm::vec4 _value);
		/**
		* \brief Set uniform of name _uniform to _value
		*/
		void setUniform(std::string _uniform, glm::vec3 _value);
		/**
		* \brief Set uniform of name _uniform to _value
		*/
		void setUniform(std::string _uniform, glm::vec2 _value);
		/**
		* \brief Set uniform array of name _uniform to _value in vector
		*/
		void setUniform(std::string _uniform, std::vector<glm::vec4> &_vectors);
		/**
		* \brief Set uniform array of name _uniform to _value in vector
		*/
		void setUniform(std::string _uniform, std::vector<glm::vec3> &_vectors);
		/**
		* \brief Set uniform of name _uniform to _value
		*/
		void setUniform(std::string _uniform, float _value);
		/**
		* \brief Set uniform array of name _uniform to _value in vector
		*/
		void setUniform(std::string _uniform, std::vector<float> &_floats);
		/**
		* \brief Set uniform of name _uniform to texture GL Id when drawing
		*/
		void setUniform(std::string _uniform, Texture* _texture);
		/**
		* \brief Set uniform of name _uniform to _matrix value
		*/
		void setUniform(std::string _uniform, glm::mat4 &_matrix);
		/**
		* \brief Set uniform of name _uniform to _matrix value
		*/
		void setUniform(std::string _uniform, std::vector<glm::mat4> &_matrices);
		/**
		* \brief Set uniform of name _uniform to texture GL Id when drawing
		*/
		void setUniform(std::string _uniform, DepthBuffer* _depth);
		/**
		* \brief Set uniform of name _uniform to texture GL Id when drawing
		*/
		void setUniform(std::string _uniform, DepthCube* _depthCube);
		/**
		* \brief Set uniform array of name _uniform to texture GL Id in vector when drawing
		*/
		void setUniform(std::string _uniform, std::vector<DepthCube*> &_cubes);
		/**
		* \brief Set GBuffer texture GL Ids when drawing
		* -sets to: in_GPosition
		*           in_GNormal
		*           in_GAlbedoSpec
		*/
		void setUniform(GBuffer* _gBuffer);

		/**
		* \brief Retrieve the shader GL Id
		*/
		GLuint getId();

		/**
		* \brief Set the texture/window dimension values to draw to
		*/
		void setViewport(glm::vec4 _viewport);

	private:
		friend class Context;

		GLuint m_id; //!< Id of shader in GL
		TextureUniforms* m_uniforms = nullptr; //!< List of uniforms used in shader
		std::vector<Sampler> m_samplers; //!< List of texture samplers used when drawing
		bool m_isDrawing; //!< True if ShaderProgram is currently in its draw function
		bool m_modelInView; //!< When culling, confirms the base model is in view
		glm::vec3 m_drawingCentre; //!< The base object centre during cullAndDraw()
		glm::vec3 m_drawingSize; //!< The base object size during cullAndDraw()
		glm::mat3 m_drawingRotation; //!< The base object rotation during cullAndDraw()
		glm::vec4 m_viewport; //!< The dimensions of the texture/window being drawn to
		ViewingFrustum* m_viewingFrustum; //!< The frustum used to check if in view
		ShaderProgram* m_self; //!< Pointer to self
		Context* m_context; //!< Pointer to glwrap context

		/**
		* \brief Parse shader file and read into GL
		*/
		void parse(std::string _path);

		/**
		* \brief Checks current uniforms for uniform Id
		*/
		int checkUniforms(std::string& _uniform);

		/**
		* \brief Sets the texture uniforms in GL when drawing
		*/
		void setSamplers();
		/**
		* \brief Resets the texture uniforms in GL after drawing
		*/
		void resetSamplers();

	};
}