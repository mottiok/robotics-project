/*
 * Manager.cpp
 *
 *  Created on: May 29, 2015
 *      Author: colman
 */

#include "Manager.h"

Manager::Manager(Robot* robot) {
	_robot = robot;
}

void Manager::Run() {
	while (true) {
		_robot->Read();
		_robot->SetSpeed(0.5, 0);
	}
}

Manager::~Manager() {
	// TODO Auto-generated destructor stub
}

