//Dustin Gehm

#include "ParticleEngine.h"

#include "Mathematics.h"
#include "RandomManager.h"
#include "Utils.h"
#include "WindowManager.h"

using Bingo::Math::DistanceSq;
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

void Particle::setOOB(size_t ind, bool oob) {
	outOfBounds[ind] = oob;
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

ParticleEngine::ParticleEngine(Surface partSurf, uint maxPart, float partLife, int posX, int posY, float velo)
	: partSurf(partSurf)
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

ParticleEngine::ParticleEngine(Surface partSurf, uint maxPart, float partLife, int posX, int posY, float velMinX, float velMaxX, float velMinY, float velMaxY)
	: partSurf(partSurf), renderSurf(WindowManager::getSingleton().getWidth(), WindowManager::getSingleton().getHeight()) {
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

	if (behavior) {
		part.setFriction(behavior->getFriction());
	}
	else {
		part.setFriction(0.0f);
	}

	return part;
}

void ParticleEngine::remakeParticles() {
	for (auto iter = parts.begin(); iter != parts.end(); iter++) {
		**iter = createParticle();
	}
}

void ParticleEngine::setParticleGravity(bool grav) {
	particleGravity = grav;
}

void ParticleEngine::addExternalForce(int x, int y, uint radius, int forceAmount) {
	addExternalForce({ x, y }, radius, forceAmount);
}

void ParticleEngine::addExternalForce(VecN<int, 2> pos, uint radius, int forcePercent) {
	uint radSq = radius * radius;

	for (Particle* part : parts) {
		if (!part->isDead()) {
			VecN<int, 2> partPos = part->getPos() + getPos();
			float distSq = DistanceSq(partPos, pos);

			if (distSq < radSq) {
				// multiply the difference of positions by a proportion of the distance from pos multiplyed be force as a percentage
				part->addForce(static_cast<VecN<float, 2>>(partPos - pos) * ((radSq - distSq) / radSq) * (forcePercent / 100.0f));
			}
		}
	}
}

void ParticleEngine::update() {
	timer.end();
	timer.start();

	float dtime = ms_to_sec(timer.diff());

	update(dtime);
}

void ParticleEngine::update(float dtime_sec) {
	for (auto iter = parts.begin(); iter != parts.end(); iter++) {
		if (behavior == NULL) {
			defaultBehavior->updateParticle(this, *iter, dtime_sec);
		}
		else {
			behavior->updateParticle(this, *iter, dtime_sec);
		}

		if ((*iter)->isDead()) {
			**iter = createParticle();
		}
	}
}

void ParticleEngine::draw(Surface& screen) {
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

	screen.draw(renderSurf, (int)getPosX() - center[0], (int)getPosY() - center[1]);

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

	//if (particleBounding) {
	//	uint rad = (clipSize[0] / 2 + clipSize[1] / 2) / 2;
	//	part.setScreenBounding(true, rad, particleBounds);
	//}

	return part;
}

void AnimParticleEngine::setAnimSpeed(uint speed) {
	partSurf.setAnimSpeed(speed);
}

void AnimParticleEngine::update() {
	ParticleEngine::update();

	baseFrameCounter = (baseFrameCounter + 1) % (partSurf.getNumFrames() * partSurf.getAnimSpeed());
}

void AnimParticleEngine::draw(Surface& screen) {
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

void ParticleEngineBehavior::setParticleBounds(VecN<int, 5> bounds) {
	particleBounds = bounds;
}

void ParticleEngineBehavior::setBoundBehavior(BoundBehavior behav) {
#if _DEBUG
	if (behav == BoundBehavior::CALLBACK) {
		Utils::Warn("Setting callback behavior with no function!");
	}
#endif
	boundBehav = { behav,behav,behav,behav };
}

void ParticleEngineBehavior::setBoundBehavior(BoundBehavior behav, PartFunc func) {
#if _DEBUG
	if (behav != BoundBehavior::CALLBACK) {
		Utils::Warn("Setting callback function with non-callback behavior!");
	}
#endif
	boundBehav = { behav,behav,behav,behav };
	boundFunc = func;
}

void ParticleEngineBehavior::setBoundBehavior(VecN<BoundBehavior, 4> behav) {
#if _DEBUG
	for (BoundBehavior be : behav) {
		if (be == BoundBehavior::CALLBACK) {
			Utils::Warn("Setting callback behavior with no function!");
		}
	}
#endif
	boundBehav = behav;
}

void ParticleEngineBehavior::setBoundBehavior(VecN<BoundBehavior, 4> behav, PartFunc func) {
#if _DEBUG
	for (BoundBehavior be : behav) {
		if (be != BoundBehavior::CALLBACK) {
			Utils::Warn("Setting callback function with non-callback behavior!");
		}
	}
#endif
	boundBehav = behav;
	boundFunc = func;
}

DefaultBehavior::DefaultBehavior() {
	setFriction(0.0f);
	setPhysicsBoost(1.0f);
	setLifeBoost(1.0f);
}

DefaultBehavior::~DefaultBehavior() {
	//
}

void DefaultBehavior::updateParticle(ParticleEngine* partEng, Particle* part, float dtime) {
	part->update(dtime * physicsBoost);

	if (part->getMaxLife() > 0 && (part->getLife() + dtime > part->getMaxLife() * lifeBoost)) {
		part->markDead();
	}
	else {
		part->incLife(dtime);
	}

	// Check top bound
	if (getParticleBounds()[0] != -1) {
		bool inBound = partEng->getPosY() + part->getPosY() + getParticleBounds()[4] > getParticleBounds()[0];

		if (!part->getOOB(0) && !inBound) {
			part->setOOB(0, true);

			if (boundBehav[0] == BoundBehavior::DIE) {
				part->markDead();
			}
			else if (boundBehav[0] == BoundBehavior::BOUNCE) {
				part->setVelY(-part->getVelY());
			}
			else if (boundBehav[0] == BoundBehavior::CALLBACK) {
				if (boundFunc) {
					boundFunc(*part);
				}
			}
		}

		if (inBound) {
			part->setOOB(0, false);
		}
	}

	// Check bottom bound
	if (getParticleBounds()[1] != -1) {
		bool inBound = partEng->getPosY() + part->getPosY() - getParticleBounds()[4] < getParticleBounds()[1];

		if (!part->getOOB(1) && !inBound) {
			part->setOOB(1, true);

			if (boundBehav[1] == BoundBehavior::DIE) {
				part->markDead();
			}
			else if (boundBehav[1] == BoundBehavior::BOUNCE) {
				part->setVelY(-part->getVelY());
			}
			else if (boundBehav[1] == BoundBehavior::CALLBACK) {
				if (boundFunc) {
					boundFunc(*part);
				}
			}
		}

		if (inBound) {
			part->setOOB(1, false);
		}
	}

	// Check left bound
	if (getParticleBounds()[2] != -1) {
		bool inBound = partEng->getPosX() + part->getPosX() + getParticleBounds()[4] > getParticleBounds()[2];

		if (!part->getOOB(2) && !inBound) {
			part->setOOB(2, true);

			if (boundBehav[2] == BoundBehavior::DIE) {
				part->markDead();
			}
			else if (boundBehav[2] == BoundBehavior::BOUNCE) {
				part->setVelX(-part->getVelX());
			}
			else if (boundBehav[2] == BoundBehavior::CALLBACK) {
				if (boundFunc) {
					boundFunc(*part);
				}
			}
		}

		if (inBound) {
			part->setOOB(2, false);
		}
	}

	// Check right bound
	if (getParticleBounds()[3] != -1) {
		bool inBound = partEng->getPosX() + part->getPosX() - getParticleBounds()[4] < getParticleBounds()[3];

		if (!part->getOOB(3) && !inBound) {
			part->setOOB(3, true);

			if (boundBehav[3] == BoundBehavior::DIE) {
				part->markDead();
			}
			else if (boundBehav[3] == BoundBehavior::BOUNCE) {
				part->setVelX(-part->getVelX());
			}
			else if (boundBehav[3] == BoundBehavior::CALLBACK) {
				if (boundFunc) {
					boundFunc(*part);
				}
			}
		}

		if (inBound) {
			part->setOOB(3, false);
		}
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

void ErraticBehavior::updateParticle(ParticleEngine* partEng, Particle* part, float dtime) {
	VecN<float, 2> vel;

	//part->update(dtime * physicsBoost);

	if (RandomManager::randBool()) {
		vel = part->getVel();

		vel = vel.reverse();
		vel[0] *= RandomManager::randPosNeg();
		vel[1] *= RandomManager::randPosNeg();

		part->setVel(vel);
	}

	DefaultBehavior::updateParticle(partEng, part, dtime);

	//if (part->getMaxLife() > 0 && (part->getLife() + dtime > part->getMaxLife() * lifeBoost)) {
	//	part->markDead();
	//}
	//else {
	//	part->incLife(dtime);
	//}
}

DefaultBehavior defaultParticleBehavior;

ParticleEngineBehavior* ParticleEngine::defaultBehavior = &defaultParticleBehavior;