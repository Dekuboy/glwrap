#ifndef _GLWRAP_OBJF
#define _GLWRAP_OBJF
#include <vector>
#include <glm/glm.hpp>

namespace glwrap
{
	class ObjAnimation;
	class ObjPart;

	/**
	* \brief Store information on part movement during animation frame
	*/
	class ObjTranslation
	{
	public:
		ObjTranslation();
		ObjTranslation(ObjPart* _part, glm::vec3 _position,
			glm::vec3 _rotation);
		~ObjTranslation();
		
		/**
		* \brief Get ObjPart that Translation applies to
		*/
		ObjPart* getPart();

		float getX();
		float getY();
		float getZ();

		float getRelativeX();
		float getRelativeY();
		float getRelativeZ();

		void setX(float _x);
		void setY(float _y);
		void setZ(float _z);

		float getXRotation();
		float getYRotation();
		float getZRotation();

		void setXRotation(float _xRotation);
		void setYRotation(float _yRotation);
		void setZRotation(float _ZRotation);

	private:
		ObjPart* m_part; //!< ObjPart that Translation applies to
		
		float m_x;
		float m_y;
		float m_z;

		float m_xRotation;
		float m_yRotation;
		float m_zRotation;

	};

	// ------------------------------------------------------------------

	/**
	* \brief Contains information on part positions during frame of animation
	*/
	class ObjFrame
	{
	public:
		ObjFrame();

		/**
		* \brief Copy _source values to _destination
		*/
		static void copy(ObjFrame* _source, ObjFrame* _destination);
		/**
		* \brief Merge from _source to _destination and store in _destination
		*/
		static void merge(ObjFrame* _source, ObjFrame* _destination,
			double _weight = 0.5f);

		/**
		* \brief Get list of different part movements in frame
		*/
		std::vector<ObjTranslation> getTranslations();
		/**
		* \brief Get translation of a specific part of the object 
		* -if _add, add a translation to base position if there is no part translation
		*/
		ObjTranslation* getTranslation(ObjPart* _part, bool _add = false);

	private:
		friend class ObjAnimation;

		std::vector<ObjTranslation> m_translations; //!< The translations that apply to separate parts during frame

	};
}

#endif