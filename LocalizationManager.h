/* 
 * File:   SlamManager.h
 * Author: colman
 *
 * Created on June 12, 2015, 12:43 PM
 */

#ifndef LOCALIZATIONMANAGER_H
#define	LOCALIZATIONMANAGER_H

#include <vector>
using namespace std;

#include "typedefs.h"
#include "Particle.h"
#include "Map.h"
#include "Convert.h"
#include "Configuration.h"
#include "Debug.h"

class LocalizationManager {
	CMap* _map;
	vector<Particle*> _particles;
	SDL2Wrapper* _sdl;
	
	/*
	 * This position is the last known best particle, just in case the entire
	 * particles vector is empty we will create new particle from this position.
	 */
	float _dX;
	float _dY;
	float _dYaw;
	
	// Fill in the childs vector with new childs from the parent particle
	void BreedParticle(Particle* particle, int dwChildCount, vector<Particle*>& childs);
	void BreedParticle(Particle* particle, int dwChildCount, float dExpansionRadius, float dYawRange, vector<Particle*>& childs);
	
	// Transfer childs vector to the main particles vector as new future parents
	void TransferChildsToParticles(vector<Particle*> childs);
public:
	LocalizationManager(CMap* map, SDL2Wrapper* sdl);
	virtual ~LocalizationManager();
	
	// Update all particles with new position and breed or kill if necessary
	void Update(float deltaX, float deltaY, float deltaYaw, LaserProxy* lp);
	
	// Get the best particle that represent the actual robot position
	Particle* GetBestParticle();
	
	// Create new particle and breed him with max childs count
	bool CreateParticle(float dX, float dY, float dYaw, float dBel);
	bool CreateParticle(float dX, float dY, float dYaw, float dBel, float dExpansionRadius, float dYawRange, int childsCount);
private:

};

#endif	/* LOCALIZATIONMANAGER_H */

