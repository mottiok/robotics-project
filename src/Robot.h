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

class Robot {
	PlayerClient* _pc;
	Position2dProxy* _pp;
	LaserProxy* _lp;
public:
	Robot(const char* ip, int port);

	void Read() {
		_pc->Read();
	}

	void SetSpeed(double aXSpeed, double aYawSpeed) {
		_pp->SetSpeed(aXSpeed, aYawSpeed);
	}

	virtual ~Robot();
};

#endif /* ROBOT_H_ */
