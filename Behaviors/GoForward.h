/*
 * GoForward.h
 *
 *  Created on: May 29, 2015
 *      Author: colman
 */

#ifndef GOFORWARD_H_
#define GOFORWARD_H_

#define MAX_EVASIVE_ANGLE (M_PI/4)

#include "Behavior.h"

class GoForward: public Behavior {

	bool HasStraightPath(Waypoint* pTarget);
	bool IsObstructed(Waypoint* pTarget);
public:
	GoForward(Robot* robot, WaypointManager* waypoints, double pixelResolution, double gridResolution, double mapWidth, double mapHeight) : 
				Behavior(robot, waypoints, pixelResolution, gridResolution, mapWidth, mapHeight, eBehavior_GoForward) {};

	bool StartCondition(eBehavior eSRCBehavior) {
		//printf("Is it time to go forward? Is forward free: %u, are we en route to the waypoint? %u\n",
			//_robot->IsForwardFree(), HasStraightPath(_waypoints->CurrentWaypoint()));

		if (NULL == _waypoints->CurrentWaypoint())
			return false;
		else
		{
//			printf("Time to go forward? Is left blocked? %u, Is forward free? %u, Got a path? %u\n", !_robot->IsLeftFree(DETECT_EVASION_RADIUS), _robot->IsForwardFree(), HasStraightPath(_waypoints->CurrentWaypoint()));

			if (IsObstructed(_waypoints->CurrentWaypoint()))
			{
//				printf("EVASIVE MANEUVERS HNNNGH\n");
				_evasiveManeuvers = true;
			}

			if (!IsEvasive())
				return (_robot->IsForwardFree() && HasStraightPath(_waypoints->CurrentWaypoint()));
			else
			{
				//bool fIsRelevantOffsetFree = ((CalcCurrWaypointAngleOffset(_waypoints->CurrentWaypoint()) < 0) ? 
				//				(_robot->IsRightFree()) :(_robot->IsLeftFree()));

				bool fIsRelevantOffsetFree = _robot->IsRightFree();

				/*printf("Angle offset is %f, is the relevant offset free? %u, is forward free? %u\n", 
					CalcCurrWaypointAngleOffset(_waypoints->CurrentWaypoint()), 
					fIsRelevantOffsetFree, _robot->IsForwardFree(DETECT_EVASION_RADIUS));*/

				if (/*eBehavior_GoForward == eSRCBehavior && */_robot->IsLeftFree(DETECT_EVASION_RADIUS) && _robot->IsForwardFree(DETECT_EVASION_RADIUS))
				{
					SetEvasive(false);
				}
				return _robot->IsForwardFree(DETECT_EVASION_RADIUS);
			}
		}
	}

	bool StopCondition() {
		return (!StartCondition(_behaviorType) || (NULL == _waypoints->CurrentWaypoint()));
	}

	void Action();

	virtual ~GoForward();
};

#endif /* GOFORWARD_H_ */
