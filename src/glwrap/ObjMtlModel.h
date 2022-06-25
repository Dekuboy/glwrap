#include <glwrap/VertexArray.h>
#include <list>

namespace glwrap
{
	class Material;
	class Texture;

	/**
	* \brief Model class which utilises a .mtl file to find textures
	*/
	class ObjMtlModel : public VertexArray
	{
	public:
		ObjMtlModel();

		virtual ~ObjMtlModel();

		/**
		* \brief Retrieve of Material of input name
		*/
		Material* getMaterial(std::string _material);
		/**
		* \brief Retrieve the list of Material types
		*/
		std::vector<Material>& getMatList();

	private:
		friend class Context;
		friend class ShaderProgram;

		std::vector<Material> m_materialList;

		/**
		* \brief Parse .obj file alongside its .mtl file
		*/
		void parse(std::string _path);
		/**
		* \brief Parse .obj file alongside its .mtl file
		* -precalculate tangentBitangentNormal matrix if _tanBitan
		*/
		void parse(std::string _path, bool _tanBitan);

		void parseMtl(std::string _path);

	};
}