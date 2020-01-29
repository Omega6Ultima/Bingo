//Dustin Gehm

#include "ParticleEngine.h"

#include "RandomManager.h"
#include "WindowManager.h"

ParticleEngine::ParticleEngine(string partImg, uint maxPart, float partLife, int posX, int posY, float velo)
: partSurf(partImg), renderSurf(WindowManager::getSingleton().getWidth(), WindowManager::getSingleton().getHeight()){
	pos = { static_cast<float>(posX), static_cast<float>(posY) };
	velMin = { -velo, -velo };
	velMax = { velo, velo };
	maxPartLife = partLife;

	parts.resize(maxPart);

	for (uint c = 0; c < maxPart; c++){
		parts[c] = createParticle();
	}

	//enables transparency
	renderSurf.setBlendMode(Surface::BLEND_BLEND);
}

ParticleEngine::ParticleEngine(string partImg, uint maxPart, float partLife, int posX, int posY, float velMinX, float velMaxX, float velMinY, float velMaxY)
: partSurf(partImg), renderSurf(WindowManager::getSingleton().getWidth(), WindowManager::getSingleton().getHeight()){
	pos = { static_cast<float>(posX), static_cast<float>(posY) };
	velMin = { velMinX, velMinY };
	velMax = { velMaxX, velMaxY };
	maxPartLife = partLife;

	parts.resize(maxPart);

	for (uint c = 0; c < maxPart; c++){
		parts[c] = createParticle();
	}

	//enables transparency
	renderSurf.setBlendMode(Surface::BLEND_BLEND);
}

ParticleEngine::~ParticleEngine(){
	for (auto iter = parts.begin(); iter != parts.end(); iter++){
		delete (*iter);
	}
}

ParticleEngine::Particle* ParticleEngine::createParticle(){
	Particle* part = new Particle(RandomManager::randFloat(velMin[0], velMax[0]),
									RandomManager::randFloat(velMin[1], velMax[1]),
									RandomManager::randFloat(0, maxPartLife));
	part->setGravityEffects(particleGravity);

	if (particleBounding){
		uint rad = (partSurf.getHeight() / 2 + partSurf.getWidth() / 2) / 2;
		part->setScreenBounding(true, rad, particleBounds);
	}

	if (behavior){
		part->setFriction(behavior->getFriction());
	}
	else{
		part->setFriction(0.0f);
	}

	return part;
}

void ParticleEngine::remakePartices(){
	for (auto iter = parts.begin(); iter != parts.end(); iter++){
		delete (*iter);

		(*iter) = createParticle();
	}
}

void ParticleEngine::setParticleGravity(bool grav){
	particleGravity = grav;
}

void ParticleEngine::setParticleBounds(bool bounding, VecN<int, 4> bounds){
	particleBounding = bounding;
	particleBounds = bounds;
}

void ParticleEngine::update(){
	timer.end();
	timer.start();

	float dtime = Timer::ms_to_sec(timer.diff());

	for (auto iter = parts.begin(); iter != parts.end(); iter++){
		if (behavior == NULL){
			defaultBehavior->updateParticle(*iter, dtime);
		}
		else{
			behavior->updateParticle(*iter, dtime);
		}

		if ((*iter)->isDead()){
			delete (*iter);

			(*iter) = createParticle();
		}
	}
}

void ParticleEngine::render(Surface& screen){
	renderSurf.setRenderTarget();

	renderSurf.clear();

	VecN<int, 2> center = renderSurf.getCenter();

	for (auto iter = parts.begin(); iter != parts.end(); iter++){
		VecN<float, 2> partOff = (*iter)->getPos();
		renderSurf.draw(partSurf, (int)(center[0] + partOff[0]), (int)(center[1] + partOff[1]));
	}

	renderSurf.clearRenderTarget();
	screen.setRenderTarget();

	screen.draw(renderSurf, (int)pos[0] - center[0], (int)pos[1] - center[1]);
}

void ParticleEngine::setBehavior(ParticleEngineBehavior* beh){
	behavior = beh;
}

ParticleEngine::Particle::Particle(float velX, float velY, float partLife)
:PhysicalObject({0, 0}, { velX, velY }){
	maxLife = partLife;
}

ParticleEngine::Particle::~Particle(){
	//
}

void ParticleEngine::Particle::incLife(float time){
	life += time;
}

void ParticleEngine::Particle::markDead(){
	dead = true;
}

ParticleEngineBehavior::ParticleEngineBehavior(){
	//
}

ParticleEngineBehavior::~ParticleEngineBehavior(){
	//
}

void ParticleEngineBehavior::setFriction(float fric){
	frictionCoeff = 1.0f - fric;
}

DefaultBehavior::DefaultBehavior(){
	//
}

DefaultBehavior::~DefaultBehavior(){
	//
}

void DefaultBehavior::updateParticle(ParticleEngine::Particle* part, float dtime){
	part->update(dtime);

	if (part->getLife() + dtime > part->getMaxLife()){
		part->markDead();
	}
	else{
		part->incLife(dtime);
	}
}

ErraticBehavior::ErraticBehavior(){
	//
}

ErraticBehavior::~ErraticBehavior(){
	//
}

void ErraticBehavior::updateParticle(ParticleEngine::Particle* part, float dtime){
	VecN<float, 2> vel;

	part->update(dtime * physicsBoost);

	if (RandomManager::randBool()){
		vel = part->getVel();

		vel = vel.reverse();
		vel[0] *= RandomManager::randPosNeg();
		vel[1] *= RandomManager::randPosNeg();

		part->setVel(vel);
	}

	if (part->getLife() + dtime > part->getMaxLife() * lifeBoost){
		part->markDead();
	}
	else{
		part->incLife(dtime);
	}
}

void ErraticBehavior::setPhysicsBoost(float boost){
	physicsBoost = boost;
}

void ErraticBehavior::setLifeBoost(float boost){
	lifeBoost = boost;
}

DefaultBehavior defaultParticleBehavior;

ParticleEngineBehavior* ParticleEngine::defaultBehavior = &defaultParticleBehavior;