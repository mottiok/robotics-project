/*
 * Manager.cpp
 *
 *  Created on: May 29, 2015
 *      Author: colman
 */

#include "Manager.h"

Manager::Manager(Robot* robot, Plan* plan, WaypointManager* waypoints) {
	_robot = robot;
	_plan = plan;
	_waypoints = waypoints;
	_currentBehavior = plan->StartPointBehavior();
}

void Manager::Run() {
	_robot->Read();
	
	_waypoints->InitWaypointTraversal();
	_waypoints->TraverseWaypoint();

	if (_currentBehavior == NULL || !_currentBehavior->StartCondition()) {

		if (NULL != _currentBehavior)
			_currentBehavior = _currentBehavior->NextBehavior();

		if (NULL == _currentBehavior)
			return;
	}

//	_currentBehavior->Action();

	while(_currentBehavior != NULL) {
		while (!_currentBehavior->StopCondition()) {
			_currentBehavior->Action();
			_robot->Read();
		}
		printf("Changing behavior!\n");
		_currentBehavior = _currentBehavior->NextBehavior();

		_robot->Read();
	}
}

Manager::~Manager() {
	// TODO Auto-generated destructor stub
}

