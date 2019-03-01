#ifndef PHYSICS_HARDWARE_ISTAGECOMPONENT_HPP
#define PHYSICS_HARDWARE_ISTAGECOMPONENT_HPP

#include <core/ChFrame.h>
#include <collision/ChCCollisionModel.h>
#include <chrono/physics/ChSystemNSC.h>

namespace chrono {
	class ChBodyAuxRef;
}

namespace Physics {
	namespace Hardware {

	    // A StageComponent is any component that cannot be detached from an IStage
		// It may move while attached to the stage, but the two cannot be separated
		class IStageComponent {
		protected:
			chrono::ChSystemNSC& mSystemHandle;
			std::shared_ptr<chrono::ChBodyAuxRef>& mStageBodyHandle;
			chrono::ChFrame<> mComp_to_stage;
			std::shared_ptr<chrono::ChBodyAuxRef> mBody;

		public:
			IStageComponent(chrono::ChSystemNSC& sys, std::shared_ptr<chrono::ChBodyAuxRef>& stageBody, chrono::ChFrame<> comp_to_stage = chrono::ChFrame<>()) :
				mSystemHandle(sys),
				mStageBodyHandle(stageBody),
				mComp_to_stage(comp_to_stage),
				mBody(std::make_shared<chrono::ChBodyAuxRef>())
			{ 
				sys.AddBody(mBody);
 
				mBody->GetCollisionModel()->SetFamily(3);
				mBody->GetCollisionModel()->SetFamilyMaskNoCollisionWithFamily(2);
			}

			virtual ~IStageComponent() = default;
			
			const chrono::ChFrame<>& getFrame_world() const { return mBody->GetFrame_REF_to_abs(); }

		protected:
			// Fully initialises mBody (mass, inertia, collision)
			virtual void assemble() = 0;

			// Uses links/joints/any elaborate setups to attach mBody to the stage
			virtual void attachToStage() = 0;

		};

	}
}

#endif
