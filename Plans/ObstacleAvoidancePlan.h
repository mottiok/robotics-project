/*
 * ObstacleAvoidancePlan.h
 *
 *  Created on: May 29, 2015
 *      Author: colman
 */

#ifndef OBSTACLEAVOIDANCEPLAN_H_
#define OBSTACLEAVOIDANCEPLAN_H_

#include "Plan.h"

class ObstacleAvoidancePlan: public Plan {
	vector<Behavior*> _behaviors;
public:
	ObstacleAvoidancePlan(Robot* robot, WaypointManager* waypoints, float fGridResolution);
	virtual ~ObstacleAvoidancePlan();
};

#endif /* OBSTACLEAVOIDANCEPLAN_H_ */
