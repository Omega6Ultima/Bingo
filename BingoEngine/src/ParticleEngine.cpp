//Dustin Gehm

#include "ParticleEngine.h"

#include "RandomManager.h"
#include "WindowManager.h"

using Bingo::Particles::AnimParticleEngine;
using Bingo::Particles::DefaultBehavior;
using Bingo::Particles::ErraticBehavior;
using Bingo::Particles::Particle;
using Bingo::Particles::ParticleEngine;
using Bingo::Particles::ParticleEngineBehavior;
using Bingo::Surfaces::WindowManager;
using Bingo::TimeConvert::ms_to_sec;

Particle::Particle(float velX, float velY, float partLife)
	:PhysicalObject({ 0, 0 }, { velX, velY }) {
	maxLife = partLife;
}

Particle::~Particle() {
	//
}

void Particle::incLife(float time) {
	life += time;
}

void Particle::markDead() {
	dead = true;
}

ParticleEngine::ParticleEngine(string partImg, uint maxPart, float partLife, int posX, int posY, float velo)
	: partSurf(partImg)
	, renderSurf(WindowManager::getSingleton().getWidth(), WindowManager::getSingleton().getHeight()) {
	pos = { static_cast<float>(posX), static_cast<float>(posY) };
	velMin = { -velo, -velo };
	velMax = { velo, velo };
	maxPartLife = partLife;

	parts.resize(maxPart);

	for (uint c = 0; c < maxPart; c++) {
		parts[c] = new Particle(0.0f, 0.0f, 0.0f);
		*parts[c] = createParticle();
	}

	//enables transparency
	renderSurf.setBlendMode(Surface::BLEND_BLEND);
}

ParticleEngine::ParticleEngine(string partImg, uint maxPart, float partLife, int posX, int posY, float velMinX, float velMaxX, float velMinY, float velMaxY)
	: partSurf(partImg), renderSurf(WindowManager::getSingleton().getWidth(), WindowManager::getSingleton().getHeight()) {
	pos = { static_cast<float>(posX), static_cast<float>(posY) };
	velMin = { velMinX, velMinY };
	velMax = { velMaxX, velMaxY };
	maxPartLife = partLife;

	parts.resize(maxPart);

	for (uint c = 0; c < maxPart; c++) {
		parts[c] = new Particle(0.0f, 0.0f, 0.0f);
		*parts[c] = createParticle();
	}

	//enables transparency
	renderSurf.setBlendMode(Surface::BLEND_BLEND);
}

ParticleEngine::~ParticleEngine() {
	for (auto iter = parts.begin(); iter != parts.end(); iter++) {
		delete (*iter);
	}
}

Particle ParticleEngine::createParticle() {
	Particle part(RandomManager::randFloat(velMin[0], velMax[0]),
		RandomManager::randFloat(velMin[1], velMax[1]),
		RandomManager::randFloat(0, maxPartLife));
	part.setGravityEffects(particleGravity);

	if (particleBounding) {
		uint rad = (partSurf.getHeight() / 2 + partSurf.getWidth() / 2) / 2;
		part.setScreenBounding(true, rad, particleBounds);
	}

	if (behavior) {
		part.setFriction(behavior->getFriction());
	}
	else {
		part.setFriction(0.0f);
	}

	return part;
}

void ParticleEngine::remakePartices() {
	for (auto iter = parts.begin(); iter != parts.end(); iter++) {
		**iter = createParticle();
	}
}

void ParticleEngine::setParticleGravity(bool grav) {
	particleGravity = grav;
}

void ParticleEngine::setParticleBounds(bool bounding, VecN<int, 4> bounds) {
	particleBounding = bounding;
	particleBounds = bounds;
}

void ParticleEngine::update() {
	timer.end();
	timer.start();

	float dtime = ms_to_sec(timer.diff());

	for (auto iter = parts.begin(); iter != parts.end(); iter++) {
		if (behavior == NULL) {
			defaultBehavior->updateParticle(*iter, dtime);
		}
		else {
			behavior->updateParticle(*iter, dtime);
		}

		if ((*iter)->isDead()) {
			**iter = createParticle();
		}
	}
}

void ParticleEngine::render(Surface& screen) {
	renderSurf.saveRenderTarget();

	renderSurf.setRenderTarget();

	renderSurf.clear();

	VecN<int, 2> center = renderSurf.getCenter();

	for (auto iter = parts.begin(); iter != parts.end(); iter++) {
		VecN<float, 2> partOff = (*iter)->getPos();
		renderSurf.draw(partSurf, static_cast<int>(center[0] + partOff[0]), static_cast<int>(center[1] + partOff[1]));
	}

	renderSurf.clearRenderTarget();
	screen.setRenderTarget();

	screen.draw(renderSurf, (int)pos[0] - center[0], (int)pos[1] - center[1]);

	renderSurf.restoreRenderTarget();
}

void ParticleEngine::setBehavior(ParticleEngineBehavior* beh) {
	behavior = beh;
}

AnimParticleEngine::AnimParticleEngine(string partImg, vector<VecN<int, 4>>& clips, uint maxPart, float partLife, int posX, int posY, float velo)
	: ParticleEngine(partImg, maxPart, partLife, posX, posY, velo)
	, partSurf(partImg) {

	for (auto iter = clips.begin(); iter != clips.end(); iter++) {
		partSurf.addClip((*iter)[0], (*iter)[1], (*iter)[2], (*iter)[3]);
	}

	clipSize[0] = clips[0][2];
	clipSize[1] = clips[0][3];

	for (auto iter = parts.begin(); iter != parts.end(); iter++) {
		clipOffsets.push_back(RandomManager::getSingleton().randInt(0, clips.size() - 1));
	}
}

AnimParticleEngine::AnimParticleEngine(string partImg, vector<VecN<int, 4>>& clips, uint maxPart, float partLife, int posX, int posY, float velMinX, float velMaxX, float velMinY, float velMaxY)
	: ParticleEngine(partImg, maxPart, partLife, posX, posY, velMinX, velMaxX, velMinY, velMaxY)
	, partSurf(partImg) {

	for (auto iter = clips.begin(); iter != clips.end(); iter++) {
		partSurf.addClip((*iter)[0], (*iter)[1], (*iter)[2], (*iter)[3]);
	}

	for (auto iter = parts.begin(); iter != parts.end(); iter++) {
		clipOffsets.push_back(RandomManager::getSingleton().randInt(0, clips.size() - 1));
	}
}

AnimParticleEngine::~AnimParticleEngine() {
	//
}

Particle AnimParticleEngine::createParticle() {
	Particle part = ParticleEngine::createParticle();

	if (particleBounding) {
		uint rad = (clipSize[0] / 2 + clipSize[1] / 2) / 2;
		part.setScreenBounding(true, rad, particleBounds);
	}

	return part;
}

void AnimParticleEngine::setAnimSpeed(uint speed) {
	partSurf.setAnimSpeed(speed);
}

void AnimParticleEngine::update() {
	ParticleEngine::update();

	baseFrameCounter = (baseFrameCounter + 1) % (partSurf.getNumFrames() * partSurf.getAnimSpeed());
}

void AnimParticleEngine::render(Surface& screen) {
	renderSurf.saveRenderTarget();

	renderSurf.setRenderTarget();

	renderSurf.clear();

	VecN<int, 2> center = renderSurf.getCenter();
	uint baseFrame = baseFrameCounter / partSurf.getAnimSpeed();

	for (uint c = 0; c < parts.size(); c++) {
		VecN<float, 2> partOff = parts[c]->getPos();
		uint clip = (clipOffsets[c] + baseFrame) % partSurf.getNumFrames();

		renderSurf.drawClip(partSurf, static_cast<int>(center[0] + partOff[0]), static_cast<int>(center[1] + partOff[1]), clip);
	}

	renderSurf.clearRenderTarget();
	screen.setRenderTarget();

	screen.draw(renderSurf, static_cast<int>(pos[0]) - center[0], static_cast<int>(pos[1]) - center[1]);

	renderSurf.restoreRenderTarget();
}

ParticleEngineBehavior::ParticleEngineBehavior() {
	//
}

ParticleEngineBehavior::~ParticleEngineBehavior() {
	//
}

void ParticleEngineBehavior::setFriction(float fric) {
	frictionCoeff = 1.0f - fric;
}

void ParticleEngineBehavior::setPhysicsBoost(float boost) {
	physicsBoost = boost;
}

void ParticleEngineBehavior::setLifeBoost(float boost) {
	lifeBoost = boost;
}

DefaultBehavior::DefaultBehavior() {
	setFriction(0.0f);
	setPhysicsBoost(1.0f);
	setLifeBoost(1.0f);
}

DefaultBehavior::~DefaultBehavior() {
	//
}

void DefaultBehavior::updateParticle(Particle* part, float dtime) {
	part->update(dtime * physicsBoost);

	if (part->getLife() + dtime > part->getMaxLife() * lifeBoost) {
		part->markDead();
	}
	else {
		part->incLife(dtime);
	}
}

ErraticBehavior::ErraticBehavior() {
	setFriction(0.0f);
	setPhysicsBoost(1.5f);
	setLifeBoost(1.5f);
}

ErraticBehavior::~ErraticBehavior() {
	//
}

void ErraticBehavior::updateParticle(Particle* part, float dtime) {
	VecN<float, 2> vel;

	part->update(dtime * physicsBoost);

	if (RandomManager::randBool()) {
		vel = part->getVel();

		vel = vel.reverse();
		vel[0] *= RandomManager::randPosNeg();
		vel[1] *= RandomManager::randPosNeg();

		part->setVel(vel);
	}

	if (part->getLife() + dtime > part->getMaxLife() * lifeBoost) {
		part->markDead();
	}
	else {
		part->incLife(dtime);
	}
}

DefaultBehavior defaultParticleBehavior;

ParticleEngineBehavior* ParticleEngine::defaultBehavior = &defaultParticleBehavior;