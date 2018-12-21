#ifndef GRAPHICS_ISTAGECOMPONENTMESH_H
#define GRAPHICS_ISTAGECOMPONENTMESH_H
#pragma once

#include <GraphicsFramework/ResourceSet.h>
#include <GraphicsFramework/Model3D.h>

namespace Graphics {
	
	class IStageComponentMesh {
	protected:
		GF::ResourceSet& mResourceBucket;
		
		GF::Model3D& mParentModel;

	public:
		IStageComponentMesh(GF::ResourceSet& resourceBucket, GF::Model3D& parentModel) :
			mResourceBucket(resourceBucket),
			mParentModel(parentModel)
		{ }

		virtual ~IStageComponentMesh() = default;

		virtual void loadResources() = 0;
		
		//This is responsible for updating any Meshes that were added to this component model during the loading period.
		//This may include updating the vertex buffer data within the mesh (ExhaustJet, TVCActuator, ...) or updating the
		//OGL transform of the mesh(es). The transform updates can be calculated with the totalStageTransform_OGL.
		virtual void updateResources(glm::mat4 stageModelTransform_OGL) = 0;

	};

}

#endif
