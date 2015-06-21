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
				Behavior(robot, waypoints, pixelResolution, gridResolution, mapWidth, mapHeight, eBehavior_TurnRight) {};

	bool StartCondition(eBehavior eSRCBehavior) {
		Waypoint* pTarget = _waypoints->CurrentWaypoint();

		/*printf("Time to turn right? Right is free: %u, Is Angle offset %f less than %f ? %u, Is forward blocked? %u\n",
					_robot->IsRightFree(), CalcCurrWaypointAngleOffset(pTarget),
					 -1 * MAX_STRAIGHT_LINE_ERROR,
					(CalcCurrWaypointAngleOffset(pTarget) < -1 * MAX_STRAIGHT_LINE_ERROR),
					!_robot->IsForwardFree(DETECT_EVASION_RADIUS));*/

//		printf("Time to turn right? Is right free? %u, Is left blocked? %u Is forward blocked? %u\n", _robot->IsRightFree(), !_robot->IsLeftFree(DETECT_EVASION_RADIUS), !_robot->IsForwardFree(DETECT_EVASION_RADIUS)); 

		if (NULL == _waypoints->CurrentWaypoint())
			return false;
		else
		{
			if (IsEvasive())
			{
				if (!_robot->IsForwardFree(DETECT_EVASION_RADIUS))
				{
//					printf("fw blocked, turning right on purpose\n");
					return _robot->IsRightFree();
				}
				else	
					return (_robot->IsRightFree() && _robot->IsLeftFree(DETECT_EVASION_RADIUS));
			}
			else
				return (_robot->IsRightFree() && (CalcCurrWaypointAngleOffset(pTarget) < -1 * MAX_STRAIGHT_LINE_ERROR));
		}
	}

	bool StopCondition() {
		return (!StartCondition(_behaviorType) || (NULL == _waypoints->CurrentWaypoint()));
	}

	void Action() {
		_robot->SetSpeed(0, -0.4);
	}

	virtual ~TurnRight();
};

#endif /* TURNRIGHT_H_ */
