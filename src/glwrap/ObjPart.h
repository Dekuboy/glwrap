#include <glwrap/ArrayPiece.h>
#include <list>
#include <string>

namespace glwrap
{
	struct TriFace;

	class Context;
	class VertexArray;
	class VertexBuffer;
	class Material;
	class ObjMtlModel;
	class ObjAnimation;

	/**
	* \brief Stores information on individual parts of an obj model
	*/
	class ObjPart : private ArrayPiece
	{
	public:
		ObjPart(std::string _name);
		~ObjPart();

	private:
		friend class Context;
		friend class VertexArray;
		friend class ObjMtlModel;

		std::vector<ObjAnimation>* m_animationList; //!< Parents list of animations

		bool m_useMaterial; //!< Stores if the part is separated by Material

		ObjPart* m_self; //!< Pointer to self to find Translation referring to this part

		/**
		* \brief Add tri face to list
		*/
		void addFace(TriFace* _face);

		/**
		* \brief Draws vertex arrays
		*/
		void drawArrays();
		/**
		* \brief Translate part by base obj animation
		*/
		void translate();

		/**
		* \brief Draw all vertex arrays represented within this part
		*/
		void draw();
		/**
		* \brief Draw all vertex arrays represented within this part that appear in view
		* -useful for large objects where parts are too distant to always be in view
		*/
		void cullAndDraw(bool _cullAnimated);

	};
}