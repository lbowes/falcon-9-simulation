#ifndef PHYSICS_HARDWARE_ISTAGECOMPONENT_HPP
#define PHYSICS_HARDWARE_ISTAGECOMPONENT_HPP

#include <core/ChFrame.h>
#include <physics/ChInertiaUtils.h>

namespace Physics {
	namespace Hardware {

	    // A StageComponent is any component that cannot be detached from an IStage
		class IStageComponent {
		public:
			IStageComponent() = default;
			virtual ~IStageComponent() = default;

			chrono::ChMatrix33<> getInertia_stage() const {
				chrono::ChFrame compToStage = calcTransform_toStage();
				chrono::ChMatrix33<> result = calcInertia_comp();
				chrono::ChInertiaUtils::RotateInertia(result, compToStage.GetA(), result);
				chrono::ChInertiaUtils::TranslateInertia(result, compToStage.GetPos(), calcMass(), result);
				return result;
			}

			double getMass() const {
				return calcMass();
			}

			chrono::ChVector<> getCoM_stage() const {
				return calcTransform_toStage() * calcCoM_comp();
			}

		protected:
			virtual chrono::ChFrame<> calcTransform_toStage() const = 0;
			virtual chrono::ChMatrix33<> calcInertia_comp() const = 0;
			virtual double calcMass() const = 0;
			virtual chrono::ChVector<> calcCoM_comp() const = 0;

		};

	}
}

#endif
