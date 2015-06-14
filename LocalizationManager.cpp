/* 
 * File:   SlamManager.cpp
 * Author: colman
 * 
 * Created on June 12, 2015, 12:43 PM
 */

#include "LocalizationManager.h"

LocalizationManager::LocalizationManager(CMap* map) {
	_map = map;
	
	_dX = _dY = _dYaw = 0;
}

LocalizationManager::~LocalizationManager() {
	if (_particles.size() > 0) {
		for (int i=0; i<_particles.size(); i++) {
			delete _particles[i];
		}
		
		_particles.clear();
	}
}

void LocalizationManager::Update(double deltaX, double deltaY, double deltaYaw, LaserProxy* lp) {
	vector<Particle*> childsToAdd;
	vector<int> childsToRemove;
	int particlesSize = _particles.size();
	
	for (int i=0; i < particlesSize; i++) {
		
		// Get each particle and update position
		Particle* particle = _particles[i];
		particle->Update(deltaX, deltaY, deltaYaw, _map, lp);
		
		double belif = particle->GetBelif();
		
		// Depending on the belif value kill or breed the given particle
		if (belif <= LOW_BELIF_THRESHOLD) {
			childsToRemove.push_back(i);
		} else if (belif >= HIGH_BELIF_THRESHOLD &&
				((particlesSize + PARTICLE_HIGH_BREED + childsToAdd.size()) < MAX_PARTICLES_COUNT)) {
			BreedParticle(particle, PARTICLE_HIGH_BREED, childsToAdd);
		} else if ((particlesSize + PARTICLE_NORMAL_BREED + childsToAdd.size()) < MAX_PARTICLES_COUNT) {
			BreedParticle(particle, PARTICLE_NORMAL_BREED, childsToAdd);
		}
	}
	
	// Remove the dead particles from the main particles vector
	if (childsToRemove.size() > 0) {
		for(int i = childsToRemove.size() - 1; i>=0; i--) {
			int indexToRemove = childsToRemove[i];
			_particles.erase(_particles.begin() + indexToRemove);
		}
	}
	
	// Add new particles to the main particles vector
	if (childsToAdd.size() > 0) {
		TransferChildsToParticles(childsToAdd);
	}
}

bool LocalizationManager::CreateParticle(double dX, double dY, double dYaw, double dBel) {
	
	// Create new particle only if not exceeded the max particles allowed
	if (_particles.size() < MAX_PARTICLES_COUNT) {
		
		/*
		 * This particle is a special one because we create him without parent
		 * so basically he is the first one and from him we need to breed few
		 * more just to start the natural breeding process.
		 */
		Particle* particle = new Particle(dX, dY, dYaw, dBel);
		_particles.push_back(particle);
		vector<Particle*> childs;
		BreedParticle(particle, PARTICLE_HIGH_BREED, childs);
		TransferChildsToParticles(childs);
		return true;
	}
	
	return false;
}

void LocalizationManager::BreedParticle(Particle* particle, int dwChildCount, vector<Particle*>& childs) {
	/*
	 * We don't want to breed only half a family so we won't breed any child
	 * if their is no more room in our particles vector
	 */
	if (_particles.size() + dwChildCount < MAX_PARTICLES_COUNT) {
		for (int i=0; i<dwChildCount; i++) {
			Particle* child = particle->CreateChild();
			childs.push_back(child);
		}
	}
}

Particle* LocalizationManager::GetBestParticle() {
	
	// Incase no particles found we create new one from last known position
	if (_particles.empty()) {
		CreateParticle(_dX, _dY, _dYaw, 1);
	}
	
	// TODO: This one need a more clever algorithm - I'm tired
	Particle* bestParticle = _particles[0];
	for (int i=1; i<_particles.size(); i++) {
		if (_particles[i]->GetBelif() > bestParticle->GetBelif()) {
			bestParticle = _particles[i];
		}
	}
	
	// Save last known position for the best particle
	_dX = bestParticle->GetX();
	_dY = bestParticle->GetY();
	_dYaw = bestParticle->GetYaw();
	
	return bestParticle;
}

void LocalizationManager::TransferChildsToParticles(vector<Particle*> childs) {
	for (int i=0; i<childs.size(); i++) {
		_particles.push_back(childs[i]);
	}
}