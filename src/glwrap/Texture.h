#ifndef _GLWRAP_TEXTURE
#define _GLWRAP_TEXTURE

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>

namespace glwrap
{
	class Context;

	/**
	* \brief Holds information on a texture loaded into GL
	*/
	class Texture
	{
	public:
		Texture();
		Texture(int _width, int _height, bool _base);
		Texture(std::string _path);
		~Texture();

		/**
		* \brief Retrieve the width and height of the Texture
		*/
		glm::ivec2 getSize();
		/**
		* \brief Get the Texture Id
		*/
		GLuint getId();

	protected:
		friend class Context;

		GLuint m_id; //!< GL Id of Texture
		glm::ivec2 m_size; //!< Width and height of Texture
		Context* m_context; //!< Pointer to glwrap context

	};
}
#endif