#include "LandingLegMesh.h"
#include "Physics/Hardware/Falcon_9/Stage_1/Landing_legs/LandingLeg.h"

namespace Graphics {

	unsigned char LandingLegMesh::mNumInstances = 0;

	LandingLegMesh::LandingLegMesh(const Physics::Hardware::LandingLeg& dataSource, GF::ResourceSet& resourceBucket, GF::Model3D& parentModel) :
		IStageComponentMesh(resourceBucket, parentModel),
		mDataSource(dataSource)
	{
		loadResources();
		mNumInstances++;
	}

	void LandingLegMesh::loadResources() 
		//Responsible for adding whatever is needed to the mResourceBucket
	{
		GF::Graphics::Shader* shader = mResourceBucket.getResource<GF::Graphics::Shader>("bodyShader");

		std::string id = std::to_string(mNumInstances);

		//A-frame																		 
		mAFrameMesh = mResourceBucket.addOBJMesh("landingLeg" + id, BOX_MODELS ? "res/models/LandingLeg_Box.obj" : "res/models/LandingLeg.obj", GL_TRIANGLES, nullptr, shader);
		mParentModel.addMesh(mAFrameMesh);

		//Telescoping piston cylinders
		Physics::Hardware::TelescopingPiston* piston = mDataSource.getPiston();
		for (unsigned char i = 0; i < piston->getCylinders().size(); i++) {
			mPistonCylinderMeshes.push_back(mResourceBucket.addOBJMesh("piston" + std::to_string(i) + "leg" + id, BOX_MODELS ? "res/models/PistonCylinder_Box.obj" : "res/models/PistonCylinder.obj", GL_TRIANGLES, nullptr, shader));
			mParentModel.addMesh(mPistonCylinderMeshes.back());
		}
	}

	void LandingLegMesh::updateResources(glm::mat4 stageModelTransform_OGL) 
		//Responsible for recalculating the mesh transforms of the A frame and piston cylinders based on the mDataSource and 
		//stageModelTransform_OGL, and updating the meshes with this transform
	{
		using namespace glm;

		//A-Frame mesh updates
		{
			mat4 AFrameTransform = stageModelTransform_OGL * mat4(mDataSource.getCompToStageTransform().getLocalToParent_total());
			mAFrameMesh->setModelTransform(AFrameTransform);
		}

		//Piston cylinder mesh updates
		{
			//Using the information about mParent, this function needs to update the OpenGL transform of the cylinders along the telescoping piston.
			Physics::Hardware::TelescopingPiston* piston = mDataSource.getPiston();
		
			for (unsigned char i = 0; i < piston->getCylinders().size(); i++) {
				Physics::Hardware::PistonCylinder* cylinder = piston->getCylinder(i);
				
				float 
					cylinderLength = static_cast<float>(cylinder->getLength()),
					pistonLength = piston->getLength(),
					alongPiston = 0.0;
				
				mat4 pistonScale = scale(vec3(cylinder->getWidthScale(), cylinderLength, cylinder->getWidthScale()));

				if ((i + 1) * cylinderLength < pistonLength)
					alongPiston = i * cylinderLength;
				else
					alongPiston = (i * cylinderLength) - (((i + 1) * cylinderLength) - pistonLength);
		
				//Rotate the cylinder around the stage
				mat4 pistonFinalTransform = rotate(static_cast<float>(radians(piston->getClockingDegree_degs())), vec3(0.0f, 1.0f, 0.0f));
		
				//Rotate the cylinder downwards to line up with the piston centre line
				float angleFromVertical = glm::angle(normalize(mDataSource.mAlongPiston_stage3D), { 0.0, 1.0, 0.0 });
				pistonFinalTransform = rotate(pistonFinalTransform, static_cast<float>(angleFromVertical), vec3(-1.0f, 0.0f, 0.0f));
		
				//Slide the cylinder into the correct position along the piston centre line
				pistonFinalTransform = translate(pistonFinalTransform, vec3(0.0f, alongPiston, 0.0f));
		
				//Translate the cylinder upwards to the mounting point on the stage
				mat4 pistonTranslation = translate(vec3(piston->getMountPoint_stage()));
				pistonFinalTransform = stageModelTransform_OGL * pistonTranslation * pistonFinalTransform * pistonScale;
		
				//Apply this newly-calculated transform to the piston mesh
				mPistonCylinderMeshes[i]->setModelTransform(pistonFinalTransform);
			}
		}
	}

}