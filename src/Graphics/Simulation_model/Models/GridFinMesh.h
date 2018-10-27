/* Instances of this class are owned by the Falcon9Model object.
 * This class does not actually own a graphical model or mesh instance, it simply functions as a calculator to help the Falcon9Model.
 * The Falcon9Model gives this class the final transform (in OpenGL space) of the fuselage of the vehicle, and this class then 
 * calculates the final output transforms (again in OpenGL space) of the fin and the hinge. The Falcon9Model requests these transforms later
 * on, to update the model matrices of the actual mesh objects representing the fins.
*/

#ifndef GRAPHICS_GRIDFINMESH_H
#define GRAPHICS_GRIDFINMESH_H
#pragma once

#include "Graphics/Simulation_model/Models/IStageComponentMesh.hpp"
#include "definitions.h"

#include <glm/mat4x4.hpp>
#include <vector>
#include <algorithm>

namespace Physics {
	namespace Hardware {
		class GridFin;
	}
}

namespace Graphics {
	
	class GridFinMesh : public IStageComponentMesh {
	private:
		static unsigned char mNumInstances;
		const Physics::Hardware::GridFin& mDataSource;
		
		GF::Graphics::Mesh
			*mHingeMesh = nullptr,
			*mFinMesh = nullptr;

	public:
		GridFinMesh(const Physics::Hardware::GridFin& dataSource, GF::ResourceSet& resourceBucket, GF::Model3D& parentModel);
		~GridFinMesh() = default;

	private:
		virtual void loadResources();
		virtual void updateResources(glm::mat4 stageModelTransform_OGL);
	};

}

#endif
