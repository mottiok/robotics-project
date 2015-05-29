/*
 * GoForward.h
 *
 *  Created on: May 29, 2015
 *      Author: colman
 */

#ifndef GOFORWARD_H_
#define GOFORWARD_H_

#include "Behavior.h"

class GoForward: public Behavior {
public:
	GoForward(Robot* robot);

	bool StartCondition() {
		return _robot->IsForwardFree();
	}

	bool StopCondition() {
		return !_robot->IsForwardFree();
	}

	void Action() {
		_robot->SetSpeed(0.5, 0);
	}

	virtual ~GoForward();
};

#endif /* GOFORWARD_H_ */
