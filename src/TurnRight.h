/*
 * TurnRight.h
 *
 *  Created on: May 29, 2015
 *      Author: colman
 */

#ifndef TURNRIGHT_H_
#define TURNRIGHT_H_

#include "Behavior.h"

class TurnRight: public Behavior {
public:
	TurnRight(Robot* robot);

	bool StartCondition() {
		return _robot->IsRightFree();
	}

	bool StopCondition() {
		return _robot->IsForwardFree();
	}

	void Action() {
		_robot->SetSpeed(0, -0.2);
	}

	virtual ~TurnRight();
};

#endif /* TURNRIGHT_H_ */
