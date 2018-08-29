#ifndef GRAPHICS_EARTHMODEL_H
#define GRAPHICS_EARTHMODEL_H
#pragma once

#include "Graphics/Simulation_model/Cameras/AllCameras.h"

#include <GraphicsFramework/Renderer.h>
#include <GraphicsFramework/ResourceSet.h>
#include <GraphicsFramework/Mesh.h>
#include <GraphicsFramework/PerspectiveCamera.h>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/vector_angle.hpp>

namespace Graphics {

	class EarthCamera {
	private:
		std::unique_ptr<GF::PerspectiveCamera> 
			mNear,
			mFar;

	public:
		EarthCamera();
		~EarthCamera() = default;

	};

	class EarthModel {
	private:
		//A measure of the resolution of the spherical model approximation.
		static constexpr unsigned mSegments = 32;
	
		//The earth model will be an approximation of an ellipsoid (a sphere to begin with), made up of square panels on the surface called 'chunks'.
		//This angle represents the angle between chunk boundary points. The smaller the angle, the more chunks are wrapped round the sphere.
		//Must be a factor of 360.0 degrees.
		static constexpr double mChunkAngle_degs = 360.0 / mSegments;  
		
		//The radius of the (test) sphere in metres
		static constexpr double mRadius = 100.0; //100.0

		//The position of centre of the sphere
		static constexpr glm::dvec3 mCentrePosition_highP = glm::dvec3(0.0, -mRadius, 0.0);

		//Reference axes
		static constexpr glm::dvec3 
			mHorizontalRefAxis = { 1.0, 0.0, 0.0 },
			mVerticalRefAxis = { 0.0, 1.0, 0.0 }; 

		glm::dvec2 mCurrentModuloAxisAngles = { 0.0, 0.0 };

		GF::Graphics::Mesh* mSphereMesh_temp = nullptr;

		GF::Graphics::Mesh
			*mNearTerrain = nullptr;
			//*mFarTerrain = nullptr;

		GF::Graphics::VertexBuffer
			*mNearPosBuffer = nullptr;
			//*mFarPosBuffer = nullptr;

		std::vector<float> mNearTerrainPositionData = { };

		GF::Graphics::IndexBuffer* mNearIndexBuffer = nullptr;

		std::vector<unsigned> mNearTerrainIndexData = { };

		GF::ResourceSet& mResourceBucket;
		GF::Graphics::Renderer mRenderer;
		GF::Graphics::Shader* mShader = nullptr;

		//temp
		GF::PerspectiveCamera tempCamera;
		//

	public:
		EarthModel(GF::ResourceSet& resourceBucket);
		~EarthModel() = default;

		void render(const SimulationCamera& currentSimCamera);

	private:
		void loadResources();
		void updateAllTransforms_OGL(SimpleCameraState currentCameraState);
		void updateMeshStructure(SimpleCameraState currentCameraState);
		glm::dvec3 getChunkPosition_world(glm::dvec2 flooredAxisAngles) const;

	};

}

#endif
