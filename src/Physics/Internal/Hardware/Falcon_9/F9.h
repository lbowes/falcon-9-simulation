#ifndef PHYSICS_HARDWARE_F9_H
#define PHYSICS_HARDWARE_F9_H
#pragma once

#include "../Common/Structural/ILaunchVehicle.h"
#include "Stage_1/F9S1.h"
#include "Stage_2/F9S2.h"

namespace chrono {
	class ChSystemNSC;
}

namespace Physics {
	class F9_DSS;
	
	namespace Hardware {

		class F9 : public ILaunchVehicle {
			friend class Physics::F9_DSS;
		private:
			F9S1 mStage1;
			F9S2 mStage2;

		public:
			F9(chrono::ChSystemNSC& sys);
			~F9() = default;

			void assemble() override;

		};

	}

	class F9_DSS {
	private:
		F9S1_DSS mF9S1_dss;
		//F9S2_DSS mF9S2_dss;

	public:
		F9_DSS() = default;
		void operator=(const F9_DSS& other);
		F9_DSS(const Hardware::F9& f9);
		~F9_DSS() = default;

		static F9_DSS lerp(const F9_DSS& a, const F9_DSS& b, float x);

		const F9S1_DSS& getF9S1_DSS() const { return mF9S1_dss; }

	};

}

#endif
