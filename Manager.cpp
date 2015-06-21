/*
 * Manager.cpp
 *
 *  Created on: May 29, 2015
 *      Author: colman
 */

#include "Manager.h"
#include "Debug.h"

Manager::Manager(Robot* robot, Plan* plan, WaypointManager* waypoints, CMap* map, SDL2Wrapper* sdl) {
	_robot = robot;
	_plan = plan;
	_waypoints = waypoints;
	_map = map;
	_sdl = sdl;
	_currentBehavior = plan->StartPointBehavior();

	_waypoints->InitWaypointTraversal();
	_waypoints->TraverseWaypoint();
	
	if (!_currentBehavior->StartCondition(_currentBehavior->GetBehaviorType()))
		_currentBehavior = _currentBehavior->NextBehavior();
}

void Manager::Run() {
	_robot->ReadAndUpdateLocalization();
	
	_waypoints->InitWaypointTraversal();
	_waypoints->TraverseWaypoint();

	if (_currentBehavior == NULL || !_currentBehavior->StartCondition(_currentBehavior->GetBehaviorType())) {
		return;
	}

	_currentBehavior->Action();
	
	while(_currentBehavior != NULL) {
		while (!_currentBehavior->StopCondition()) {
			_currentBehavior->Action();
			_robot->ReadAndUpdateLocalization();
			_sdl->FlushChanges();
		}
		printf("Changing behavior! Are we evasive? %u\n", _currentBehavior->IsEvasive());

		_currentBehavior = _currentBehavior->NextBehavior();
		_robot->ReadAndUpdateLocalization();
	}
}

Manager::~Manager() {
	// TODO Auto-generated destructor stub
}

