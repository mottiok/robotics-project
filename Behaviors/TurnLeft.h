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

	TurnLeft(Robot* robot, WaypointManager* waypoints, double pixelResolution, double gridResolution, double mapWidth, double mapHeight) : 
				Behavior(robot, waypoints, pixelResolution, gridResolution, mapWidth, mapHeight) {};

	bool StartCondition() {
		Waypoint* pTarget = _waypoints->CurrentWaypoint();

		/*printf("Time to turn left? Left is free: %u, Is Angle offset %f more than %f ? %u, Is forward blocked? %u\n",
			_robot->IsLeftFree(), CalcCurrWaypointAngleOffset(pTarget), MAX_STRAIGHT_LINE_ERROR,
			(CalcCurrWaypointAngleOffset(pTarget) > MAX_STRAIGHT_LINE_ERROR), !_robot->IsForwardFree());*/
		if (NULL == _waypoints->CurrentWaypoint())
			return false;
		else
			return (_robot->IsLeftFree() && ((CalcCurrWaypointAngleOffset(pTarget) > MAX_STRAIGHT_LINE_ERROR) || !_robot->IsForwardFree()));
	}

	bool StopCondition() {
		return (!StartCondition() || (NULL == _waypoints->CurrentWaypoint()));
	}

	void Action() {
		_robot->SetSpeed(0, 0.2);
	}

	virtual ~TurnLeft();
};

#endif /* TURNLEFT_H_ */
