/*
 * MapSearchNode.h
 *
 *  Created on: Jun 5, 2015
 *      Author: colman
 */

#ifndef MAPSEARCHNODE_H_
#define MAPSEARCHNODE_H_

#include "Map.h"
#include "Libraries/stlastar.h"

class MapSearchNode {
	CMap* _map;
	SPosition _position;
public:
    MapSearchNode() {
        _map = NULL;
        _position.dwX = 0;
        _position.dwY = 0;
    };

    // TODO:
    // I feel like each node knowing the map is a bad practice, how to refactor this one?
    // Maybe it's better to pass the path planner as a bridge to the map and not the actual map?
    MapSearchNode(SPosition position, CMap* map) {
        _map = map;
        _position = position;
    }

    virtual ~MapSearchNode();

    dword GetXPos() { return _position.dwX; }
    dword GetYPos() { return _position.dwY; }

    void SetPosition(dword dX, dword dY) {
        _position.dwX = dX;
        _position.dwY = dY;
    }

    float GoalDistanceEstimate(MapSearchNode &nodeGoal);
    bool IsGoal(MapSearchNode &nodeGoal);
    bool GetSuccessors(AStarSearch<MapSearchNode>* aStarSearch, MapSearchNode* parentNode);
    bool AddSuccessor(dword dX, dword dY, SPosition parentPosition, AStarSearch<MapSearchNode>* aStarSearch);
    float GetCost(MapSearchNode &successor);
    bool IsSameState(MapSearchNode &node);
};

#endif /* MAPSEARCHNODE_H_ */
