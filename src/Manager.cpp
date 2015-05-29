/*
 * Manager.cpp
 *
 *  Created on: May 29, 2015
 *      Author: colman
 */

#include "Manager.h"

Manager::Manager(Robot* robot, Plan* plan) {
	_robot = robot;
	_plan = plan;
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
		}

		_currentBehavior = _currentBehavior->NextBehavior();
		_robot->Read();
	}
}

Manager::~Manager() {
	// TODO Auto-generated destructor stub
}

