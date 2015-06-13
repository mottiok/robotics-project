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

#define MAX_STRAIGHT_LINE_ERROR (M_PI/12) // 15 degrees either side

using namespace std;

class Behavior {
	vector<Behavior*> _behaviors;
protected:
	Robot* _robot;
	WaypointManager* _waypoints;
	float _gridResolution;

	float CalcCurrWaypointAngleOffset(Waypoint* pTarget)
	{
		SPosition sCurrPos;
		sCurrPos.dwX = floor(_robot->GetX() / (_gridResolution / 100));
		sCurrPos.dwY = floor(_robot->GetY() / (_gridResolution / 100));

		float fGradientError = (SPosition::CalcGradientAngleOffset(sCurrPos, pTarget->GetPosition(), _robot->GetYaw()));

		printf("Robo claims it's at (%u, %u) with bearing %f headed for (%u, %u), off by %f\n", sCurrPos.dwX, sCurrPos.dwY, _robot->GetYaw(), pTarget->GetPosition().dwX, pTarget->GetPosition().dwY, fGradientError);

		return fGradientError;
	}
public:
	Behavior(Robot* robot, WaypointManager* waypoints, float fGridResolution) : _robot(robot), _waypoints(waypoints), _gridResolution(fGridResolution) {};
	virtual bool StartCondition() = 0;
	virtual bool StopCondition() = 0;
	virtual void Action()
	{
		SPosition sCurrPos;
		sCurrPos.dwX = floor(_robot->GetX() / (_gridResolution / 100));
		sCurrPos.dwY = floor(_robot->GetY() / (_gridResolution / 100));

		printf("Time to traverse waypoint? Is %f less than %u?\n", 
			SPosition::CalcDistance(sCurrPos, _waypoints->CurrentWaypoint()->GetPosition()),
			WAYPOINT_RADIUS);

		if (SPosition::CalcDistance(sCurrPos, _waypoints->CurrentWaypoint()->GetPosition()) < WAYPOINT_RADIUS)
		{
			_waypoints->TraverseWaypoint();
		}
	}

	void AddBehavior(Behavior* nextBehavior) {
		_behaviors.push_back(nextBehavior);
	}

	Behavior* NextBehavior() {
		for(size_t i = 0; i < _behaviors.size(); i++) {
			if (_behaviors[i]->StartCondition()) {
				return _behaviors[i];
			}
		}

		return NULL;
	}

	virtual ~Behavior();
};

#endif /* BEHAVIOR_H_ */
