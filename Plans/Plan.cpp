/*
 * Plan.cpp
 *
 *  Created on: May 29, 2015
 *      Author: colman
 */

#include "Plan.h"

Plan::Plan(Robot* robot) {
	_startPointBehavior = NULL;
}

Plan::~Plan() {
	for (size_t i = 0; i < _behaviors.size(); i++) {
		delete _behaviors[i];
	}
}

