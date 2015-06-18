/*
 * TurnRight.h
 *
 *  Created on: May 29, 2015
 *      Author: colman
 */

#ifndef TURNRIGHT_H_
#define TURNRIGHT_H_

#include "Behavior.h"

class TurnRight: public Behavior {
public:

	TurnRight(Robot* robot, WaypointManager* waypoints, double pixelResolution, double gridResolution, double mapWidth, double mapHeight) : 
				Behavior(robot, waypoints, pixelResolution, gridResolution, mapWidth, mapHeight) {};

	bool StartCondition() {
		Waypoint* pTarget = _waypoints->CurrentWaypoint();

//		printf("Time to turn right? Right is free: %u, Is Angle offset %f less than %f ? %u\n",
//					_robot->IsRightFree(), CalcCurrWaypointAngleOffset(pTarget),
//					 -1 * MAX_STRAIGHT_LINE_ERROR,
//					(CalcCurrWaypointAngleOffset(pTarget) < -1 * MAX_STRAIGHT_LINE_ERROR));

		return (_robot->IsRightFree() && (CalcCurrWaypointAngleOffset(pTarget) < -1 * MAX_STRAIGHT_LINE_ERROR) && CalcCurrWaypointAngleOffset(pTarget));
	}

	bool StopCondition() {
		return (!StartCondition());
	}

	void Action() {
		_robot->SetSpeed(0, -0.2);
	}

	virtual ~TurnRight();
};

#endif /* TURNRIGHT_H_ */
