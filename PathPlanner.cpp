/*
 * PathPlanner.cpp
 *
 *  Created on: Jun 5, 2015
 *      Author: colman
 */

#include "PathPlanner.h"
#include "Libraries/stlastar.h"
#include "Debug.h"

PathPlanner::PathPlanner(CMap* map) {
    _map = map;
    _aStarAlgorithm = new AStarSearch<MapSearchNode>(MAX_NODES_COUNT);
}

PathPlanner::~PathPlanner() {
//    _aStarAlgorithm->EnsureMemoryFreed();
    delete _aStarAlgorithm;
}

void PathPlanner::SetStartAndGoalStates(SPosition startPosition, SPosition goalPosition) {
    MapSearchNode startNode(startPosition, _map);
    MapSearchNode goalNode(goalPosition, _map);

    _aStarAlgorithm->SetStartAndGoalStates(startNode, goalNode);
}

void PathPlanner::SearchPath(dword& searchState, int& searchSteps) {
    do {
        searchSteps++;
        searchState = _aStarAlgorithm->SearchStep();
#ifdef DRAW_ASTAR_EXPANSION
        MapSearchNode *node = _aStarAlgorithm->GetOpenListStart();
        while (node) {

            _map->ColorCellByCoord(node->GetXPos(), node->GetYPos(), ASTAR_EXPANSION_COLOR);
            node = _aStarAlgorithm->GetOpenListNext();
        }
#endif
    } while (searchState == AStarSearch<MapSearchNode>::SEARCH_STATE_SEARCHING);
}

bool PathPlanner::GetPathByStartAndGoalPosition(SPosition startPosition,
        SPosition goalPosition,
        vector<MapSearchNode*>& nodesFromStartToGoal) {
    printf("Searching the best path from (%u,%u) to (%u,%u).\n",
                startPosition.dwX, startPosition.dwY,
                goalPosition.dwX, goalPosition.dwY);

    SetStartAndGoalStates(startPosition, goalPosition);

    dword searchState;
    int searchSteps = 0;

    SearchPath(searchState, searchSteps);

    if (searchState == AStarSearch<MapSearchNode>::SEARCH_STATE_SUCCEEDED) {
        printf("Goal found after %d steps.\n", searchSteps);

        int nodes = 1;
        MapSearchNode *node = _aStarAlgorithm->GetSolutionStart();
        nodesFromStartToGoal.push_back(node);

        for (;;) {
            node = _aStarAlgorithm->GetSolutionNext();

            if (!node) {
                break;
            }

            nodes++;
            nodesFromStartToGoal.push_back(node);
        }

        printf("Path made of %d nodes.\n", nodes);
    } else {
        printf("Search terminated after %d steps. Didn't find goal.\n", searchSteps);
    }

    _aStarAlgorithm->FreeSolutionNodes();
//    _aStarAlgorithm->EnsureMemoryFreed(); //TODO: Known bug, if no path found memory allocation is negative and the assertion failed.

    return (searchState == AStarSearch<MapSearchNode>::SEARCH_STATE_SUCCEEDED);
}
