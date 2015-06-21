/*
 * GoForward.cpp
 *
 *  Created on: May 29, 2015
 *      Author: colman
 */

#include "GoForward.h"
#include <math.h>

#define CORRECTIVE_TURN_THRESHOLD (M_PI/24) // Do corrective turns when we miss by 7.5 degrees
#define CORRECTIVE_TURN_MODIFIER (M_PI/60) // Turn by 3 degrees

bool GoForward::HasStraightPath(Waypoint* pTarget)
{
	float fGradientError = abs(CalcCurrWaypointAngleOffset(pTarget));

	return (fGradientError < MAX_STRAIGHT_LINE_ERROR);
}

bool GoForward::IsObstructed(Waypoint* pTarget)
{
	SPosition sCurrPos;
	sCurrPos.dwX = Convert::RobotRelativeXPosToPixelXCoord(_robot->GetX(), CM_TO_METERS(_pixelResolution), _mapWidth);
	sCurrPos.dwY = Convert::RobotRelativeYPosToPixelYCoord(_robot->GetY(), CM_TO_METERS(_pixelResolution), _mapHeight);

	sCurrPos.dwX = (sCurrPos.dwX / (_gridResolution / _pixelResolution));
	sCurrPos.dwY = (sCurrPos.dwY / (_gridResolution / _pixelResolution));

	//printf("Distance to next WP: %f\n", SPosition::CalcDistance(sCurrPos, _waypoints->CurrentWaypoint()->GetPosition()));

	return (SPosition::CalcDistance(sCurrPos, _waypoints->CurrentWaypoint()->GetPosition()) > WAYPOINT_RADIUS &&
			!_robot->IsForwardFree(DETECT_EVASION_RADIUS));
}

void GoForward::Action()
{
	float fGradientError = CalcCurrWaypointAngleOffset(_waypoints->CurrentWaypoint());
	double dYawSpeed = 0;

	if (abs(fGradientError) > CORRECTIVE_TURN_THRESHOLD)
	{
		dYawSpeed = (fGradientError > 0) ? (-1.0) : (1.0);

		dYawSpeed *= CORRECTIVE_TURN_MODIFIER;
	}

	if (IsObstructed(_waypoints->CurrentWaypoint()))
	{
		printf("EVASIVE MANEUVERS CAPTAIN!\n");
		_evasiveManeuvers = true;
	}

	_robot->SetSpeed(0.2, dYawSpeed);

	Behavior::Action();
}

GoForward::~GoForward() {
	// TODO Auto-generated destructor stub
}

