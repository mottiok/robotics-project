/*
 * Robot.cpp
 *
 *  Created on: May 29, 2015
 *      Author: colman
 */

#include "Robot.h"

Robot::Robot(const char* ip, int port, LocalizationManager* localization) {
	_pc = new PlayerClient(ip, port);
	_pp = new Position2dProxy(_pc);
	_lp = new LaserProxy(_pc);
	_localization = localization;

	_pp->SetMotorEnable(true);
	
	/*
	 * Refresh Player's reading BUG
	 * Internet suggested to use _pp->IsValid instead but it's not working
	 */
	for (int i=0; i<15; i++) {
		Read();
	}
}

Robot::~Robot() {
	delete _lp;
	delete _pp;
	delete _pc;
}

void Robot::Read() {
	_pc->Read();
}

void Robot::ReadAndUpdateLocalization() {
	Read();
	UpdateLocalization();
}

double Robot::SetPosition(double dX, double dY, double dYaw, bool setOdemetry) {
	_dX = dX;
	_dY = dY;
	_dYaw = dYaw;

	if (setOdemetry) {
		SetOdemetry(dX, dY, dYaw);
	}
}

void Robot::SetOdemetry(double dX, double dY, double dYaw) {
	_pp->SetOdometry(dX, dY, dYaw);
}

double Robot::GetX() {
	return _dX;
}

double Robot::GetY() {
	return _dY;
}

double Robot::GetYaw() {
	return _dYaw;
}

void Robot::SetSpeed(double aXSpeed, double aYawSpeed) {
	_pp->SetSpeed(aXSpeed, aYawSpeed);
}

bool Robot::IsForwardFree(float fRadius) {
	if (((*_lp)[332]) > fRadius && ((*_lp)[260]) > fRadius && ((*_lp)[404]) > fRadius) {
		return true;
	}

	return false;
}

bool Robot::IsRightFree(float fRadius) {
	if (((*_lp)[166]) > fRadius) {
		return true;
	}
	//printf("Cannot turn right! R: %f\n", (*_lp)[166]);
	return false;
}

bool Robot::IsLeftFree(float fRadius) {
	if (((*_lp)[498]) > fRadius) {
		return true;
	}
	//printf("Cannot turn left! R: %f\n", (*_lp)[498]);
	return false;
}

void Robot::SetOdometryByPixelCoord(double dX, double dY, double dYaw, double resolution, double mapWidth, double mapHeight) {		
	SetPosition(Convert::PixelXCoordToRobotRelativeXPos(dX, CM_TO_METERS(resolution), mapWidth),
				Convert::PixelYCoordToRobotRelativeYPos(dY, CM_TO_METERS(resolution), mapHeight),
				DTOR(dYaw), true);

	_localization->CreateParticle(_dX, _dY, _dYaw, 1);
}

void Robot::UpdateLocalization() {
	double deltaX = _pp->GetXPos() - _dX;
	double deltaY = _pp->GetYPos() - _dY;
	double deltaYaw = _pp->GetYaw() - _dYaw;
	
	/*
	 * We update the localization manager by the delta position and chose the
	 * best particle to use as our real position
	 */
	_localization->Update(deltaX, deltaY, deltaYaw, _lp);
	Particle* particle = _localization->GetBestParticle();
	SetPosition(particle->GetX(), particle->GetY(), particle->GetYaw(), false);
}
