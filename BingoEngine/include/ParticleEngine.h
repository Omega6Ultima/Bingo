//Dustin Gehm

#pragma once
#ifndef _PARTICLE_ENGINE_H
#define _PARTICLE_ENGINE_H

#include "AnimSurface.h"
#include "PhysicalObject.h"
#include "Positional.h"
#include "Surface.h"
#include "Timer.h"
#include "VecN.h"

using std::vector;

namespace Bingo {

	namespace Particles {

		using Math::PhysicalObject;
		using Math::Positional;
		using Math::VecN;
		using Surfaces::AnimSurface;
		using Surfaces::Surface;
		using Time::Timer;

		class Particle : public PhysicalObject {
		public:
			Particle(float velX, float velY, float partLife);
			~Particle();

			inline float getMaxLife() const {
				return maxLife;
			}

			void incLife(float time);
			inline float getLife() const {
				return life;
			}

			void markDead();
			bool isDead() const {
				return dead;
			}

			void setOOB(size_t ind, bool oob);
			bool getOOB(size_t ind) const {
				return outOfBounds[ind];
			}

		private:
			float maxLife = 0.0f;
			float life = 0.0f;
			bool dead = false;
			VecN<bool, 4> outOfBounds = VecN<bool, 4>({ false, false, false, false });
		};

		class ParticleEngineBehavior;

		class ParticleEngine : public Positional {
		public:
			ParticleEngine(string partImg, uint maxPart, float partLife, int posX, int posY, float velo);
			ParticleEngine(string partImg, uint maxPart, float partLife, int posX, int posY, float velMinX, float velMaxX, float velMinY, float velMaxY);
			ParticleEngine(Surface partSurf, uint maxPart, float partLife, int posX, int posY, float velo);
			ParticleEngine(Surface partSurf, uint maxPart, float partLife, int posX, int posY, float velMinX, float velMaxX, float velMinY, float velMaxY);
			~ParticleEngine();

			void remakeParticles();

			void setParticleGravity(bool grav);
			inline bool getParticleGravity() const {
				return particleGravity;
			}

			void setBehavior(ParticleEngineBehavior* beh);

			inline Surface& getParticleSurface() {
				return partSurf;
			}

			void addExternalForce(int x, int y, uint radius, float forceAmount);
			void addExternalForce(VecN<int, 2> pos, uint radius, float forcePercent);

			virtual void update();
			virtual void update(float dtime_sec);

			virtual void draw(Surface& screen);

		protected:
			virtual Particle createParticle();

		protected:
			//VecN<float, 2> pos;
			VecN<float, 2> velMin;
			VecN<float, 2> velMax;
			bool particleGravity = false;
			float maxPartLife = 0;
			Timer timer;
			vector<Particle*> parts;
			Surface partSurf;
			Surface renderSurf;
			ParticleEngineBehavior* behavior = NULL;
			static ParticleEngineBehavior* defaultBehavior;
		};

		class AnimParticleEngine :public ParticleEngine {
		public:
			AnimParticleEngine(string partImg, vector<VecN<int, 4>>& clips, uint maxPart, float partLife, int posX, int posY, float velo);
			AnimParticleEngine(string partImg, vector<VecN<int, 4>>& clips, uint maxPart, float partLife, int posX, int posY, float velMinX, float velMaxX, float velMinY, float velMaxY);
			~AnimParticleEngine();

			void setAnimSpeed(uint speed);
			inline uint getAnimSpeed() const {
				return partSurf.getAnimSpeed();
			}

			void update() override;

			void draw(Surface& screen) override;

		protected:
			virtual Particle createParticle() override;

		protected:
			AnimSurface partSurf;
			VecN<int, 2> clipSize;
			vector<uint> clipOffsets;
			uint baseFrameCounter = 0;
		};

		class ParticleEngineBehavior {
		public:
			enum class BoundBehavior {
				DIE,
				BOUNCE,
				CALLBACK,
			};

			typedef void(*PartFunc)(Particle& part);

			ParticleEngineBehavior();
			~ParticleEngineBehavior();

			virtual void updateParticle(ParticleEngine* partEng, Particle* part, float dtime) = 0;

			void setFriction(float fric);
			inline float getFriction() const {
				return 1.0f - frictionCoeff;
			}

			void setPhysicsBoost(float boost);
			inline float getPhysicsBoost() const {
				return physicsBoost;
			}

			void setLifeBoost(float boost);
			inline float getLifeBoost() const {
				return lifeBoost;
			}

			// Values are top, bottom, left, right bounds, and radius
			void setParticleBounds(VecN<int, 5> bounds);
			inline VecN<int, 5> getParticleBounds() const {
				return particleBounds;
			}

			void setBoundBehavior(BoundBehavior behav);
			void setBoundBehavior(BoundBehavior behav, PartFunc func);
			void setBoundBehavior(VecN<BoundBehavior, 4> behav);
			void setBoundBehavior(VecN<BoundBehavior, 4> behav, PartFunc func);
			inline VecN<BoundBehavior, 4> getBoundBehavior() const {
				return boundBehav;
			}

		protected:
			float frictionCoeff = 0.0f;
			float physicsBoost = 1.0f;
			float lifeBoost = 1.0f;
			VecN<int, 5> particleBounds = { -1, -1, -1, -1, 1 };
			VecN<BoundBehavior, 4> boundBehav = { BoundBehavior::DIE, BoundBehavior::DIE, BoundBehavior::DIE, BoundBehavior::DIE };
			PartFunc boundFunc = NULL;

		private:
			//
		};

		class DefaultBehavior : public ParticleEngineBehavior {
		public:
			DefaultBehavior();
			~DefaultBehavior();

			virtual void updateParticle(ParticleEngine* partEng, Particle* part, float dtime) override;

		private:
			//
		};

		class ErraticBehavior : public DefaultBehavior {
		public:
			ErraticBehavior();
			~ErraticBehavior();

			virtual void updateParticle(ParticleEngine* partEng, Particle* part, float dtime) override;

		private:
			//
		};

	}

}

#endif