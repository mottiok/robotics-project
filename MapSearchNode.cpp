/*
 * MapSearchNode.cpp
 *
 *  Created on: Jun 5, 2015
 *      Author: colman
 */

#include "MapSearchNode.h"
#include <math.h>

MapSearchNode::~MapSearchNode() {
	// TODO Auto-generated destructor stub
}

float MapSearchNode::GoalDistanceEstimate(MapSearchNode &nodeGoal) {
    float deltaX = fabs((float)GetXPos() - (float)nodeGoal.GetYPos());
    float deltaY = fabs((float)GetXPos() - (float)nodeGoal.GetYPos());

    return sqrt(deltaX*deltaX + deltaY*deltaY);
}

bool MapSearchNode::IsGoal(MapSearchNode &nodeGoal) {
    return (GetXPos() == nodeGoal.GetXPos() &&
            GetYPos() == nodeGoal.GetYPos());
}

bool MapSearchNode::GetSuccessors(AStarSearch<MapSearchNode>* aStarSearch, MapSearchNode* parentNode) {
    SPosition parentPosition(-1, -1);

    if (parentNode) {
            parentPosition.dwX = parentNode->GetXPos();
            parentPosition.dwY = parentNode->GetYPos();
    }

    // Top Cell
    AddSuccessor(GetXPos(), GetYPos() - 1, parentPosition, aStarSearch);

    // Bottom Cell
    AddSuccessor(GetXPos(), GetYPos() + 1, parentPosition, aStarSearch);

    // Left Cell
    AddSuccessor(GetXPos() - 1, GetYPos(), parentPosition, aStarSearch);

    // Right Cell
    AddSuccessor(GetXPos() + 1, GetYPos(), parentPosition, aStarSearch);

    // TODO: Check if 8 directions is better than 4 when doing the Driver
    // Add 4 more directions if needed
//    AddSuccessor(GetXPos() + 1, GetYPos() + 1, parentPosition, aStarSearch);
    AddSuccessor(GetXPos() - 1, GetYPos() - 1, parentPosition, aStarSearch);
//    AddSuccessor(GetXPos() + 1, GetYPos() - 1, parentPosition, aStarSearch);
//    AddSuccessor(GetXPos() - 1, GetYPos() + 1, parentPosition, aStarSearch);

    return true;
}

bool MapSearchNode::AddSuccessor(dword dX, dword dY, SPosition parentPosition, AStarSearch<MapSearchNode>* aStarSearch) {
    SMapCell* cell = _map->GetMapCell(dX, dY);

    // Add the cell as a successor if it's valid
    if (cell != NULL &&
            !(dX == parentPosition.dwX &&
                dY == parentPosition.dwY) && cell->fIsPassable) {
        SPosition position(dX, dY);
        MapSearchNode newNode = MapSearchNode(position, _map);
        aStarSearch->AddSuccessor(newNode);
        return true;
    }

    return false;
}

float MapSearchNode::GetCost(MapSearchNode &successor) {
    return _map->GetMapCell(successor.GetXPos(), successor.GetYPos())->dwWeight;
}

bool MapSearchNode::IsSameState(MapSearchNode& searchNode) {
    // Same state in our world (grid) is simply when both nodes in the same position
    return (GetXPos() == searchNode.GetXPos() &&
            GetYPos() == searchNode.GetYPos());
}
