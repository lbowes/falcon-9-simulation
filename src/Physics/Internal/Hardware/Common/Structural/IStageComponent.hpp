#ifndef PHYSICS_HARDWARE_ISTAGECOMPONENT_HPP
#define PHYSICS_HARDWARE_ISTAGECOMPONENT_HPP
#pragma once

#include <core/ChFrame.h>
#include <physics/ChBodyAuxRef.h>
#include <physics/ChSystem.h>

namespace chrono {
	class ChBodyAuxRef;
}

namespace Physics {
	namespace Hardware {

		class IStageComponent {
		protected:
			std::shared_ptr<chrono::ChBodyAuxRef>& mStageBodyHandle;
            std::shared_ptr<chrono::ChBodyAuxRef> mBody;

		public:
			IStageComponent(std::shared_ptr<chrono::ChBodyAuxRef>& stageBody) :
                mStageBodyHandle(stageBody),
                mBody(std::make_shared<chrono::ChBodyAuxRef>())
			{ 
                stageBody->GetSystem()->AddBody(mBody);
            }

			virtual ~IStageComponent() = default;
			
			const chrono::ChFrame<>& getFrame_world() const { return mBody->GetFrame_REF_to_abs(); }

		protected:
			// Initialises mass, centre of mass transform, inertia of mBody
			virtual void assemble() = 0;

			//  Attaches the component to the stage in some way
			virtual void attachToStage() = 0;

		};

	}
}

#endif // PHYSICS_HARDWARE_ISTAGECOMPONENT_HPP
