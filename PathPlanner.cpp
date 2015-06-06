/*
 * PathPlanner.cpp
 *
 *  Created on: Jun 5, 2015
 *      Author: colman
 */

#include "PathPlanner.h"
#include "Libraries/stlastar.h"

// Debug
#define DRAW_ALGORITHM_PROCESS
#define ALGORITHM_EXPANSION_COLOR 200, 255, 255
#define RED_RGB_FORMAT 255, 0, 0
#define GREEN_RGB_FORMAT 0, 255, 0
#define BLUE_RGB_FORMAT 0, 0, 255

PathPlanner::PathPlanner(CMap* map) {
    _map = map;
    _aStarAlgorithm = new AStarSearch<MapSearchNode>(MAX_NODES_COUNT);
}

PathPlanner::~PathPlanner() {
    _aStarAlgorithm->EnsureMemoryFreed();
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
        MapSearchNode *node = _aStarAlgorithm->GetOpenListStart();
        while (node) {
#ifdef DRAW_ALGORITHM_PROCESS
            _map->ColorCellByCoord(node->GetXPos(), node->GetYPos(), ALGORITHM_EXPANSION_COLOR);
#endif
            node = _aStarAlgorithm->GetOpenListNext();
        }
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
#ifdef DRAW_ALGORITHM_PROCESS
        _map->ColorCellByCoord(node->GetXPos(), node->GetYPos(), RED_RGB_FORMAT);
#endif
        nodesFromStartToGoal.push_back(node);

        for (;;) {
            node = _aStarAlgorithm->GetSolutionNext();

            if (!node) {
#ifdef DRAW_ALGORITHM_PROCESS
                node = _aStarAlgorithm->GetSolutionEnd();
                _map->ColorCellByCoord(node->GetXPos(), node->GetYPos(), GREEN_RGB_FORMAT);
#endif
                break;
            }

#ifdef DRAW_ALGORITHM_PROCESS
            _map->ColorCellByCoord(node->GetXPos(), node->GetYPos(), BLUE_RGB_FORMAT);
#endif
            nodes++;
            nodesFromStartToGoal.push_back(node);
        }

        printf("Path made of %d nodes.\n", nodes);
    } else {
        printf("Search terminated after %d steps. Didn't find goal.\n", searchSteps);
    }

    _aStarAlgorithm->FreeSolutionNodes();
    _aStarAlgorithm->EnsureMemoryFreed();

    return (searchState == AStarSearch<MapSearchNode>::SEARCH_STATE_SUCCEEDED);
}