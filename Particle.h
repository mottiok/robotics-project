/* 
 * File:   Particle.h
 * Author: colman
 *
 * Created on June 12, 2015, 12:43 PM
 */

#ifndef PARTICLE_H
#define	PARTICLE_H

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
using namespace std;

#include "Map.h"
#include "Configuration.h"
#include "Convert.h"
#include "SDL2Wrapper.h"

#include <libplayerc++/playerc++.h>
using namespace PlayerCc;

class Particle {
	float _dX;
	float _dY;
	float _dYaw;
	float _dBel;
	
	int _lifes;
	int _age;
	
	float Randomize(float dMin, float dMax);
	float ProbabilityByMovement(float deltaX, float deltaY, float deltaYaw);
	float ProbabilityByLaserScan(float dX, float dY, float dYaw, CMap* map, SDL2Wrapper* sdl, LaserProxy* lp, bool shouldDraw);
	
public:
	Particle(float dX, float dY, float dYaw, float dBel);
	virtual ~Particle();
	
	Particle* CreateChild();
	Particle* CreateChild(float dExpansionRadius, float dYawRange);
	void Update(float dX, float dY, float dYaw, CMap* map, SDL2Wrapper* sdl, LaserProxy* lp);
	void DrawLaserScan(CMap* map, SDL2Wrapper* sdl, LaserProxy* lp);
	
	float GetX();
	float GetY();
	float GetYaw();
	float GetBelif();
	
	void IncreaseAge();
	int Age();
	void DecreaseLife();
	bool IsDead();
	
private:

};

#endif	/* PARTICLE_H */

