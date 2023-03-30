#ifndef _GLWRAP_ARRAY_PIECE
#define _GLWRAP_ARRAY_PIECE
#include <glwrap/BufferArray.h>
#include <string>

namespace glwrap
{
	struct TriFace;

	class Context;
	class Material;

	class ArrayPiece
	{
	public:

		/**
		* \brief Retrieve name of part
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
		* \brief Retrieve the size of ObjPart based on xyz values
		*/
		glm::vec3 getSize();
		/**
		* \brief Retrieve the centre of ModelJoint
		*/
		glm::vec3 getCentre();

	protected:
		std::string m_name; //!< Name of ArrayPiece
		std::vector<GLuint> m_idList; //!< List of vertex array GL Ids

		bool m_dirty; //!< If a buffer has been added, buffer information must be sent to the GPU
		std::vector<TriFace*> m_faces; //!< List of tris in part
		std::vector<std::vector<VertexBuffer>>
			m_buffers; //!< List of different buffers in part, separated by Material ([mat][])
		glm::mat4 m_animationUniform; //!< Transformation matrix based on animated movement

		std::vector<Material*> m_materials; //!< List of Material used in part

		Context* m_context; //!< Pointer to glwrap context

		glm::vec3 m_minPoint; //!< Minimum xyz values
		glm::vec3 m_maxPoint; //!< Maximum xyz values
		glm::vec3 m_offset; //!< Centre xyz values

		/**
		* \brief Protected ctor only called by child class
		*/
		ArrayPiece();
		/**
		* \brief Virtual dtor can be called through pointer
		*/
		virtual ~ArrayPiece();


		/**
		* \brief Add tri face to list
		*/
		virtual void addFace(TriFace* _face);
		/**
		* \brief Draws vertex arrays
		*/
		virtual void drawArrays();

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
		virtual void draw();
		/**
		* \brief Draw all vertex arrays represented within this mesh that appear in view
		* -useful for large models where meshes are too distant to always be in view
		*/
		virtual void cullAndDraw();

	};
}

#endif