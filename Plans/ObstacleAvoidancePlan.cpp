/*
 * ObstacleAvoidancePlan.cpp
 *
 *  Created on: May 29, 2015
 *      Author: colman
 */

#include "ObstacleAvoidancePlan.h"
#include "../Behaviors/GoForward.h"
#include "../Behaviors/TurnRight.h"
#include "../Behaviors/TurnLeft.h"

ObstacleAvoidancePlan::ObstacleAvoidancePlan(Robot* robot, WaypointManager* waypoints, float fGridResolution):Plan(robot) {
	GoForward* forward = new GoForward(robot, waypoints, fGridResolution);
	TurnRight* right = new TurnRight(robot, waypoints, fGridResolution);
	TurnLeft* left = new TurnLeft(robot, waypoints, fGridResolution);

	forward->AddBehavior(right);
	forward->AddBehavior(left);

	right->AddBehavior(forward);
	left->AddBehavior(forward);

	_behaviors.push_back(forward);
	_behaviors.push_back(right);
	_behaviors.push_back(left);

	_startPointBehavior = forward;
}

ObstacleAvoidancePlan::~ObstacleAvoidancePlan() {
	for (size_t i = 0; i < _behaviors.size(); i++) {
		delete _behaviors[i];
	}
}

