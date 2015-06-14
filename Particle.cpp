/* 
 * File:   Particle.cpp
 * Author: colman
 * 
 * Created on June 12, 2015, 12:43 PM
 */

#include "Particle.h"
#include "Debug.h"

Particle::Particle(double dX, double dY, double dYaw, double dBel) {
	_dX = dX;
	_dY = dY;
	_dYaw = dYaw;
	_dBel = dBel;
	
	_lifes = PARTICLE_LIFES;
	_age = 1;
}

Particle::~Particle() {
}

Particle* Particle::CreateChild() {
	return CreateChild(EXPANSION_RADIUS);
}

Particle* Particle::CreateChild(double dExpansionRadius) {
	double newX = _dX + Randomize(-dExpansionRadius, dExpansionRadius);
	double newY = _dY + Randomize(-dExpansionRadius, dExpansionRadius);
	double newYaw = _dYaw + Randomize(-dExpansionRadius, dExpansionRadius);
	return new Particle(newX, newY, newYaw, 1);
}

void Particle::Update(double deltaX, double deltaY, double deltaYaw, CMap* map, LaserProxy* lp) {
	_dX += deltaX;
	_dY += deltaY;
	_dYaw += deltaYaw;
	
	// Calculate new belif value from prediction belif, laser scan and the belif magic number
	double predictionBelif = ProbabilityByMovement(deltaX, deltaY, deltaYaw) * _dBel;
	double probabilityByScan = ProbabilityByLaserScan(_dX, _dY, _dYaw, map, lp);
	_dBel = probabilityByScan * predictionBelif * BELIF_MAGIC_NUMBER;
	
	if (_dBel > 1)
		_dBel = 1;
}

double Particle::GetBelif() {
	return _dBel;
}

double Particle::GetX() {
	return _dX;
}

double Particle::GetY() {
	return _dY;
}

double Particle::GetYaw() {
	return _dYaw;
}

double Particle::Randomize(double dMin, double dMax) {
	double num = (double)rand() / RAND_MAX;
	return dMin + num * (dMax - dMin);
}

double Particle::ProbabilityByMovement(double deltaX, double deltaY, double deltaYaw) {
	
	double distance = sqrt(pow(deltaX,2) + pow(deltaY,2));
	
	/*
	 * Welcome to magic numbers section - this one need a refactoring
	 * Any good ideas how to make it better?
	 */
	
	if (distance < 0.1)
		return 1;
	
	if (distance < 0.2)
		return 0.9;
	
	if (distance < 0.4)
		return 0.7;
	
	if (distance < 0.5)
		return 0.5;
	
	if (distance < 0.7)
		return 0.3;
	
	return 0.1;
}

double Particle::ProbabilityByLaserScan(double dX, double dY, double dYaw, CMap* map, LaserProxy* lp) {
	int scans = lp->GetCount();
	double maxRange = lp->GetMaxRange();
	
	double correctHits = 0;
	double totalHits = 0;
	
	for(int i=0; i<scans; i++) {
		double range = lp->GetRange(i);
		
		if (range < maxRange) {
			totalHits++;
			
			// Calculate obstacle position
			double bearing = lp->GetBearing(i);
			double dObstacleX = dX + range * cos(dYaw + bearing);
			double dObstacleY = dY + range * sin(dYaw + bearing);
			
			// Convert relative obstacle position to our valid map position
			double resolution = CM_TO_METERS(map->GetPixelResolution());
			double mapWidth = map->GetMapWidth();
			double mapHeight = map->GetMapHeight();
			
			dObstacleX = Convert::RobotRelativeXPosToPixelXCoord(dObstacleX, resolution, mapWidth);
			dObstacleY = Convert::RobotRelativeYPosToPixelYCoord(dObstacleY, resolution, mapHeight);
			
			// Check bounds before actually trying to get cell
			if (dObstacleX < 0 || dObstacleX >= map->GetMapWidth() ||
					dObstacleY < 0 || dObstacleY >= map->GetMapHeight()) {
				continue;
			}
			
			SPosition cellPosition = map->PixelCoordToCellPosition(dObstacleX, dObstacleY);
			SMapCell* cell = map->GetMapCell(cellPosition.dwX, cellPosition.dwY);
			
			if (!cell->fIsPassable) {
				correctHits++;
			}
		}
	}
	
	double accuracy = correctHits / totalHits;
	
	return accuracy;
}

// TODO: This is the same as above (almost) - try to combine
void Particle::DrawLaserScan(CMap* map, SDL2Wrapper* sdl, LaserProxy* lp) {
	int scans = lp->GetCount();
	double maxRange = lp->GetMaxRange();
	
	for(int i=0; i<scans; i++) {
		double range = lp->GetRange(i);
		
		if (range < maxRange) {
			
			// Calculate obstacle position
			double bearing = lp->GetBearing(i);
			double dObstacleX = GetX() + range * cos(GetYaw() + bearing);
			double dObstacleY = GetY() + range * sin(GetYaw() + bearing);
			
			// Convert relative obstacle position to our valid map position
			double resolution = CM_TO_METERS(map->GetPixelResolution());
			double mapWidth = map->GetMapWidth();
			double mapHeight = map->GetMapHeight();
			
			dObstacleX = Convert::RobotRelativeXPosToPixelXCoord(dObstacleX, resolution, mapWidth);
			dObstacleY = Convert::RobotRelativeYPosToPixelYCoord(dObstacleY, resolution, mapHeight);
			
			// Check bounds before actually trying to get cell
			if (dObstacleX < 0 || dObstacleX >= map->GetMapWidth() ||
					dObstacleY < 0 || dObstacleY >= map->GetMapHeight()) {
				continue;
			}
			
			SPosition cellPosition = map->PixelCoordToCellPosition(dObstacleX, dObstacleY);
			SMapCell* cell = map->GetMapCell(cellPosition.dwX, cellPosition.dwY);

			if (!cell->fIsPassable) {
				sdl->DrawPoint(dObstacleX, dObstacleY, RED_RGB_FORMAT, 255);
			}
		}
	}
}

void Particle::IncreaseAge() {
	_age++;
}

int Particle::Age() {
	return _age;
}


void Particle::DecreaseLife() {
	_lifes--;
}

bool Particle::IsDead() {
	return _lifes <= 0;
}

