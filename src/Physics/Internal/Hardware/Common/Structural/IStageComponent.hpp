#ifndef PHYSICS_HARDWARE_ISTAGECOMPONENT_HPP
#define PHYSICS_HARDWARE_ISTAGECOMPONENT_HPP

#include <core/ChFrame.h>
#include <collision/ChCCollisionModel.h>

namespace Physics {
	namespace Hardware {

	    // A StageComponent is any component that cannot be detached from an IStage
		// It may move while attached to the stage, but the two cannot be separated
		class IStageComponent {
		protected:
			chrono::ChSystemNSC& mSystemHandle;
			chrono::ChBodyAuxRef& mStageBody;
			std::shared_ptr<chrono::ChBodyAuxRef> mBody;

		public:
			IStageComponent(chrono::ChSystemNSC& sys, chrono::ChBodyAuxRef& stageBody, chrono::ChFrame<> comp_to_stage = chrono::ChFrame<>()) :
				mSystemHandle(sys),
				mBody(std::make_shared<chrono::ChBodyAuxRef>())
			{ 
				sys.AddBody(mBody);
			}

			virtual ~IStageComponent() = default;
			
		protected:
			void assemble() {
				mBody->SetMass(calcMass());
				mBody->SetInertia(calcInertia_comp());
				mBody->SetFrame_COG_to_REF(calcCoM_comp());
				initCollision();
				mBody->SetCollisionModel(calcCollisionModel_comp());
			}

			virtual chrono::ChFrame<> calcTransform_toStage() const = 0;
			virtual chrono::ChMatrix33<> calcInertia_comp() const = 0;
			virtual double calcMass() const = 0;
			virtual chrono::ChFrame<> calcCoM_comp() const = 0;
			virtual void initCollision() = 0;
			virtual chrono::ChCollisionModel calcCollisionModel_comp() const = 0;
			
			// Uses links and joints to attach the component to the stage
			virtual void attachToStage() = 0;

		};

	}
}

#endif
