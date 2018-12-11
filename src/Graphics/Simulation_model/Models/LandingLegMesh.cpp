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
		for (unsigned char i = 0; i < piston->getSubCylinderLengths().size(); i++) {
			const std::string objFilepath = BOX_MODELS ? "res/models/PistonCylinder_box.obj" : "res/models/PistonCylinder_" + std::to_string(i) + ".obj";
			mPistonCylinderMeshes.push_back(mResourceBucket.addOBJMesh("piston" + std::to_string(i) + "leg" + id, objFilepath, GL_TRIANGLES, nullptr, shader));
			mParentModel.addMesh(mPistonCylinderMeshes.back());
		}

		//Pusher
		mPusherMesh = mResourceBucket.addOBJMesh("leg" + id + "pusher", "res/models/UnitCylinder.obj", GL_TRIANGLES, nullptr, shader);
		mParentModel.addMesh(mPusherMesh);
	}

	void LandingLegMesh::updateResources(glm::mat4 stageModelTransform_OGL) 
		//Responsible for recalculating the mesh transforms of the A frame and piston cylinders based on the mDataSource and 
		//stageModelTransform_OGL, and updating the meshes with this transform
	{
		using namespace glm;

		const float clockDegree_rads = glm::radians(mDataSource.mClockingDegree);

		//A-Frame mesh updates
		{
			mat4 AFrameTransform = stageModelTransform_OGL * mat4(mDataSource.getCompToStageTransform().getLocalToParent_total());
			mAFrameMesh->setModelTransform(AFrameTransform);
		}

		//Telescoping piston mesh updates
		{
			//Using the information about mParent, this function needs to update the OpenGL transform of the cylinders along the telescoping piston.
			Physics::Hardware::TelescopingPiston* piston = mDataSource.getPiston();
		
			const std::vector<double> cylinderLengths = piston->getSubCylinderLengths();
			float lengthCovered = 0.0f;			
			for (unsigned char i = 0; i < cylinderLengths.size(); i++) {
				float 
					pistonLength = piston->getLength(),
					alongPiston = 0.0;
				
				if(lengthCovered + cylinderLengths[i] < pistonLength) {
					alongPiston = lengthCovered;
					lengthCovered += cylinderLengths[i];
				}
				else
					alongPiston = lengthCovered - ((lengthCovered + cylinderLengths[i]) - pistonLength);

				//Rotate the cylinder around the stage
				mat4 pistonFinalTransform = rotate(clockDegree_rads, vec3(0.0f, 1.0f, 0.0f));
		
				//Rotate the cylinder downwards to line up with the piston centre line
				const float angleFromVertical = glm::angle(normalize(mDataSource.mAlongPiston_stage3D), { 0.0, 1.0, 0.0 });
				pistonFinalTransform = rotate(pistonFinalTransform, angleFromVertical, vec3(-1.0f, 0.0f, 0.0f));
		
				//Slide the cylinder into the correct position along the piston centre line
				pistonFinalTransform = translate(pistonFinalTransform, vec3(0.0f, alongPiston, 0.0f));
		
				//Translate the cylinder upwards to the mounting point on the stage
				mat4 pistonTranslation = translate(mDataSource.getPistonStartPoint_stage());
				pistonFinalTransform = stageModelTransform_OGL * pistonTranslation * pistonFinalTransform;
		
				//Apply this newly-calculated transform to the piston mesh
				mPistonCylinderMeshes[i]->setModelTransform(pistonFinalTransform);
			}
		}

		//Deployment pusher mesh updates			
		{
			const float diameter = 0.05f;

			//A scale transform to convert the unit cylinder into the a cylinder with different proportions to represent the deployment pusher
			mat4 pusherScale = scale(vec3(diameter, static_cast<float>(mDataSource.getDeploymentActuator()->getLength()), diameter));

			mat4 pusherTransform = rotate(clockDegree_rads, vec3(0.0f, 1.0f, 0.0f));

			const float angleFromVertical = glm::angle(normalize(mDataSource.mAlongPusher_stage3D), { 0.0, 1.0, 0.0 });
			pusherTransform = rotate(pusherTransform, angleFromVertical, vec3(-1.0f, 0.0f, 0.0f));

			mat4 pusherTranslation = translate(mDataSource.getPusherStartPoint_stage());
			pusherTransform = stageModelTransform_OGL * pusherTranslation * pusherTransform * pusherScale;
			
			mPusherMesh->setModelTransform(pusherTransform);
		}
	}

}