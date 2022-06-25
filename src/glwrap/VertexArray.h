#ifndef _GLWRAP_VERTEXARRAY
#define _GLWRAP_VERTEXARRAY
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glwrap/Model3D.h>

#include <vector>
#include <string>
namespace glwrap
{
	struct TriFace;

	class Context;
	class VertexBuffer;
	class ObjPart;
	class ObjAnimation;
	class ShaderProgram;

	/**
	* \brief Holds information on a model loaded into GL (obj)
	*/
	class VertexArray : public Model3D
	{
	public:
		VertexArray();
		virtual ~VertexArray();

		/**
		* \brief Sets buffer of name _attribute to "Default" part
		* -does not work if VertexArray has parsed an obj file
		*/
		VertexBuffer* setBuffer(std::string _attribute);

		/**
		* \brief Attach Animation to object from file path
		*/
		ObjAnimation* addAnimation(std::string _path);
		int playAnimationOnce(std::string _name);
		void playAnimationOnce(int _index);
		int enableAnimation(std::string _name);
		void enableAnimation(int _index);
		int enableOnlyAnimation(std::string _name);
		void enableOnlyAnimation(int _index);
		int disableAnimation(std::string _name);
		void disableAnimation(int _index);
		void disableAllAnimations();

		/**
		* \brief Retrieve all the parts that make up the model
		*/
		const std::vector<ObjPart>& getParts();
		/**
		* \brief Retrieve part that is called _name
		*/
		ObjPart* getPart(std::string _name);
		/**
		* Retrieve list of attached animations
		*/
		const std::vector<ObjAnimation>& getAnimations();

	protected:
		friend class Context;
		friend class ShaderProgram;

		std::vector<ObjPart*> m_parts; //!< Information on individual parts of the model
		std::vector<ObjAnimation> m_animations; //!< List of animations attached to the model
		VertexArray* m_self; //!< Pointer to self to set in individual parts

		/**
		* \brief Parse obj file and read into GL
		*/
		void parse(std::string _path);
		/**
		* \brief Parse obj file and read into Gl
		* -if(_tanBitan) calculate tangent bitangent attributes for TBN matrix
		*/
		virtual void parse(std::string, bool _tanBitan);

		/**
		* \brief Separates strings into substrings based on white space
		*/
		void splitStringWhitespace(std::string& _input, std::vector<std::string>& _output);
		/**
		* \brief Splits string based on input value as a splitter
		*/
		void splitString(std::string& _input, char _splitter, std::vector<std::string>& _output);
		/**
		* \brief Checks if vertex values are the minimum or maximum of the model
		*/
		void checkMinMax(glm::vec3& _vertexPosition);

		/**
		* \brief Calls upon the part list and draws vertex arrays
		* -to be called by ShaderProgram
		*/
		virtual void draw();
		/**
		* \brief Calls upon the part list and draws vertex arrays if in view
		* -to be called by ShaderProgram
		* -uses values from current active ShaderProgram to cull
		*/
		virtual void cullAndDraw();
		/**
		* \brief Draws individual part of object by name
		*/
		virtual void drawPart(std::string _partName);

	};
}

#endif