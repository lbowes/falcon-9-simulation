/* Instances of this class are owned by the Falcon9Model object.
 * This class does not actually own a graphical model or mesh instance, it simply functions as a calculator to help the Falcon9Model.
 * The Falcon9Model gives this class the final transform (in OpenGL space) of the fuselage of the vehicle, and this class then 
 * calculates the final output transforms (again in OpenGL space) of the fin and the hinge. The Falcon9Model requests these transforms later
 * on, to update the model matrices of the actual mesh objects representing the fins.
*/

#ifndef GRAPHICS_GRIDFINMESH_H
#define GRAPHICS_GRIDFINMESH_H
#pragma once

#include <glm/mat4x4.hpp>
#include <vector>
#include <algorithm>

namespace Physics {
	namespace Hardware {
		class GridFin;
	}
}

namespace Graphics {
	
	class GridFinMesh {
	private:
		Physics::Hardware::GridFin &mDataSource;

		glm::mat4
		  mHingeTransform_OGL,
		  mFinTransform_OGL;

	public:
	  	GridFinMesh(Physics::Hardware::GridFin &dataSource);
	  	~GridFinMesh() = default;

	  	void updateTransforms_OGL(glm::mat4 stageModelTransform_OGL);

	  	glm::mat4 getFinTransform_OGL() const { return mFinTransform_OGL; }
	  	glm::mat4 getHingeTransform_OGL() const { return mHingeTransform_OGL; }

	};

}

#endif
