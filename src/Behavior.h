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
	virtual void Action() = 0;

	void AddBehavior(Behavior* nextBehavior) {
		_behaviors.push_back(nextBehavior);
	}

	Behavior* NextBehavior() {
		for(vector<Behavior*>::iterator it = _behaviors.begin(); it != _behaviors.end(); ++it) {
			if ((*it)->StartCondition()) {
				return *it;
			}
		}

		// TODO: Do something if all the behaviors return false - maybe return 'this'?
		return this; // Not the best solution - possible infinite loop
	}

	virtual ~Behavior();
};

#endif /* BEHAVIOR_H_ */
