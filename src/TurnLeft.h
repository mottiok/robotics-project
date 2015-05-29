/*
 * TurnLeft.h
 *
 *  Created on: May 29, 2015
 *      Author: colman
 */

#ifndef TURNLEFT_H_
#define TURNLEFT_H_

#include "Behavior.h"

class TurnLeft: public Behavior {
public:
	TurnLeft(Robot* robot);

	bool StartCondition() {
		return _robot->IsLeftFree();
	}

	bool StopCondition() {
		return _robot->IsForwardFree();
	}

	void Action() {
		_robot->SetSpeed(0, 0.2);
	}

	virtual ~TurnLeft();
};

#endif /* TURNLEFT_H_ */
