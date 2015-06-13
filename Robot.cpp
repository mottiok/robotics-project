/*
 * Robot.cpp
 *
 *  Created on: May 29, 2015
 *      Author: colman
 */

#include "Robot.h"

Robot::Robot(const char* ip, int port) {
	_pc = new PlayerClient(ip, port);
	_pp = new Position2dProxy(_pc);
	_lp = new LaserProxy(_pc);

	_pp->SetMotorEnable(true);

	// Refresh Player's reading BUG
	for (int i=0; i<15; i++)
		Read();
	
	_dX = _dY = _dYaw = 0;
}

Robot::~Robot() {
	delete _lp;
	delete _pp;
	delete _pc;
}

