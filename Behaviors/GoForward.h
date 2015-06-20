/*
 * GoForward.h
 *
 *  Created on: May 29, 2015
 *      Author: colman
 */

#ifndef GOFORWARD_H_
#define GOFORWARD_H_

#include "Behavior.h"

class GoForward: public Behavior {

	bool HasStraightPath(Waypoint* pTarget);
public:
	GoForward(Robot* robot, WaypointManager* waypoints, double pixelResolution, double gridResolution, double mapWidth, double mapHeight) : 
				Behavior(robot, waypoints, pixelResolution, gridResolution, mapWidth, mapHeight) {};

	bool StartCondition() {
//		printf("Is it time to go forward? Is forward free: %u, are we en route to the waypoint? %u\n",
//			_robot->IsForwardFree(), HasStraightPath(_waypoints->CurrentWaypoint()));

		if (NULL == _waypoints->CurrentWaypoint())
			return false;
		else
			return (_robot->IsForwardFree() && (HasStraightPath(_waypoints->CurrentWaypoint()) || !_robot->IsLeftFree() || !_robot->IsRightFree()));
	}

	bool StopCondition() {
		return (!StartCondition() || (NULL == _waypoints->CurrentWaypoint()));
	}

	void Action();

	virtual ~GoForward();
};

#endif /* GOFORWARD_H_ */
