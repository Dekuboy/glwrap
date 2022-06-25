#ifndef _GLWRAP_ARRAY_PIECE
#define _GLWRAP_ARRAY_PIECE
#include <glwrap/BufferArray.h>
#include <string>

namespace glwrap
{
	struct TriFace;

	class Context;

	class ArrayPiece
	{
	public:

	protected:
		std::string m_name; //!< Name of ObjPart
		std::vector<GLuint> m_idList; //!< List of vertex array GL Ids

		bool m_dirty; //!< If a buffer has been added, buffer information must be sent to the GPU
		std::vector<TriFace*> m_faces; //!< List of tris in part
		std::vector<std::vector<VertexBuffer>>
			m_buffers; //!< List of different buffers in part, separated by Material ([mat][])
		glm::mat4 m_animationUniform; //!< Transformation matrix based on animated movement

		Context* m_context; //!< Pointer to glwrap context

		glm::vec3 m_minPoint; //!< Minimum xyz values
		glm::vec3 m_maxPoint; //!< Maximum xyz values

	};
}

#endif