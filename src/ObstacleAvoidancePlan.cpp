/*
 * ObstacleAvoidancePlan.cpp
 *
 *  Created on: May 29, 2015
 *      Author: colman
 */

#include "ObstacleAvoidancePlan.h"
#include "GoForward.h"

ObstacleAvoidancePlan::ObstacleAvoidancePlan(Robot* robot):Plan(robot) {
	GoForward* forward = new GoForward(robot);
	_behaviors.push_back(forward);

	_startPointBehavior = forward;
}

ObstacleAvoidancePlan::~ObstacleAvoidancePlan() {
	// TODO Auto-generated destructor stub
}

