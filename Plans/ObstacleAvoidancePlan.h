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
public:
	ObstacleAvoidancePlan(Robot* robot);
	virtual ~ObstacleAvoidancePlan();
};

#endif /* OBSTACLEAVOIDANCEPLAN_H_ */
