#include <string>
#include <glm/glm.hpp>

namespace glwrap
{
	class GltfModel;
	class ModelJoint;
	class ObjMtlModel;
	class ObjPart;
	class Texture;

	/**
	* Holds Texture Information for Models
	*/
	class Material
	{
	public:
		/**
		* \brief Retrieve path to Texture for loading
		*/
		std::string getTexturePath();
		/**
		* \brief Set Texture to draw with when Material is used
		*/
		void setTexture(Texture* _texture);

		/**
		* \brief Retrieve path to Metallic Roughness map for loading
		*/
		std::string getMdetalRoughPath();
		/**
		* \brief Set Metallic Roughness map to draw with when Material is used
		*/
		void setMetalRough(Texture* _texture);

		/**
		* \brief Retrieve path to Normal map for loading
		*/
		std::string getNormalPath();
		/**
		* \brief Set Normal map to draw with when Material is used
		*/
		void setNormal(Texture* _texture);

		/**
		* \brief Retrieve path to Occlusion map for loading
		*/
		std::string getOcclusionPath();
		/**
		* \brief Set Occlusion map to draw with when Material is used
		*/
		void setOcclusion(Texture* _texture);

		/**
		* \brief Retrieve path to Emissive map for loading
		*/
		std::string getEmissivePath();
		/**
		* \brief Set Emissive map to draw with when Material is used
		*/
		void setEmissive(Texture* _texture);

	private:
		friend class GltfModel;
		friend class ModelJoint;
		friend class ObjMtlModel;
		friend class ObjPart;

		std::string m_name; //!< Name of Material

		std::string m_colourPath; //!< Path to Texture File
		Texture* m_colourMap = nullptr; //!< Texture to apply when using Material
		char m_colourAttrib; //!< Texture attribute Id in shader
		glm::vec4 m_colourFactor; //!< Texture colour scale factor

		std::string m_metalRoughPath; //!< Path to Metallic Roughness File
		Texture* m_metalRoughMap = nullptr; //!< Metallic Roughness to apply when using Material
		char m_metalRoughAttrib; //!< Metallic Roughness attribute Id in shader
		glm::vec2 m_metalRoughFactor; //!< Metallic Roughness scale factor

		std::string m_normalPath; //!< Path to Normal File
		Texture* m_normalMap = nullptr; //!< Normal to apply when using Material
		char m_normalAttrib; //!< Normal attribute Id in shader
		float m_normalFactor; //!< Normal scale factor

		std::string m_occlusionPath; //!< Path to Occlusion File
		Texture* m_occlusionMap = nullptr; //!< Occlusion to apply when using Material
		char m_occlusionAttrib; //!< Occlusion attribute Id in shader
		float m_occlusionFactor; //!< Occlusion scale factor

		std::string m_emissivePath; //!< Path to Emissive File
		Texture* m_emissiveMap = nullptr; //!< Emission to apply when using Material
		char m_emissiveAttrib; //!< Metallic Roughness attribute Id in shader
		glm::vec3 m_emissiveFactor; //!< Metallic Roughness scale factor

	};
}