/*
 * Robot.h
 *
 *  Created on: May 29, 2015
 *      Author: colman
 */

#ifndef ROBOT_H_
#define ROBOT_H_

#include <libplayerc++/playerc++.h>
using namespace PlayerCc;
#include <cmath>

class Robot {
	PlayerClient* _pc;
	Position2dProxy* _pp;
	LaserProxy* _lp;
	double _dX;
	double _dY;
	double _dYaw;
public:
	Robot(const char* ip, int port);

	void Read() {
		_pc->Read();
		
		// Update robot position using the delta from last read
		_dX += _pp->GetXPos();
		_dY += _pp->GetYPos() * -1; // TODO: Check this in real world
		_dYaw = _pp->GetYaw();
		_pp->SetOdometry(0, 0, _dYaw);
	}
	
	void SetOdometryByPixelCoord(double dX, double dY, double dYaw, double dMapResolution) {
		_dX = dX * (dMapResolution / 100);
		_dY = dY * (dMapResolution / 100);
		_dYaw = DTOR(dYaw);
		_pp->SetOdometry(0, 0, _dYaw);
	}
	
	double GetX() {
		return _dX;
	}
	
	double GetY() {
		return _dY;
	}
	
	double GetYaw() {
		return _dYaw;
	}

	void SetSpeed(double aXSpeed, double aYawSpeed) {
		_pp->SetSpeed(aXSpeed, aYawSpeed);
	}

	bool IsForwardFree() {
		if (((*_lp)[332]) > 0.7) {
			return true;
		}

		return false;
	}

	bool IsRightFree() {
		if (((*_lp)[50]) > 0.7) {
			return true;
		}

		return false;
	}

	bool IsLeftFree() {
		if (((*_lp)[615]) > 0.7) {
			return true;
		}

		return false;
	}

	virtual ~Robot();
};

#endif /* ROBOT_H_ */
