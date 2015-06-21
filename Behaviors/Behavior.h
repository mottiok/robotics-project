/*
 * Behavior.h
 *
 *  Created on: May 29, 2015
 *      Author: colman
 */

#ifndef BEHAVIOR_H_
#define BEHAVIOR_H_

#include <math.h>
#include "../Robot.h"
#include "../WaypointManager.h"
#include <vector>
#include "../Convert.h"

#define MAX_STRAIGHT_LINE_ERROR (M_PI/12) // 7.5 degrees either side

enum eBehavior
{
	eBehavior_GoForward,
	eBehavior_TurnRight,
	eBehavior_TurnLeft,
	eBehavior_Count
};

using namespace std;

class Behavior {
	vector<Behavior*> _behaviors;
protected:
	Robot* _robot;
	WaypointManager* _waypoints;
	bool  _evasiveManeuvers;
	eBehavior _behaviorType;
	
	double _gridResolution;
	double _pixelResolution;
	double _mapWidth;
	double _mapHeight;

	float CalcCurrWaypointAngleOffset(Waypoint* pTarget)
	{
		SPosition sCurrPos;
		sCurrPos.dwX = Convert::RobotRelativeXPosToPixelXCoord(_robot->GetX(), CM_TO_METERS(_pixelResolution), _mapWidth);
		sCurrPos.dwY = Convert::RobotRelativeYPosToPixelYCoord(_robot->GetY(), CM_TO_METERS(_pixelResolution), _mapHeight);

		sCurrPos.dwX = (sCurrPos.dwX / (_gridResolution / _pixelResolution));
		sCurrPos.dwY = (sCurrPos.dwY / (_gridResolution / _pixelResolution));
		float fGradientError = (SPosition::CalcGradientAngleOffset(sCurrPos, pTarget->GetPosition(), _robot->GetYaw()));

		//printf("Robot claims it's at (%u, %u) with bearing %f headed for (%u, %u), off by %f\n", sCurrPos.dwX, sCurrPos.dwY, _robot->GetYaw(), pTarget->GetPosition().dwX, pTarget->GetPosition().dwY, fGradientError);

		return fGradientError;
	}
public:
	Behavior(Robot* robot, WaypointManager* waypoints, double pixelResolution, double gridResolution, double mapWidth, double mapHeight, eBehavior behavior) : _robot(robot), _waypoints(waypoints), _pixelResolution(pixelResolution), _gridResolution(gridResolution), _mapWidth(mapWidth), _mapHeight(mapHeight), _evasiveManeuvers(false), _behaviorType(behavior) {};
						
	virtual bool StartCondition(eBehavior eSRCBehavior) = 0;
	virtual bool StopCondition() = 0;
	virtual void Action()
	{
		SPosition sCurrPos;
		sCurrPos.dwX = Convert::RobotRelativeXPosToPixelXCoord(_robot->GetX(), CM_TO_METERS(_pixelResolution), _mapWidth);
		sCurrPos.dwY = Convert::RobotRelativeYPosToPixelYCoord(_robot->GetY(), CM_TO_METERS(_pixelResolution), _mapHeight);

		sCurrPos.dwX = (sCurrPos.dwX / (_gridResolution / _pixelResolution));
		sCurrPos.dwY = (sCurrPos.dwY / (_gridResolution / _pixelResolution));

	//	printf("Time to traverse waypoint? Is %f less than %u?\n", 
	//	SPosition::CalcDistance(sCurrPos, _waypoints->CurrentWaypoint()->GetPosition()),
	//		WAYPOINT_RADIUS);

		if (SPosition::CalcDistance(sCurrPos, _waypoints->CurrentWaypoint()->GetPosition()) < WAYPOINT_RADIUS)
		{
			_waypoints->TraverseWaypoint();
			_evasiveManeuvers = false;
		}
	}

	bool IsEvasive() { return _evasiveManeuvers; };
	bool SetEvasive(bool fIsEvasive) { _evasiveManeuvers = fIsEvasive; };
	eBehavior GetBehaviorType() { return _behaviorType; };

	void AddBehavior(Behavior* nextBehavior) {
		_behaviors.push_back(nextBehavior);
	}

	Behavior* NextBehavior() {
		for(size_t i = 0; i < _behaviors.size(); i++) {
			_behaviors[i]->SetEvasive(IsEvasive());
			if (_behaviors[i]->StartCondition(_behaviorType)) {
				return _behaviors[i];
			}

			if (IsEvasive() != _behaviors[i]->IsEvasive()){
				SetEvasive(_behaviors[i]->IsEvasive());

				if (NULL == NextBehavior())
					return this;
				else
					return NextBehavior();
			}
		}

		return NULL;
	}

	virtual ~Behavior();
};

#endif /* BEHAVIOR_H_ */
