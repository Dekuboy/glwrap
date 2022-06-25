#include <glwrap/VertexBuffer.h>
#include <string>

namespace glwrap
{
	struct TriFace;

	class Context;
	class GltfModel;
	class Material;

	/**
	* \brief Stores information on individual joints of a model
	*/
	class ModelJoint
	{
	public:
		ModelJoint(std::string _name);
		~ModelJoint();

		/**
		* \brief Retrieve name of joint
		*/
		std::string getName();
		/**
		* \brief Retrieve list of tri faces
		*/
		const std::vector<TriFace*>& getFaces();

		/**
		* \brief Retrieve vertex total which use the same Material within part
		*/
		int getVertexCount(int _materialId);

		/**
		* \brief Retrieve GL Id of the vertex array which uses input Material
		*/
		GLuint getId(int _materialId);

		/**
		* \brief Retrieve the size of ModelJoint based on xyz values
		*/
		glm::vec3 getSize();
		/**
		* \brief Retrieve the centre of ModelJoint
		*/
		glm::vec3 getCentre();

	private:
		friend class Context;
		friend class GltfModel;

		std::string m_name; //!< Name of ObjPart
		std::vector<GLuint> m_idList; //!< List of vertex array GL Ids

		bool m_dirty; //!< If a buffer has been added, buffer information must be sent to the GPU
		std::vector<TriFace*> m_faces; //!< List of tris in part
		std::vector<std::vector<VertexBuffer> > 
			m_buffers; //!< List of different buffers in mesh, separated by Primitives ([prims][])
		glm::mat4* m_animationUniform; //!< Transformation matrix based on animated movement

		std::vector<Material*> m_materials; //!< List of Material used in part

		Context* m_context; //!< Pointer to glwrap context

		glm::vec3 m_minPoint; //!< Minimum xyz values
		glm::vec3 m_maxPoint; //!< Maximum xyz values
		glm::vec3 m_size; //!< Size of mesh

		/**
		* \brief Add tri face to list
		*/
		void addFace(TriFace* _face);

		/**
		* \brief Checks if vertex values are the minimum or maximum of the model
		*/
		void checkMinMax(glm::vec3& _vertexPosition);
		/**
		* \brief Checks if vertex values are the minimum of the model
		*/
		void checkMin(glm::vec3& _vertexPosition);
		/**
		* \brief Checks if vertex values are the maximum of the model
		*/
		void checkMax(glm::vec3& _vertexPosition);

		/**
		* \brief Draws vertex arrays
		*/
		void drawArrays();

		/**
		* \brief Generates a vertex array Id in GL
		*/
		void generateArrays();
		/**
		* \brief Retrieve buffer of name _attribute and retrieve to set
		*/
		VertexBuffer* setBuffer(std::string _attribute, int _materialId);

		/**
		* \brief Draw all vertex arrays represented within this joint
		*/
		void draw();
		/**
		* \brief Draw all vertex arrays represented within this mesh that appear in view
		* -useful for large models where meshes are too distant to always be in view
		*/
		void cullAndDraw();

	};
}