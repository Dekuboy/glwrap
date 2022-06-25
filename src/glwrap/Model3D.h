#ifndef _GLWRAP_MODEL3D
#define _GLWRAP_MODEL3D
#include <vector>
#include <glm/glm.hpp>
#include <glwrap/TriFace.h>

namespace glwrap
{
	struct TriFace;
	class Context;

	class Model3D
	{
	public:
		virtual ~Model3D() { }

		/**
		* \brief Retrieve the size of model based on xyz values
		*/
		glm::vec3 getSize() { return m_size; }
		/**
		* \brief Retrieve the centre of model
		*/
		glm::vec3 getCentre() { return m_minPoint + 0.5f * m_size; }
		/**
		* \brief Retrieve all the tri information of the model
		*/
		virtual std::vector<TriFace>& getFaces() { return m_faces; }

		/**
		* \brief Set cull animation
		* -if(true) when ShaderProgram calls cullAndDraw() then
		*    ObjPart will assume non-moving parts are within the view if the
		*    model is in view
		* -useful for smaller objects with moving parts with a larger reach
		*/
		void setCullAnimation(bool _switch) { m_cullAnimated = _switch; }
		/**
		* \brief Get cull animation
		*/
		bool getCullAnimation() { return m_cullAnimated; }

	protected:
		friend class Context;
		friend class ShaderProgram;

		bool m_dirty; //!< If the buffers have been altered, update in GL
		std::vector<TriFace> m_faces; //!< Retains information on all tris in the model
		bool m_cullAnimated; //!< Set to cull parts individually in case parts exceed model boundaries during animation

		glm::vec3 m_minPoint; //!< Minimum xyz values
		glm::vec3 m_maxPoint; //!< Maximum xyz values
		glm::vec3 m_size; //!< total size of the model

		Context* m_context; //!< Pointer to glwrap context

		virtual void draw() {}
		virtual void cullAndDraw() {}
	};
}
#endif