#ifndef GRAPHICS_EARTHMODEL_H
#define GRAPHICS_EARTHMODEL_H
#pragma once

#include "Physics/External/Earth.h"
#include "Graphics/Simulation_model/Cameras/AllCameras.h"

#include <PhysicsFramework/CoordTransform3D.h>
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
		//This angle represents the angle between chunk boundary points on the surface. The smaller the angle, the more chunks are wrapped round the sphere.
		//Must be a factor of 360.0 degrees.
		static constexpr double mChunkAngle_degs = 360.0 / mSegments;  
		
		//The radius of the (test) sphere in metres
		static constexpr double mRadius = 6378137.0; //6378137.0

		//Reference axes
		static constexpr glm::dvec3 
			mHorizontalRefAxis = { 1.0, 0.0, 0.0 },
			mVerticalRefAxis = { 0.0, 1.0, 0.0 }; 

		//The camera's angle around the sphere is continuous, but chunks discretize the surface.
		//This variable stores the nearest bottom-left chunk vertex to the camera's current angle.
		glm::dvec2 mLastModuloAxisAngles = { 0.0, 0.0 };

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

		void render(const SimulationCamera& activeSimCam, const CoordTransform3D& eunToEcef);

	private:
		void loadResources();
		void updateMeshStructure(glm::dvec3 camPosition_EUN, const CoordTransform3D& eunToEcef);
		void updateTransforms_OGL(glm::dvec3 cameraPosition_EUN);
		glm::dvec3 getChunkPosition_ecef(glm::dvec2 flooredAxisAngles) const;

	};

}

#endif
