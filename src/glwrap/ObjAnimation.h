#include <glwrap/Animation.h>
#include <glwrap/ObjFrame.h>
#include <string>

namespace glwrap
{
	class VertexArray;
	class Context;

	/**
	* \brief Holds information to apply Animation to .obj
	* -Object must hold appropriately named parts in order to apply animation
	* -Animations apply rotation and translation to individual parts
	* -Animations are made up of keyframes to interpolate between
	*/
	class ObjAnimation : public Animation
	{
	public:
		ObjAnimation();
		ObjAnimation(VertexArray* _model);
		~ObjAnimation();

		/**
		* \brief Get current frame of animation in use
		*/
		ObjFrame* getFrame();
		/**
		* \brief Get interpolated frame between two keyframes
		*/
		ObjFrame* getMergeFrame();
		/**
		* \brief Get max frames of animation
		*/
		int getMaxFrames();
		/**
		* \brief Move frame of animation forward by _deltaTime
		*/
		void nextFrame(float _deltaTime);

	private:
		friend class Context;

		VertexArray* m_model; //!< Animation attached to Obj
		std::vector<ObjFrame> m_frames; //!< Information on the frames of animation
		ObjFrame m_mergeFrame; //!< Interpolated frame at current animation time

		/**
		* \brief .obj Animation file is read and converted to keyframes
		* -"n" denotes name of animation
		* -"f" denotes new frame of animation
		* -"t" denotes transformation (translate is multiplied by partSize/100 when parsed)
		* -transformation format:
		* t PartName xTranslate yTranslate zTranslate xAngle yAngle zAngle
		*/
		void parse(std::string _path);

		/**
		* \brief Split the file string of white space
		*/
		void splitString(std::string input, char splitter, 
			std::vector<std::string>* output);
		/**
		* \brief Generate interpolated frame between keyframes
		*/
		void generateMergeFrame(ObjFrame* a, 
			ObjFrame* b, double weight);

	};
}