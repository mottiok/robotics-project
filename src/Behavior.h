/*
 * Behavior.h
 *
 *  Created on: May 29, 2015
 *      Author: colman
 */

#ifndef BEHAVIOR_H_
#define BEHAVIOR_H_

#include "Robot.h"
#include <vector>
using namespace std;

class Behavior {
	vector<Behavior*> _behaviors;
protected:
	Robot* _robot;
public:
	Behavior(Robot* robot);
	virtual bool StartCondition() = 0;
	virtual bool EndCondition() = 0;
	virtual bool Action() = 0;
	void AddBehavior(Behavior* nextBehavior) {
		_behaviors.push_back(nextBehavior);
	}
	virtual ~Behavior();
};

#endif /* BEHAVIOR_H_ */
