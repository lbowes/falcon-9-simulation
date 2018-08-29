#include "EarthModel.h"

#include <GraphicsFramework/Vendor/ImGui/imgui.h>

namespace Graphics {

	constexpr unsigned EarthModel::mSegments;
	constexpr double EarthModel::mChunkAngle_degs;  
	constexpr double EarthModel::mRadius;
	constexpr glm::dvec3
		EarthModel::mCentrePosition_highP,
		EarthModel::mHorizontalRefAxis,
		EarthModel::mVerticalRefAxis;
	
	//EarthModel::ModelReferenceAxes EarthModel::mDynamicRefAxes;

	EarthModel::EarthModel(GF::ResourceSet& resourceBucket) :
		mResourceBucket(resourceBucket),
		tempCamera(glm::vec3(0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.1f, 1000.0f, 0.0f, 45.0f)  
	{	
		loadResources();
	}

	void EarthModel::render(const SimulationCamera& currentSimCamera) {
		//Copy the required state of the currently active camera
		const GF::Camera& cam = currentSimCamera.getInternalCamera_immutable();
		tempCamera.setFront(cam.getFront());
		tempCamera.setUp(cam.getUp());
		tempCamera.setFOVY(cam.getFOVY());
		tempCamera.setAspect(cam.getAspect());

		//updateDynamicAxes(currentSimCamera.getState());
		updateMeshStructure(currentSimCamera.getState());

		updateAllTransforms_OGL(currentSimCamera.getState());

		mNearTerrain->sendRenderCommand(mRenderer);

		//temp
		//mSphereMesh_temp->sendRenderCommand(mRenderer);
		//

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		mRenderer.flush();
	}

	void EarthModel::loadResources() {
		mShader = mResourceBucket.getResource<GF::Graphics::Shader>("bodyShader");

		mNearTerrain = mResourceBucket.addMesh("nearTerrain", GL_TRIANGLES, nullptr, mShader);

		//temp
		mSphereMesh_temp = mResourceBucket.addOBJMesh("sphere", "res/models/Sphere.obj", GL_TRIANGLES, nullptr, mShader);
		//

		GF::Graphics::VertexFormat positionFormat(0, 3, GL_FLOAT);
		mNearPosBuffer = mResourceBucket.addVertexBuffer("nearTerrainPosBuffer", GL_STATIC_DRAW, { 0, 3, GL_FLOAT }, mNearTerrainPositionData);
		mNearTerrain->addBuffer(mNearPosBuffer);

		mNearIndexBuffer = mResourceBucket.addIndexBuffer("nearTerrainIndices", GL_STATIC_DRAW, mNearTerrainIndexData);
		mNearTerrain->addIndexBuffer(mNearIndexBuffer);

		//Set renderer camera
		mRenderer.setCamera(tempCamera);
	}

	void EarthModel::updateAllTransforms_OGL(SimpleCameraState currentCameraState) {
		glm::mat4 transform = inverse(translate(currentCameraState.mPosition_highP - mCentrePosition_highP));
		mNearTerrain->setModelTransform(transform);

		//temp
		glm::dvec3 pos = currentCameraState.mPosition_highP - mCentrePosition_highP;
		transform = inverse(translate(pos));
		mSphereMesh_temp->setModelTransform(transform);
		//
	}

	void EarthModel::updateMeshStructure(SimpleCameraState currentCameraState) {
#if 1
		using namespace glm;
		
		const dvec3 P = normalize(currentCameraState.mPosition_highP - mCentrePosition_highP);

		const dvec2 axisAngles {
			-degrees(orientedAngle(normalize(dvec3(P.x, 0.0, P.z)), mHorizontalRefAxis, mVerticalRefAxis)),
			-degrees(orientedAngle(P, mVerticalRefAxis, cross(P, mVerticalRefAxis)))
		};
		
		//The modulo of the axis angles for the chunk directly beneath the camera. This is the 'base' chunk reference position.
		const dvec2 baseModuloAxisAngles = floor(axisAngles / mChunkAngle_degs) * mChunkAngle_degs;

		ImGui::Text("axisAngles: %f, %f\n", axisAngles.x, axisAngles.y);
		ImGui::Text("currentCameraState.mPosition_highP: %f, %f, %f\n", currentCameraState.mPosition_highP.x, currentCameraState.mPosition_highP.y, currentCameraState.mPosition_highP.z);
		ImGui::Text("mHorizontalRefAxis: %f, %f, %f\n", mHorizontalRefAxis.x, mHorizontalRefAxis.y, mHorizontalRefAxis.z);

		if(mCurrentModuloAxisAngles == baseModuloAxisAngles)
			return;

		mCurrentModuloAxisAngles = baseModuloAxisAngles;

		mNearTerrainPositionData.clear();
		mNearTerrainIndexData.clear();

		//Add position data
		for(unsigned h = 0; h < mSegments; h++) {
			for(unsigned v = 0; v < mSegments; v++) {
				dvec3 vertexPos_BL = getChunkPosition_world(baseModuloAxisAngles + dvec2(h, v) * mChunkAngle_degs);
				
				if(degrees(angle(normalize(vertexPos_BL), P)) < 30.0) {
					
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
		//for(int x = 0; x < gridSize_chunks; x++) {
		//	for(int y = 0; y < gridSize_chunks; y++) {
		//		unsigned 
		//			chunkCount = x * gridSize_chunks + y,
		//			xIndex = (chunkCount - (chunkCount % gridSize_chunks)) / gridSize_chunks,
		//			yIndex = chunkCount - xIndex * (gridSize_chunks + 1);

		//		mNearTerrainIndexData.push_back(chunkCount + xIndex);
		//		mNearTerrainIndexData.push_back(chunkCount + xIndex + 1);
		//		mNearTerrainIndexData.push_back(chunkCount + xIndex + 1 + (gridSize_chunks + 1));
		//		
		//		mNearTerrainIndexData.push_back(chunkCount + xIndex + 1 + (gridSize_chunks + 1));
		//		mNearTerrainIndexData.push_back(chunkCount + xIndex + (gridSize_chunks + 1));
		//		mNearTerrainIndexData.push_back(chunkCount + xIndex);
		//	}
		//}

		//Index data
		const unsigned chunkCount = mNearTerrainPositionData.size() / 12;

		ImGui::Text("chunkCount: %i\n", chunkCount);

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

		const double chunkAngle_rads = radians(mChunkAngle_degs);

		const dvec3 axisForVerticalRots = cross(mDynamicRefAxes.mHorizontal, mDynamicRefAxes.mVertical);

		const int gridSize_chunks = 16;

		mNearTerrainPositionData.clear();
		mNearTerrainIndexData.clear();

		//Add position data
		for(int h = -gridSize_chunks / 2; h <= gridSize_chunks / 2; h++) {
			for(int v = -gridSize_chunks / 2; v <= gridSize_chunks / 2; v++) {
				dvec3 vertexPos = mCentrePosition_highP + mRadius * rotate(rotate(mDynamicRefAxes.mHorizontal, h * chunkAngle_rads, mDynamicRefAxes.mVertical), v * chunkAngle_rads, axisForVerticalRots);

				mNearTerrainPositionData.push_back(vertexPos.x);
				mNearTerrainPositionData.push_back(vertexPos.y);
				mNearTerrainPositionData.push_back(vertexPos.z);
			}
		}

		//Index data
		for(int x = 0; x < gridSize_chunks; x++) {
			for(int y = 0; y < gridSize_chunks; y++) {
				unsigned 
					chunkCount = x * gridSize_chunks + y,
					xIndex = (chunkCount - (chunkCount % gridSize_chunks)) / gridSize_chunks,
					yIndex = chunkCount - xIndex * (gridSize_chunks + 1);

				mNearTerrainIndexData.push_back(chunkCount + xIndex);
				mNearTerrainIndexData.push_back(chunkCount + xIndex + 1);
				mNearTerrainIndexData.push_back(chunkCount + xIndex + 1 + (gridSize_chunks + 1));
				
				mNearTerrainIndexData.push_back(chunkCount + xIndex + 1 + (gridSize_chunks + 1));
				mNearTerrainIndexData.push_back(chunkCount + xIndex + (gridSize_chunks + 1));
				mNearTerrainIndexData.push_back(chunkCount + xIndex);
			}
		}

		mNearPosBuffer->updateData(mNearTerrainPositionData);
		mNearIndexBuffer->updateData(mNearTerrainIndexData);
#endif
	}

	glm::dvec3 EarthModel::getChunkPosition_world(glm::dvec2 moduloAxisAngles) const {
		glm::dvec3 pos = glm::rotate(mVerticalRefAxis * mRadius, glm::radians(moduloAxisAngles.y), cross(mHorizontalRefAxis, mVerticalRefAxis));
		pos = glm::rotate(pos, glm::radians(moduloAxisAngles.x), mVerticalRefAxis);
		return pos;
	}

#if 0
	void EarthModel::updateDynamicAxes(SimpleCameraState currentCameraState) 
		//1. Get the horizontal and vertical angle displacements of the new position, from the current dynamic axes.
		//2. If either of the angles exceed 'mChunkAngle_degs' then shift the dynamic axes correctly.
	{
		using namespace glm;

		//Normalized camera position vector in earth space
		const dvec3 P = normalize(currentCameraState.mPosition_highP - mCentrePosition_highP);
		
		glm::dvec2 axisAngleDisplacement;
		
		axisAngleDisplacement.x = 
			degrees(
				orientedAngle(
					normalize(P - (dot(P, mDynamicRefAxes.mVertical) * mDynamicRefAxes.mVertical)), 
					mDynamicRefAxes.mHorizontal, 
					mDynamicRefAxes.mVertical
				)
			);

		const dvec3 normal = normalize(cross(P, mDynamicRefAxes.mVertical));
		axisAngleDisplacement.y = 
			degrees(
				orientedAngle(
					P,
					normalize(mDynamicRefAxes.mHorizontal - (dot(mDynamicRefAxes.mHorizontal, normal) * normal)),
					abs(normal)
				)
			);

		//Rotate the horizontal axis if necessary
		if(axisAngleDisplacement.x < -mChunkAngle_degs || axisAngleDisplacement.x >= 0.0) {
			const double numChunksMoved = ceil(axisAngleDisplacement.x / mChunkAngle_degs);
			mDynamicRefAxes.mHorizontal = rotate(mDynamicRefAxes.mHorizontal, radians(-numChunksMoved * mChunkAngle_degs), mDynamicRefAxes.mVertical);
		}
		
		//Rotate the vertical axis if necessary
		if(axisAngleDisplacement.y < -mChunkAngle_degs || axisAngleDisplacement.y >= 0.0) {
			const double numChunksMoved = ceil(axisAngleDisplacement.y / mChunkAngle_degs);
			const dvec3 rotationAxis = abs(cross(mDynamicRefAxes.mHorizontal, mDynamicRefAxes.mVertical));
		
			mDynamicRefAxes.mVertical = rotate(mDynamicRefAxes.mVertical, radians(-numChunksMoved * mChunkAngle_degs), rotationAxis);
			mDynamicRefAxes.mHorizontal = rotate(mDynamicRefAxes.mHorizontal, radians(-numChunksMoved * mChunkAngle_degs), rotationAxis);
		}
	}
#endif

}