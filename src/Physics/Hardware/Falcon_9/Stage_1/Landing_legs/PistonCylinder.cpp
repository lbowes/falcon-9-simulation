#include "PistonCylinder.h"
#include "TelescopingPiston.h"

namespace Physics {
	namespace Hardware {

		PistonCylinder::PistonCylinder(const TelescopingPiston& parent, double widthScale, unsigned char numberInPiston) :
			mParent(parent),
			mNumberInPiston(numberInPiston),
			mWidthScale(widthScale)
		{ }

	}
}