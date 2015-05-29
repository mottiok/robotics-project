/*
 * ObstacleAvoidancePlan.cpp
 *
 *  Created on: May 29, 2015
 *      Author: colman
 */

#include "ObstacleAvoidancePlan.h"
#include "GoForward.h"
#include "TurnRight.h"
#include "TurnLeft.h"

ObstacleAvoidancePlan::ObstacleAvoidancePlan(Robot* robot):Plan(robot) {
	GoForward* forward = new GoForward(robot);
	TurnRight* right = new TurnRight(robot);
	TurnLeft* left = new TurnLeft(robot);

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
	// TODO Auto-generated destructor stub
}

