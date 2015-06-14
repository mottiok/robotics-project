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
}

void Manager::Run() {
	_robot->ReadAndUpdateLocalization();
	_map->DrawMapState(_sdl, true);
	
	_waypoints->InitWaypointTraversal();
	_waypoints->TraverseWaypoint();

	if (_currentBehavior == NULL || !_currentBehavior->StartCondition()) {

		if (NULL != _currentBehavior)
			_currentBehavior = _currentBehavior->NextBehavior();

		if (NULL == _currentBehavior)
			return;
	}

	_currentBehavior->Action();
	
	while(_currentBehavior != NULL) {
		while (!_currentBehavior->StopCondition()) {
			_map->DrawMapState(_sdl, false);
			_currentBehavior->Action();
			_robot->ReadAndUpdateLocalization();
			_sdl->FlushChanges();
		}
//		printf("Changing behavior!\n");
		_currentBehavior = _currentBehavior->NextBehavior();
		_robot->ReadAndUpdateLocalization();
	}
}

Manager::~Manager() {
	// TODO Auto-generated destructor stub
}

