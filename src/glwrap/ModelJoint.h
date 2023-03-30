#include <glwrap/ArrayPiece.h>
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
	class ModelJoint : private ArrayPiece
	{
	public:
		ModelJoint(std::string _name);
		~ModelJoint();

	private:
		friend class Context;
		friend class GltfModel;

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
		* \brief Draw all vertex arrays represented within this mesh that appear in view
		* -useful for large models where meshes are too distant to always be in view
		*/
		void cullAndDraw();

	};
}