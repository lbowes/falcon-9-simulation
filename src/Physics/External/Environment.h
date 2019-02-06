#ifndef PHYSICS_EXTERNAL_ENVIRONMENT_H
#define PHYSICS_EXTERNAL_ENVIRONMENT_H
#pragma once

#include <chrono/core/ChVector.h>

#define WIND_ACTIVE 0

namespace Physics {
	namespace External {
		
		class Environment {
		public:
			static constexpr double
				G =                    6.67408e-11,
				seaLevelGravity =      9.80665,		
				universalGasConstant = 8314.4598, 
				airMolarMass =         0.0289644,         
				tempLapseRate =        0.0065,			
				seaLevelStdPressure =  101325.0,  
				seaLevelStdTemp =      288.15,      
				seaLevelAirDensity =   1.225,		
				specGasConstant =      287.053,	  
				degreesCtoKDiff =      273.15,
				sutherlandTemp =       110.4;
	
			static chrono::Vector windVelocity_m_per_s;
	
		public:
			static void update(double t);
	
			static double toGeopotential_m(int geometricAltitude_m);
			static double toGeometric_m(int geoPotentialAltitude_m);
			static double getAirPressure_Pa(int altitude_m);
			static double getAirDensity_kg_per_m3(int altitude_m);
			static double getGravity(int altitude_m);
			static double getKineticTemperature_K(int altitude_m);
			static double getKineticTemperature_degsC(int altitude_m);
			static double getDynamicViscosity_cP(int altitude_m);
			static double getKinematicViscosity_St(int altitude_m);
	
		private:
			Environment() = delete;
	
		};
	
	}
}

#endif
									  