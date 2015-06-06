/*
 * PathPlanner.h
 *
 *  Created on: Jun 5, 2015
 *      Author: colman
 */

#ifndef PATHPLANNER_H_
#define PATHPLANNER_H_

#include "Map.h"
#include "MapSearchNode.h"

// A-Star Max Nodes Count - A better value could be the number of cells we have
#define MAX_NODES_COUNT 32767

class PathPlanner {
    CMap* _map;
    AStarSearch<MapSearchNode>* _aStarAlgorithm;

    void SetStartAndGoalStates(SPosition startPosition, SPosition goalPosition);
    void SearchPath(dword& searchState, int& searchSteps);
public:
    PathPlanner(CMap* map);
    virtual ~PathPlanner();

    bool GetPathByStartAndGoalPosition(SPosition startPosition,
            SPosition goalPosition,
            vector<MapSearchNode*>& nodesFromStartToGoal);
};

#endif /* PATHPLANNER_H_ */
