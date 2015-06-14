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
#include <libplayerc++/playerc++.h>
using namespace PlayerCc;

class Particle {
	double _dX;
	double _dY;
	double _dYaw;
	double _dBel;
	
	double Randomize(double dMin, double dMax);
	double ProbabilityByMovement(double deltaX, double deltaY, double deltaYaw);
	double ProbabilityByLaserScan(double dX, double dY, double dYaw, CMap* map, LaserProxy* lp);
	
public:
	Particle(double dX, double dY, double dYaw, double dBel);
	virtual ~Particle();
	
	Particle* CreateChild();
	void Update(double dX, double dY, double dYaw, CMap* map, LaserProxy* lp);
	
	double GetX();
	double GetY();
	double GetYaw();
	double GetBelif();
	
private:

};

#endif	/* PARTICLE_H */

