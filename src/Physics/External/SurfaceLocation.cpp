#include "SurfaceLocation.h"

namespace Physics {
	namespace External {
 
		SurfaceLocation::SurfaceLocation(const glm::dvec3& gpsCoord_LLH, const std::string& name) :
			mGPSCoordinate_LLH(gpsCoord_LLH),
			mName(name)
		{
			using namespace glm;
			
			mEUNtoECEF.setLocalToParent_position(::External::GeoCoordUtils::toECEF(gpsCoord_LLH));

			double latitudeRotation_degs = ::External::Earth::geocentricToGeodeticLat(gpsCoord_LLH.x) - 90.0;
			
			dmat4 
				eunAlignmentRot = rotate(dmat4(1.0f), radians(-90.0), dvec3(0.0, 1.0, 0.0)),
				latitudeRot = rotate(dmat4(1.0f), radians(latitudeRotation_degs), dvec3(0.0, 0.0, 1.0)),
				longitudeRot = rotate(dmat4(1.0f), radians(-gpsCoord_LLH.y), dvec3(0.0, 1.0, 0.0));

			mEUNtoECEF.setLocalToParent_rotation(longitudeRot * latitudeRot * eunAlignmentRot);
		}

	}
}