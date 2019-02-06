#include "SurfaceLocation.h"
#include "GeoCoordUtils.h"

namespace Physics {
	namespace External {
 
		SurfaceLocation::SurfaceLocation(const std::string& name, const chrono::Vector& gpsCoord_LLH) :
			mName(name),
			mGPSCoordinate_LLH(gpsCoord_LLH)
		{
			updateENU_to_ECEFTransform();
		}

		void SurfaceLocation::operator=(const SurfaceLocation& other) {
			this->mEUN_to_ECEF = other.mEUN_to_ECEF;
			this->mGPSCoordinate_LLH = other.mGPSCoordinate_LLH;
			this->mName = other.mName;
		}

		void SurfaceLocation::setGPSPosition(const chrono::Vector& gpsCoord_LLH) {
			mGPSCoordinate_LLH = gpsCoord_LLH;
			updateENU_to_ECEFTransform();
		}

		void SurfaceLocation::updateENU_to_ECEFTransform() 
			// Called whenever the GPS position changes
		{
			// TODO: Update this to use chrono::Vectors and chrono::ChFrames rather than glm::dvec3s and GF::CoordTransform3Ds
			//----------------------------------------------------------------------------------------------------------------

			using namespace glm;

			mEUN_to_ECEF.setLocalToParent_translation(::External::GeoCoordUtils::toECEF(mGPSCoordinate_LLH));

			double latitudeRotation_degs = ::External::Earth::geocentricToGeodeticLat(mGPSCoordinate_LLH.x) - 90.0;
			
			dmat4 
				eunAlignmentRot = rotate(radians(-90.0), dvec3(0.0, 1.0, 0.0)),
				latitudeRot = rotate(radians(latitudeRotation_degs), dvec3(0.0, 0.0, 1.0)),
				longitudeRot = rotate(radians(-mGPSCoordinate_LLH.y), dvec3(0.0, 1.0, 0.0));

			mEUN_to_ECEF.setLocalToParent_rotation(longitudeRot * latitudeRot * eunAlignmentRot);
		}

	}
}