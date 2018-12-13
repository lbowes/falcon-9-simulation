#ifndef PHYSICS_HARDWARE_TVCACTUATOR_H
#define PHYSICS_HARDWARE_TVCACTUATOR_H
#pragma once

#include <cmath>
#include <glm/vec2.hpp>
#include <glm/geometric.hpp>
#include <glm/trigonometric.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>

namespace Physics {
	namespace Hardware {

		class TVCActuator {
			friend class Engine;
		private:
			const glm::dvec2
				mStagePoint_engine,          //local to engine
				mNeutralEnginePoint_engine;  //local to engine
			
			const double 
				mClockingDegree = 0.0,       //degrees
				mMaxEngineGimbalAngle = 0.0, //degrees
				mCosConstant_1 = 0.0,        //magnitude(mStagePoint)^2 + magnitude(mEnginePoint)^2
				mCosConstant_2 = 0.0,        //2 * magnitude(mStagePoint) * magnitude(mEnginePoint)  
				mMinLength = 0.0,            //m
				mMaxLength = 0.0,            //m
				mStagePointAngle = 0.0,		 //angle between mStagePoint and vec3(0, -1, 0) in degrees
				mEnginePointAngle = 0.0;	 //angle between mNeutralEnginePoint and vec3(0, -1, 0) in degrees

			glm::dvec2 mEnginePoint_engine;  //local to engine
											       
			double							       
				mResultantAngle = 0.0,       //degrees
				mPercentExtension = 0.0,	       
				mCurrentLength = 0.0;        //m

		public:
			TVCActuator(double maxEngineGimbalAngle_degs, glm::dvec2 stagePoint_engine, glm::dvec2 enginePoint_engine, double clockingDegree_degs);
			~TVCActuator() = default;
		
			void setPercentExtension(double newPercentExtension);
			
			double getClockingDegree() const { return mClockingDegree; }
			glm::dvec2 getStagePoint_engine() const { return mStagePoint_engine; }
			glm::dvec2 getNeutralEnginePoint_engine() const { return mNeutralEnginePoint_engine; }
			glm::dvec2 getEnginePoint_engine() const { return mEnginePoint_engine; }

		private:
			void updateParams();

		};

	}
}

#endif
