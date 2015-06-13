/*
 * TurnLeft.h
 *
 *  Created on: May 29, 2015
 *      Author: colman
 */

#ifndef TURNLEFT_H_
#define TURNLEFT_H_

#include "Behavior.h"

class TurnLeft: public Behavior {
public:

	TurnLeft(Robot* robot, WaypointManager* waypoints, float fGridResolution) : Behavior(robot, waypoints, fGridResolution) {};

	bool StartCondition() {
		Waypoint* pTarget = _waypoints->CurrentWaypoint();

		printf("Time to turn left? Left is free: %u, Is Angle offset %f more than %f ? %u\n",
			_robot->IsLeftFree(), CalcCurrWaypointAngleOffset(pTarget), MAX_STRAIGHT_LINE_ERROR,
			(CalcCurrWaypointAngleOffset(pTarget) > MAX_STRAIGHT_LINE_ERROR));
		return (_robot->IsLeftFree() && (CalcCurrWaypointAngleOffset(pTarget) > MAX_STRAIGHT_LINE_ERROR));
	}

	bool StopCondition() {
		return (!StartCondition());
	}

	void Action() {
		_robot->SetSpeed(0, 0.2);
		Behavior::Action();
	}

	virtual ~TurnLeft();
};

#endif /* TURNLEFT_H_ */
