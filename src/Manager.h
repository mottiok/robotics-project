/*
 * Manager.h
 *
 *  Created on: May 29, 2015
 *      Author: colman
 */

#ifndef MANAGER_H_
#define MANAGER_H_

#include "Robot.h"
#include "Behavior.h"

class Manager {
	Robot* _robot;
public:
	Manager(Robot* robot);
	void Run();
	virtual ~Manager();
};

#endif /* MANAGER_H_ */
