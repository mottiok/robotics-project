/*
 * Plan.h
 *
 *  Created on: May 29, 2015
 *      Author: colman
 */

#ifndef PLAN_H_
#define PLAN_H_

#include "../Robot.h"
#include "../Behaviors/Behavior.h"

class Plan {
protected:
	Behavior* _startPointBehavior;
	vector<Behavior*> _behaviors;
public:
	Plan(Robot* robot);
	Behavior* StartPointBehavior() { return _startPointBehavior; }
	virtual ~Plan();
};

#endif /* PLAN_H_ */
