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
#include "Map.h"
#include "SDL2Wrapper.h"

class Manager {
	Robot* _robot;
	Plan* _plan;
	WaypointManager* _waypoints;

	Behavior* _currentBehavior;
	CMap* _map;
	SDL2Wrapper* _sdl;
public:
	Manager(Robot* robot, Plan* plan, WaypointManager* waypoints, CMap* map, SDL2Wrapper* sdl);
	void Run();
	virtual ~Manager();
};

#endif /* MANAGER_H_ */
