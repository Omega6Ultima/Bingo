//Dustin Gehm

#pragma once
#ifndef _PARTICLE_ENGINE_H
#define _PARTICLE_ENGINE_H

#include "PhysicalObject.h"
#include "Surface.h"
#include "Timer.h"
#include "VecN.h"

using std::vector;

class ParticleEngineBehavior;

class ParticleEngine{
public:
	class Particle : public PhysicalObject{
	public:
		Particle(float velX, float velY, float partLife);
		~Particle();

		inline float getMaxLife() const{
			return maxLife;
		}

		void incLife(float time);
		inline float getLife() const{
			return life;
		}

		void markDead();
		bool isDead(){
			return dead;
		}

	private:
		float maxLife = 0.0f;
		float life = 0.0f;
		bool dead = false;
	};

	ParticleEngine(string partImg, uint maxPart, float partLife, int posX, int posY, float velo);
	ParticleEngine(string partImg, uint maxPart, float partLife, int posX, int posY, float velMinX, float velMaxX, float velMinY, float velMaxY);
	~ParticleEngine();

	virtual Particle* createParticle();
	void remakePartices();

	void setParticleGravity(bool grav);
	inline bool getParticleGravity() const{
		return particleGravity;
	}

	void setParticleBounds(bool bounding, VecN<int, 4> bounds);
	inline VecN<int, 4> getParticleBounds() const{
		return particleBounds;
	}

	inline bool getParticleBounding() const{
		return particleBounding;
	}

	void setBehavior(ParticleEngineBehavior* beh);

	void update();

	void render(Surface& screen);

private:
	VecN<float, 2> pos;
	VecN<float, 2> velMin;
	VecN<float, 2> velMax;
	bool particleGravity = false;
	bool particleBounding = false;
	VecN<int, 4> particleBounds = VecN<int, 4>({-1, -1, -1, -1});
	float maxPartLife = 0;
	Timer timer;
	vector<Particle*> parts;
	Surface partSurf;
	Surface renderSurf;
	ParticleEngineBehavior* behavior = NULL;
	static ParticleEngineBehavior* defaultBehavior;
};

class ParticleEngineBehavior{
public:
	ParticleEngineBehavior();
	~ParticleEngineBehavior();

	virtual void updateParticle(ParticleEngine::Particle* part, float dtime) = 0;

	void setFriction(float fric);
	inline float getFriction() const{
		return 1.0f - frictionCoeff;
	}

protected:
	float frictionCoeff = 0.0f;

private:
	//
};

class DefaultBehavior : public ParticleEngineBehavior{
public:
	DefaultBehavior();
	~DefaultBehavior();

	virtual void updateParticle(ParticleEngine::Particle* part, float dtime) override;

private:
	//
};

class ErraticBehavior : public DefaultBehavior{
public:
	ErraticBehavior();
	~ErraticBehavior();

	virtual void updateParticle(ParticleEngine::Particle* part, float dtime) override;

	void setPhysicsBoost(float boost);
	inline float getPhysicsBoost() const{
		return physicsBoost;
	}

	void setLifeBoost(float boost);
	inline float getLifeBoost() const{
		return lifeBoost;
	}

protected:
	float physicsBoost = 1.5f;
	float lifeBoost = 1.5f;

private:
	//
};

#endif