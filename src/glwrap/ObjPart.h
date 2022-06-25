#include <glwrap/VertexBuffer.h>
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
	class ObjPart
	{
	public:
		ObjPart(std::string _name);
		~ObjPart();

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

	private:
		friend class Context;
		friend class VertexArray;
		friend class ObjMtlModel;

		std::string m_name; //!< Name of ObjPart
		std::vector<GLuint> m_idList; //!< List of vertex array GL Ids

		bool m_dirty; //!< If a buffer has been added, buffer information must be sent to the GPU
		std::vector<TriFace*> m_faces; //!< List of tris in part
		std::vector<std::vector<VertexBuffer>> 
			m_buffers; //!< List of different buffers in part, separated by Material ([mat][])
		glm::mat4 m_animationUniform; //!< Transformation matrix based on animated movement
		std::vector<ObjAnimation>* m_animationList; //!< Parents list of animations

		bool m_useMaterial; //!< Stores if the part is separated by Material
		std::vector<Material*> m_materials; //!< List of Material used in part

		ObjPart* m_self; //!< Pointer to self to find Translation referring to this part
		Context* m_context; //!< Pointer to glwrap context

		float m_offsetX; //!< x Offset from obj centre
		float m_offsetY; //!< y Offset from obj centre
		float m_offsetZ; //!< z Offset from obj centre

		float m_maxX; //!< Max x value in part
		float m_maxY; //!< Max y value in part
		float m_maxZ; //!< Max z value in part
		float m_minX; //!< Min x value in part
		float m_minY; //!< Min y value in part
		float m_minZ; //!< Min z value in part

		/**
		* \brief Add tri face to list
		*/
		void addFace(TriFace* _face);

		/**
		* \brief Translate part by base obj animation
		*/
		void translate();
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