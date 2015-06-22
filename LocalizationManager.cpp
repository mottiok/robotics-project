/* 
 * File:   SlamManager.cpp
 * Author: colman
 * 
 * Created on June 12, 2015, 12:43 PM
 */

#include "LocalizationManager.h"
#include <ctime>

LocalizationManager::LocalizationManager(CMap* map, SDL2Wrapper* sdl) {
	_map = map;
	_sdl = sdl;
	
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

void LocalizationManager::Update(float deltaX, float deltaY, float deltaYaw, LaserProxy* lp) {		
	vector<Particle*> childsToAdd;
	vector<int> childsToRemove;
	int particlesSize = _particles.size();
	
	for (int i=0; i < particlesSize; i++) {
		
		// Get each particle and update position
		Particle* particle = _particles[i];
		particle->Update(deltaX, deltaY, deltaYaw, _map, _sdl, lp);
		
		float belif = particle->GetBelif();
		
		// Depending on the belif value kill or breed the given particle
		if (belif <= LOW_BELIF_THRESHOLD) {
			particle->DecreaseLife();
			if (particle->IsDead() || belif <= 0) {
				childsToRemove.push_back(i);				
			}
		} else if (belif >= HIGH_BELIF_THRESHOLD &&
				((particlesSize + PARTICLE_HIGH_BREED + childsToAdd.size()) < MAX_PARTICLES_COUNT)) {
			particle->IncreaseAge();
			BreedParticle(particle, PARTICLE_HIGH_BREED, childsToAdd);
		} else if ((particlesSize + PARTICLE_NORMAL_BREED + childsToAdd.size()) < MAX_PARTICLES_COUNT) {
			particle->IncreaseAge();
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
	
	Particle* bestParticle;
#ifdef DRAW_PARTICLES
	bestParticle = GetBestParticle();
	for (int i=0; i<_particles.size(); i++) {
		Particle* particle = _particles[i];
		float dX = Convert::RobotRelativeXPosToPixelXCoord(particle->GetX(), CM_TO_METERS(_map->GetPixelResolution()), _map->GetMapWidth());
		float dY = Convert::RobotRelativeYPosToPixelYCoord(particle->GetY(), CM_TO_METERS(_map->GetPixelResolution()), _map->GetMapHeight());
		if (particle->GetX() == bestParticle->GetX() &&
				particle->GetY() == bestParticle->GetY() &&
				particle->GetYaw() == bestParticle->GetYaw() &&
				particle->GetBelif() == bestParticle->GetBelif()) {
			continue;
		}
		
//		_sdl->FillRectangle(dX, dY, (double)(_map->GetGridResolution() / _map->GetPixelResolution()), PURPLE_RGB_FORMAT, 255, false);
		_sdl->DrawPoint(dX, dY, PURPLE_RGB_FORMAT, 255);
	}
	
	float dX = Convert::RobotRelativeXPosToPixelXCoord(bestParticle->GetX(), CM_TO_METERS(_map->GetPixelResolution()), _map->GetMapWidth());
	float dY = Convert::RobotRelativeYPosToPixelYCoord(bestParticle->GetY(), CM_TO_METERS(_map->GetPixelResolution()), _map->GetMapHeight());
	_sdl->FillRectangle(dX, dY, (double)(_map->GetGridResolution() / _map->GetPixelResolution()), RED_RGB_FORMAT, 255, false);
#endif
	
#ifdef DRAW_BEST_LASER_SCAN
	bestParticle = GetBestParticle();
	bestParticle->DrawLaserScan(_map, _sdl, lp);
#endif
}

bool LocalizationManager::CreateParticle(float dX, float dY, float dYaw, float dBel) {
	CreateParticle(dX, dY, dYaw, dBel, EXPANSION_RADIUS, YAW_RANGE, PARTICLE_HIGH_BREED);
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

void LocalizationManager::BreedParticle(Particle* particle, int dwChildCount, float dExpansionRadius, float dYawRange, vector<Particle*>& childs) {
	if (_particles.size() + dwChildCount < MAX_PARTICLES_COUNT) {
		for (int i=0; i<dwChildCount; i++) {
			Particle* child = particle->CreateChild(dExpansionRadius, dYawRange);
			childs.push_back(child);
		}
	}
}

Particle* LocalizationManager::GetBestParticle() {
	
	// Incase no particles found we create new one from last known position
	if (_particles.empty()) {
		CreateParticle(_dX, _dY, _dYaw, 1, EMERGENCY_EXPANSION_RADIUS, EMERGENCY_YAW_RANGE,  PARTICLE_EMERGENCY_BREED);
		Particle* randomParticle = _particles[rand() % _particles.size()];
		_dX = randomParticle->GetX();
		_dY = randomParticle->GetY();
		_dYaw = randomParticle->GetYaw();
		return randomParticle;
	}
	
	Particle* bestParticle = _particles[0];
	for (int i=1; i<_particles.size(); i++) {
		if (_particles[i]->GetBelif() > bestParticle->GetBelif() &&
				_particles[i]->Age() < bestParticle->Age()) {
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

bool LocalizationManager::CreateParticle(float dX, float dY, float dYaw, float dBel, float dExpansionRadius, float dYawRange, int childsCount) {
	// Create new particle only if not exceeded the max particles allowed
	if (_particles.size() + childsCount < MAX_PARTICLES_COUNT) {
		
		/*
		 * This particle is a special one because we create him without parent
		 * so basically he is the first one and from him we need to breed few
		 * more just to start the natural breeding process.
		 */
		Particle* particle = new Particle(dX, dY, dYaw, dBel);
		_particles.push_back(particle);
		vector<Particle*> childs;
		BreedParticle(particle, childsCount, dExpansionRadius, dYawRange, childs);
		TransferChildsToParticles(childs);
		return true;
	}
	
	return false;
}
