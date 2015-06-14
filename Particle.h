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
	double _dX;
	double _dY;
	double _dYaw;
	double _dBel;
	
	int _lifes;
	int _age;
	
	double Randomize(double dMin, double dMax);
	double ProbabilityByMovement(double deltaX, double deltaY, double deltaYaw);
	double ProbabilityByLaserScan(double dX, double dY, double dYaw, CMap* map, LaserProxy* lp);
	
public:
	Particle(double dX, double dY, double dYaw, double dBel);
	virtual ~Particle();
	
	Particle* CreateChild();
	Particle* CreateChild(double dExpansionRadius);
	void Update(double dX, double dY, double dYaw, CMap* map, LaserProxy* lp);
	void DrawLaserScan(CMap* map, SDL2Wrapper* sdl, LaserProxy* lp);
	
	double GetX();
	double GetY();
	double GetYaw();
	double GetBelif();
	
	void IncreaseAge();
	int Age();
	void DecreaseLife();
	bool IsDead();
	
private:

};

#endif	/* PARTICLE_H */

