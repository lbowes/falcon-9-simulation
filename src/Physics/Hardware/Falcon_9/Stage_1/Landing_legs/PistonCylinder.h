#ifndef PHYSICS_HARDWARE_PISTONCYLIDNER_H
#define PHYSICS_HARDWARE_PISTONCYLIDNER_H
#pragma once

#include <glm/mat4x4.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>

namespace Physics {
	namespace Hardware {
		
		class TelescopingPiston;
		
		class PistonCylinder {
		private:
			static constexpr double mLength = 2.85;       //m, 2.85
			const TelescopingPiston& mParent;
			const unsigned char mNumberInPiston = 0; //0 is connected to the stage mount point, 1 is next along etc
			double mWidthScale = 0.0;
			
		public:
			PistonCylinder(const TelescopingPiston& parent, double widthScale, unsigned char numberInPiston);
			~PistonCylinder() = default;

			double getLength() const { return mLength; }
			double getWidthScale() const { return mWidthScale; }

		};

	}
}

#endif
