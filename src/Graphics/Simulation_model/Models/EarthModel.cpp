#include "EarthModel.h"

#include <GraphicsFramework/Vendor/ImGui/imgui.h>

namespace Graphics {

	constexpr unsigned EarthModel::mSegments;
	constexpr double EarthModel::mChunkAngle_degs;  
	constexpr double EarthModel::mRadius;
	constexpr glm::dvec3 
			EarthModel::mHorizontalRefAxis,
			EarthModel::mVerticalRefAxis; 

	EarthModel::EarthModel(GF::ResourceSet& resourceBucket) :
		mResourceBucket(resourceBucket),
		tempCamera(glm::vec3(0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.1f, 6378137.0, 0.0f, 45.0f)  
	{	
		loadResources();
	}

	void EarthModel::render(const SimulationCamera& activeSimCam, const GF::CoordTransform3D& eunToEcef) {
		//Copy the required state of the currently active camera
		const GF::Camera& cam = activeSimCam.getInternalCamera_immutable();
		tempCamera.setFront(cam.getFront());
		tempCamera.setUp(cam.getUp());
		tempCamera.setFOVY(cam.getFOVY());
		tempCamera.setAspect(cam.getAspect());

		//This is where the lat-lon earth mesh is generated based purely on the EUN position of the camera.
		glm::dvec3 cameraPos_EUN = activeSimCam.getPosition();		
		//updateMeshStructure(cameraPos_EUN, eunToEcef);
		
		updateTransforms_OGL(cameraPos_EUN);

		mNearTerrain->sendRenderCommand(mRenderer);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		mRenderer.flush();
	}

	void EarthModel::loadResources() {
		mShader = mResourceBucket.getResource<GF::Graphics::Shader>("bodyShader");

		mNearTerrain = mResourceBucket.addMesh("nearTerrain", GL_TRIANGLES, nullptr, mShader);

		GF::Graphics::VertexFormat positionFormat(0, 3, GL_FLOAT);
		mNearPosBuffer = mResourceBucket.addVertexBuffer("nearTerrainPosBuffer", GL_STATIC_DRAW, { 0, 3, GL_FLOAT }, mNearTerrainPositionData);
		mNearTerrain->addBuffer(mNearPosBuffer);

		mNearIndexBuffer = mResourceBucket.addIndexBuffer("nearTerrainIndices", GL_STATIC_DRAW, mNearTerrainIndexData);
		mNearTerrain->addIndexBuffer(mNearIndexBuffer);

		//Set renderer camera
		mRenderer.setCamera(tempCamera);
	}

	void EarthModel::updateMeshStructure(glm::dvec3 camPosition_EUN, const GF::CoordTransform3D& eunToEcef) {
#if 0
		using namespace glm;
		
		const dvec3 P = normalize(camPosition_EUN - mCentrePosition_highP);

		const dvec2 axisAngles {
			-degrees(orientedAngle(normalize(dvec3(P.x, 0.0, P.z)), mHorizontalRefAxis, mVerticalRefAxis)),
			-degrees(orientedAngle(P, mVerticalRefAxis, cross(P, mVerticalRefAxis)))
		};
		
		//The modulo of the axis angles for the chunk directly beneath the camera. This is the 'base' chunk reference position.
		const dvec2 baseModuloAxisAngles = floor(axisAngles / mChunkAngle_degs) * mChunkAngle_degs;

		if(mLastModuloAxisAngles == baseModuloAxisAngles)
			return;

		mLastModuloAxisAngles = baseModuloAxisAngles;

		mNearTerrainPositionData.clear();
		mNearTerrainIndexData.clear();

		//Add position data
		for(unsigned h = 0; h < mSegments; h++) {
			for(unsigned v = 0; v < mSegments; v++) {
				dvec3 vertexPos_BL = getChunkPosition_world(baseModuloAxisAngles + dvec2(h, v) * mChunkAngle_degs);
				
				if(degrees(abs(angle(normalize(vertexPos_BL), P))) < 30.0) {
					
					//Bottom left vertex
					mNearTerrainPositionData.push_back(vertexPos_BL.x);
					mNearTerrainPositionData.push_back(vertexPos_BL.y);
					mNearTerrainPositionData.push_back(vertexPos_BL.z);
					
					//Top left vertex
					dvec3 vertexPos_TL = getChunkPosition_world(baseModuloAxisAngles + dvec2(h, v) * mChunkAngle_degs + dvec2(mChunkAngle_degs, 0.0));
					mNearTerrainPositionData.push_back(vertexPos_TL.x);
					mNearTerrainPositionData.push_back(vertexPos_TL.y);
					mNearTerrainPositionData.push_back(vertexPos_TL.z);

					//Top right vertex
					dvec3 vertexPos_TR = getChunkPosition_world(baseModuloAxisAngles + dvec2(h, v) * mChunkAngle_degs + dvec2(mChunkAngle_degs, mChunkAngle_degs));
					mNearTerrainPositionData.push_back(vertexPos_TR.x);
					mNearTerrainPositionData.push_back(vertexPos_TR.y);
					mNearTerrainPositionData.push_back(vertexPos_TR.z);
					
					//Bottom right
					dvec3 vertexPos_BR = getChunkPosition_world(baseModuloAxisAngles + dvec2(h, v) * mChunkAngle_degs + dvec2(0.0, mChunkAngle_degs));
					mNearTerrainPositionData.push_back(vertexPos_BR.x);
					mNearTerrainPositionData.push_back(vertexPos_BR.y);
					mNearTerrainPositionData.push_back(vertexPos_BR.z);
				}
			}
		}

		//Index data

		const unsigned chunkCount = mNearTerrainPositionData.size() / 12;

		for(unsigned i = 0; i < chunkCount; i++) {
			mNearTerrainIndexData.push_back(i * 4);			
			mNearTerrainIndexData.push_back(i * 4 + 1);			
			mNearTerrainIndexData.push_back(i * 4 + 2);			

			mNearTerrainIndexData.push_back(i * 4 + 2);			
			mNearTerrainIndexData.push_back(i * 4 + 3);			
			mNearTerrainIndexData.push_back(i * 4);			
		}

		mNearPosBuffer->updateData(mNearTerrainPositionData);
		mNearIndexBuffer->updateData(mNearTerrainIndexData);
#else
		using namespace glm;

		const dvec3 temp = eunToEcef.toParentSpace(camPosition_EUN);

		//ImGui::Text("%f, %f, %f\n", temp.x, temp.y, temp.z);

		const dvec3 P = normalize(eunToEcef.toParentSpace(camPosition_EUN));

		const dvec2 axisAngles {
			-degrees(orientedAngle(normalize(dvec3(P.x, 0.0, P.z)), mHorizontalRefAxis, mVerticalRefAxis)),
			-degrees(orientedAngle(P, mVerticalRefAxis, cross(P, mVerticalRefAxis)))
		};

		//The modulo of the axis angles for the chunk directly beneath the camera. This is the 'base' chunk reference position.
		const dvec2 baseModuloAxisAngles = floor(axisAngles / mChunkAngle_degs) * mChunkAngle_degs;

		if(mLastModuloAxisAngles == baseModuloAxisAngles)
			return;

		mLastModuloAxisAngles = baseModuloAxisAngles;

		mNearTerrainPositionData.clear();
		mNearTerrainIndexData.clear();

		//Add position data
		for(unsigned h = 0; h < mSegments; h++) {
			for(unsigned v = 0; v < mSegments; v++) {
				dvec3 vertexPos_BL = eunToEcef.toLocalSpace(getChunkPosition_ecef(baseModuloAxisAngles + dvec2(h, v) * mChunkAngle_degs));

				//Why is this not working? -----------------------------------------------
				if(degrees(angle(normalize(vertexPos_BL), P)) < 30.0) {
					
					//Bottom left vertex
					mNearTerrainPositionData.push_back(vertexPos_BL.x);
					mNearTerrainPositionData.push_back(vertexPos_BL.y);
					mNearTerrainPositionData.push_back(vertexPos_BL.z);
					
					//Top left vertex
					dvec3 vertexPos_TL = eunToEcef.toLocalSpace(getChunkPosition_ecef(baseModuloAxisAngles + dvec2(h, v) * mChunkAngle_degs + dvec2(mChunkAngle_degs, 0.0)));
					mNearTerrainPositionData.push_back(vertexPos_TL.x);
					mNearTerrainPositionData.push_back(vertexPos_TL.y);
					mNearTerrainPositionData.push_back(vertexPos_TL.z);

					//Top right vertex
					dvec3 vertexPos_TR = eunToEcef.toLocalSpace(getChunkPosition_ecef(baseModuloAxisAngles + dvec2(h, v) * mChunkAngle_degs + dvec2(mChunkAngle_degs, mChunkAngle_degs)));
					mNearTerrainPositionData.push_back(vertexPos_TR.x);
					mNearTerrainPositionData.push_back(vertexPos_TR.y);
					mNearTerrainPositionData.push_back(vertexPos_TR.z);
					
					//Bottom right
					dvec3 vertexPos_BR = eunToEcef.toLocalSpace(getChunkPosition_ecef(baseModuloAxisAngles + dvec2(h, v) * mChunkAngle_degs + dvec2(0.0, mChunkAngle_degs)));
					mNearTerrainPositionData.push_back(vertexPos_BR.x);
					mNearTerrainPositionData.push_back(vertexPos_BR.y);
					mNearTerrainPositionData.push_back(vertexPos_BR.z);
				}
			}
		}

		//Index data
		const unsigned chunkCount = mNearTerrainPositionData.size() / 12;

		for(unsigned i = 0; i < chunkCount; i++) {
			mNearTerrainIndexData.push_back(i * 4);			
			mNearTerrainIndexData.push_back(i * 4 + 1);			
			mNearTerrainIndexData.push_back(i * 4 + 2);			

			mNearTerrainIndexData.push_back(i * 4 + 2);			
			mNearTerrainIndexData.push_back(i * 4 + 3);			
			mNearTerrainIndexData.push_back(i * 4);			
		}

		mNearPosBuffer->updateData(mNearTerrainPositionData);
		mNearIndexBuffer->updateData(mNearTerrainIndexData);
#endif
	}

	void EarthModel::updateTransforms_OGL(glm::dvec3 cameraPosition_EUN) {
		mNearTerrain->setModelTransform(inverse(translate(cameraPosition_EUN)));
	}
	
	glm::dvec3 EarthModel::getChunkPosition_ecef(glm::dvec2 moduloAxisAngles) const {
		glm::dvec3 pos = glm::rotate(mVerticalRefAxis * mRadius, glm::radians(moduloAxisAngles.y), cross(mHorizontalRefAxis, mVerticalRefAxis));
		pos = glm::rotate(pos, glm::radians(moduloAxisAngles.x), mVerticalRefAxis);
		return pos;
	}

}