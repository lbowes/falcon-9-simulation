#include "TVCActuator.h"

namespace Physics {
	namespace Hardware {

		TVCActuator::TVCActuator(double maxEngineGimbalAngle_degs, glm::dvec2 stagePoint_engine, glm::dvec2 enginePoint_engine, double clockingDegree_degs) :
			mStagePoint_engine(stagePoint_engine),
			mNeutralEnginePoint_engine(enginePoint_engine),
			mClockingDegree(clockingDegree_degs),
			mMaxEngineGimbalAngle(maxEngineGimbalAngle_degs),
			mEnginePoint_engine(enginePoint_engine),
			mCosConstant_1(pow(length(mStagePoint_engine), 2.0) + pow(length(mEnginePoint_engine), 2.0)),
			mCosConstant_2(2 * length(mStagePoint_engine) * length(mEnginePoint_engine)),
			mStagePointAngle(abs(glm::degrees(atan(mStagePoint_engine.x / mStagePoint_engine.y)))),
			mEnginePointAngle(abs(glm::degrees(atan(mEnginePoint_engine.x / mEnginePoint_engine.y)))),
			mMinLength(sqrt(mCosConstant_1 - mCosConstant_2 * cos(glm::radians(mStagePointAngle - mEnginePointAngle - mMaxEngineGimbalAngle)))),
			mMaxLength(sqrt(mCosConstant_1 - mCosConstant_2 * cos(glm::radians(mStagePointAngle - mEnginePointAngle + mMaxEngineGimbalAngle))))
		{ }

		void TVCActuator::setPercentExtension(double newPercentExtension) {
			if (newPercentExtension >= 0.0 && newPercentExtension <= 1.0) {
				mPercentExtension = newPercentExtension;
				updateParams();
			}
		}

		void TVCActuator::updateParams() {
			using namespace glm;
			
			mCurrentLength = mMinLength + (mMaxLength - mMinLength) * mPercentExtension;
			mResultantAngle = mStagePointAngle - degrees(acos((mCosConstant_1 - pow(mCurrentLength, 2.0)) / mCosConstant_2)) - mEnginePointAngle;
			mEnginePoint_engine = rotate(mNeutralEnginePoint_engine, -radians(mResultantAngle));
		}

	}
}