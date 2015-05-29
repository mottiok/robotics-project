/*
 * Manager.h
 *
 *  Created on: May 29, 2015
 *      Author: colman
 */

#ifndef MANAGER_H_
#define MANAGER_H_

#include "Robot.h"
#include "Behaviors/Behavior.h"
#include "Plans/Plan.h"

class Manager {
	Robot* _robot;
	Plan* _plan;
	Behavior* _currentBehavior;
public:
	Manager(Robot* robot, Plan* plan);
	void Run();
	virtual ~Manager();
};

#endif /* MANAGER_H_ */
