#include "SurfaceLocation.h"

namespace Physics {
	namespace External {
 
		SurfaceLocation::SurfaceLocation(const std::string& name, const glm::dvec3& gpsCoord_LLH) :
			mName(name),
			mGPSCoordinate_LLH(gpsCoord_LLH)
		{
			updateENU_to_ECEFTransform();
		}

		void SurfaceLocation::setGPSPosition(const glm::dvec3& gpsCoord_LLH) {
			mGPSCoordinate_LLH = gpsCoord_LLH;
			updateENU_to_ECEFTransform();
		}

		void SurfaceLocation::updateENU_to_ECEFTransform() 
			//Called whenever the GPS position changes
		{
			using namespace glm;

			mEUN_to_ECEF.setLocalToParent_position(::External::GeoCoordUtils::toECEF(mGPSCoordinate_LLH));

			double latitudeRotation_degs = ::External::Earth::geocentricToGeodeticLat(mGPSCoordinate_LLH.x) - 90.0;
			
			dmat4 
				eunAlignmentRot = rotate(radians(-90.0), dvec3(0.0, 1.0, 0.0)),
				latitudeRot = rotate(radians(latitudeRotation_degs), dvec3(0.0, 0.0, 1.0)),
				longitudeRot = rotate(radians(-mGPSCoordinate_LLH.y), dvec3(0.0, 1.0, 0.0));

			mEUN_to_ECEF.setLocalToParent_rotation(longitudeRot * latitudeRot * eunAlignmentRot);
		}

	}
}