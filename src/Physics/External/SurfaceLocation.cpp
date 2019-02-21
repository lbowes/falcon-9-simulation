#include "SurfaceLocation.h"
#include "GeoCoordUtils.h"
#include "Earth.h"

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
			mEUN_to_ECEF.SetPos(::External::GeoCoordUtils::toECEF(mGPSCoordinate_LLH));

			double latitudeRotation_degs = Earth::geocentricToGeodeticLat(mGPSCoordinate_LLH.x) - 90.0;
			
			//dmat4 
				//eunAlignmentRot = rotate(radians(-90.0), dvec3(0.0, 1.0, 0.0)),
				//latitudeRot = rotate(radians(latitudeRotation_degs), dvec3(0.0, 0.0, 1.0)),
				//longitudeRot = rotate(radians(-mGPSCoordinate_LLH.y), dvec3(0.0, 1.0, 0.0));
			chrono::Quaternion 
				eunAlignmentRot = chrono::Q_from_AngAxis(90.0 * chrono::CH_C_DEG_TO_RAD, chrono::VECT_Y),
				latitudeRot = chrono::Q_from_AngAxis(latitudeRotation_degs * chrono::CH_C_DEG_TO_RAD, chrono::VECT_Z),
				longitudeRot = chrono::Q_from_AngAxis(-mGPSCoordinate_LLH.y * chrono::CH_C_DEG_TO_RAD, chrono::VECT_Y);

			//mEUN_to_ECEF.setLocalToParent_rotation(longitudeRot * latitudeRot * eunAlignmentRot);
			mEUN_to_ECEF.SetRot(longitudeRot * latitudeRot * eunAlignmentRot);
		}

	}
}