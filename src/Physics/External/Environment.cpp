#include "Environment.h"
#include "Earth.h"

#include <cmath>

namespace Physics {
	namespace External {

#if WIND_ACTIVE							  
		chrono::Vector Environment::windVelocity_m_per_s = chrono::Vector(6.253, 0.0, 0.0); // a constant 14mph wind due east
#else
		chrono::Vector Environment::windVelocity_m_per_s = chrono::Vector(0.0);
#endif

		void Environment::update(double t) {
			// Updates the wind direction and speed pseudo-randomly

		}

		double Environment::toGeopotential_m(int geometricAltitude_m) {
			return (Earth::WGS84::a * geometricAltitude_m) / (Earth::WGS84::a + geometricAltitude_m);
		}

		double Environment::toGeometric_m(int geopotentialAltitude_m) {
			return (Earth::WGS84::a * geopotentialAltitude_m) / (Earth::WGS84::a - geopotentialAltitude_m);
		}

		double Environment::getAirPressure_Pa(int altitude_m) 
			// Implementation of U.S Standard Atmosphere
			// http://www.braeunig.us/space/atmmodel.htm
		{
			if (altitude_m <= 0)
				return seaLevelStdPressure;
			if (altitude_m > 1'000'000.0)
				return 0.00000001; // Almost no air pressure at vacuum

			double 
				geoPot_Km = toGeopotential_m(altitude_m) / 1000.0,
				geometric_Km = altitude_m / 1000.0;

			// Using geopotential height value here
			if (geoPot_Km <= 11.0)
				return 101325.0 * pow((288.15 / (288.15 - 6.5 * geoPot_Km)), (34.1632 / -6.5));
			else if (geoPot_Km <= 20.0)
				return 22632.06 * exp(-34.1632 * (geoPot_Km - 11.0) / 216.65);
			else if (geoPot_Km <= 32.0)
				return 5474.889 * pow((216.65 / (216.65 + (geoPot_Km - 20.0))), 34.1632);
			else if (geoPot_Km <= 47.0)
				return 868.0187 * pow((228.65 / (228.65 + 2.8 * (geoPot_Km - 32.0))), (34.1632 / 2.8));
			else if (geoPot_Km <= 51.0)
				return 110.9063 * exp(-34.1632 * (geoPot_Km - 47.0) / 270.65);
			else if (geoPot_Km <= 71.0)
				return 66.93887 * pow((270.65 / (270.65 - 2.8 * (geoPot_Km - 51.0))), (34.1632 / -2.8));
			else if (geoPot_Km <= 84.852)
				return 3.956420 * pow((214.65 / (214.65 - 2.0 * (geoPot_Km - 71.0))), (34.1632 / -2.0));
			// Using geometric height value from now on
			else if (geometric_Km <= 1000.0) {
				double 
					A = 0.0,
					B = 0.0,
					C = 0.0,
					D = 0.0,
					E = 0.0;

				if (geometric_Km <= 91.0) {       A = 0.0;           B = 2.159582e-06;  C = -4.836957e-04; D = -0.1425192;  E = 13.47530; }
				else if (geometric_Km <= 100.0) { A = 0.0;           B = 3.304895e-05;  C = -0.009062730;  D = 0.6516698;   E = -11.03037; }
				else if (geometric_Km <= 110.0) { A = 0.0;           B = 6.693926e-05;  C = -0.01945388;   D = 1.719080;    E = -47.75030; }
				else if (geometric_Km <= 120.0) { A = 0.0;           B = -6.539316e-05; C = 0.02485568;    D = -3.223620;   E = 135.9355; }
				else if (geometric_Km <= 150.0) { A = 2.283506e-07;  B = -1.343221e-04; C = 0.02999016;    D = -3.055446;   E = 113.5764; }
				else if (geometric_Km <= 200.0) { A = 1.209434e-08;  B = -9.692458e-06; C = 0.003002041;   D = -0.4523015;  E = 19.19151; }
				else if (geometric_Km <= 300.0) { A = 8.113942e-10;  B = -9.822568e-07; C = 4.687616e-04;  D = -0.1231710;  E = 3.067409; }
				else if (geometric_Km <= 500.0) { A = 9.814674e-11;  B = -1.654439e-07; C = 1.148115e-04;  D = -0.05431334; E = -2.011365; }
				else if (geometric_Km <= 750.0) { A = -7.835161e-11; B = 1.964589e-07;  C = -1.657213e-04; D = 0.04305869;  E = -14.77132; }
				else if (geometric_Km <= 1000.0) { A = 2.813255e-11; B = -1.120689e-07; C = 1.695568e-04;  D = -0.1188941;  E = 14.56718; }

				return exp(A * pow(geometric_Km, 4.0) + B * pow(geometric_Km, 3.0) + C * pow(geometric_Km, 2.0) + D * geometric_Km + E);
			}
			else
				return 0.0;
		}

		double Environment::getAirDensity_kg_per_m3(int altitude_m) 
			// Implementation of U.S Standard Atmosphere
			// http://www.braeunig.us/space/atmmodel.htm
		{
			if (altitude_m <= 0)
				return seaLevelAirDensity;

			double
				geoPotKm = toGeopotential_m(altitude_m) / 1000.0,
				geometricKm = altitude_m / 1000.0,
				temperatureK = getKineticTemperature_K(altitude_m);

			if (geoPotKm <= 84.852) 
				return getAirPressure_Pa(altitude_m) / (specGasConstant * temperatureK);
			else if (geometricKm <= 1000.0) {
				double 
					A = 0.0, 
					B = 0.0, 
					C = 0.0, 
					D = 0.0, 
					E = 0.0;

				if (geometricKm <= 91.0)        { A = 0.0;           B = -3.322622e-06; C = 9.111460e-04;  D = -0.2609971;  E = 5.944694;  }
				else if (geometricKm <= 100.0)  { A = 0.0;           B = 2.873405e-05;  C = -0.008492037;  D = 0.6541179;   E = -23.62010; }
				else if (geometricKm <= 110.0)  { A = -1.240774e-05; B = 0.005162063;   C = -0.8048342;    D = 55.55996;    E = -1443.338; }
				else if (geometricKm <= 120.0)  { A = 0.0;           B = -8.854164e-05; C = 0.03373254;    D = -4.390837;   E = 176.5294;  }
				else if (geometricKm <= 150.0)  { A = 3.661771e-07;  B = -2.154344e-04; C = 0.04809214;    D = -4.884744;   E = 172.3597;  }
				else if (geometricKm <= 200.0)  { A = 1.906032e-08;  B = -1.527799e-05; C = 0.004724294;   D = -0.6992340;  E = 20.50921;  }
				else if (geometricKm <= 300.0)  { A = 1.199282e-09;  B = -1.451051e-06; C = 6.910474e-04;  D = -0.1736220;  E = -5.321644; }
				else if (geometricKm <= 500.0)  { A = 1.140564e-10;  B = -2.130756e-07; C = 1.570762e-04;  D = -0.07029296; E = -12.89844; }
				else if (geometricKm <= 750.0)  { A = 8.105631e-12;  B = -2.358417e-09; C = -2.635110e-06; D = -0.01562608; E = -20.02246; }
				else if (geometricKm <= 1000.0) { A = -3.701195e-12; B = -8.608611e-09; C = 5.118829e-05;  D = -0.06600998; E = -6.137674; }

				return exp(A * pow(geometricKm, 4.0) + B * pow(geometricKm, 3.0) + C * pow(geometricKm, 2.0) + D * geometricKm + E);
			}
			else 
				return 0.0;
		}

		double Environment::getGravity(int altitude_m) {
			if (altitude_m <= 0)
				return seaLevelGravity;

			return seaLevelGravity * pow(Earth::WGS84::a / (Earth::WGS84::a + altitude_m), 2.0);
		}

		double Environment::getKineticTemperature_K(int altitude_m) 
			// Implementation of U.S Standard Atmosphere
			// http://www.braeunig.us/space/atmmodel.htm
		{
			if (altitude_m <= 0)
				return seaLevelStdTemp;

			double
				geoPot_Km = toGeopotential_m(altitude_m) / 1000.0,
				geometric_Km = altitude_m / 1000.0;

			// Using geopotential height value here
			if (geoPot_Km <= 11.0)        
				return 288.15 - 6.5 * geoPot_Km;
			else if (geoPot_Km <= 20.0)   
				return 216.65;
			else if (geoPot_Km <= 32.0)   
				return 196.65 + geoPot_Km;
			else if (geoPot_Km <= 47.0)   
				return 139.05 + 2.8 * geoPot_Km;
			else if (geoPot_Km <= 51.0)   
				return 270.65;
			else if (geoPot_Km <= 71.0)   
				return 413.45 - 2.8 * geoPot_Km;
			else if (geoPot_Km <= 84.852) 
				return 356.65 - 2.0 * geoPot_Km;
			// Using geometric height value from now on
			else if (geometric_Km <= 91.0)   
				return 186.8673;
			else if (geometric_Km <= 110.0)  
				return 263.1905 - 76.3232 * sqrt(1.0 - pow((geometric_Km - 91.0) / -19.9429, 2.0));
			else if (geometric_Km <= 120.0)  
				return 240.0 + 12.0 * (geometric_Km - 110.0);
			else if (geometric_Km <= 1000.0) 
				return 1000.0 - 640.0 * exp(-0.01875 * ((geometric_Km - 120.0) * (6356.766 + 120.0) / (6356.766 + geometric_Km)));
			else 
				return 1000.0;
		}

		double Environment::getKineticTemperature_degsC(int altitude_m) {
			return getKineticTemperature_K(altitude_m) - degreesCtoKDiff;
		}

		double Environment::getDynamicViscosity_cP(int altitude_m) {
			double
				T = getKineticTemperature_K(altitude_m),
				T0 = 273.15,
				u0 = 0.00001716;

			return u0 * pow(T / T0, 1.5) * ((T0 + sutherlandTemp) / (T + sutherlandTemp));
		}

		double Environment::getKinematicViscosity_St(int altitude_m) {
			return getDynamicViscosity_cP(altitude_m) / getAirDensity_kg_per_m3(altitude_m);
		}

	}
}