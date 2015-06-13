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
	GoForward(Robot* robot, WaypointManager* waypoints, float fGridResolution) : Behavior(robot, waypoints, fGridResolution) {};

	bool StartCondition() {
		printf("Is it time to go forward? Is forward free: %u, are we en route to the waypoint? %u\n",
			_robot->IsForwardFree(), HasStraightPath(_waypoints->CurrentWaypoint()));
		return (_robot->IsForwardFree() && HasStraightPath(_waypoints->CurrentWaypoint()));
	}

	bool StopCondition() {
		return (!StartCondition());
	}

	void Action();

	virtual ~GoForward();
};

#endif /* GOFORWARD_H_ */
