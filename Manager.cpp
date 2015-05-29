/*
 * Manager.cpp
 *
 *  Created on: May 29, 2015
 *      Author: colman
 */

#include "Manager.h"

Manager::Manager(Robot* robot, Plan* plan, Map* map) {
	_robot = robot;
	_plan = plan;
	_map = map;
	_currentBehavior = plan->StartPointBehavior();
}

void Manager::Run() {
	_robot->Read();

	if (_currentBehavior == NULL || !_currentBehavior->StartCondition()) {
		return;
	}

	_currentBehavior->Action();

	while(_currentBehavior != NULL) {
		while (!_currentBehavior->StopCondition()) {
			_currentBehavior->Action();
			_robot->Read();
			// TODO: The current X,Y of the robot is relative to the start position - can't continue until I'll have particles and actual grid
			//_map->DrawPoint(_robot->GetXPos(), _robot->GetYPos(), 0, 0, 0, true);
		}

		_currentBehavior = _currentBehavior->NextBehavior();
		_robot->Read();
	}
}

Manager::~Manager() {
	// TODO Auto-generated destructor stub
}

