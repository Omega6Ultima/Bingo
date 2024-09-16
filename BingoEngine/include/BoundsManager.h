//Dustin Gehm

#pragma once
#ifndef _BOUNDMANAGER_H
#define _BOUNDMANAGER_H

#include <map>
#include <vector>

#include <SDL_rect.h>

#include "Color.h"
#include "Core.h"
#include "CompassDirection.h"
#include "PhysicalObject.h"
#include "Singleton.h"
#include "Surface.h"
#include "Timer.h"
#include "VecN.h"

using std::map;
using std::pair;
using std::vector;

#define DEBUG_DRAWS_ROUGH 1
#define DEBUG_DRAWS_FINE 1

namespace Bingo {

	using Core::Manager;

	namespace Collision {

		class BoundsBase;
		class BoundsLine;

	}

	class BoundsManager : public Singleton<BoundsManager>, public Manager {
	public:
		enum BoundsType {
			CIRCLE = 0,
			LINE,
			BOX
		};

		BoundsManager();
		~BoundsManager();

		void addBounds(Collision::BoundsBase* b);

		void update();

		void draw(Surfaces::Surface& surf);

	private:
		void handleCollision(Collision::BoundsBase* base, Math::VecN<float, 2> normal);

		//order is Left, Bottom, Right, Top
		Collision::BoundsLine* screenEdges[4];
		vector<Collision::BoundsBase*> bounds;
		map<pair<int, int>, uint> cooldowns;
		Time::Timer timer;
		const uint cooldown = 100;
	};

	namespace Collision {

		using Colors::Color;
		using Direction::CompassDirection;
		using Math::PhysicalObject;
		using Math::VecN;
		using Surfaces::Surface;

		typedef void(*BoundFunc)();

		class BoundsBase : public PhysicalObject {
		public:
			BoundsBase(VecN<int, 2> pos, Color col);
			~BoundsBase();

			void setReflectStr(float str);
			inline float getReflectStr() const {
				return reflectStr;
			}

			void setReflectScreenEdge(bool reflect);
			inline bool getReflectScreenEdge() const {
				return reflectScreenEdge;
			}

			virtual VecN<int, 4> getRoughArea() const = 0;
			virtual VecN<int, 2> getClosestPoint(VecN<int, 2> p) = 0;
			virtual VecN<float, 2> getNormal(VecN<int, 2> p, CompassDirection& dir) = 0;
			virtual BoundsManager::BoundsType getType() const = 0;
			virtual VecN<int, 2> getCenter() const = 0;
			virtual void draw(Surface& surf, bool filled = true) const = 0;

			BoundFunc onCollide = NULL;

		protected:
			float reflectStr = .1f;
			bool reflectScreenEdge = true;
			Color color;

		private:
			friend class BoundsManager;
		};

		class BoundsCircle : public BoundsBase {
		public:
			BoundsCircle(VecN<int, 2> pos, uint radius, Color col);
			~BoundsCircle();

			void setRad(uint radius);
			inline uint getRad() const {
				return rad;
			}

			virtual VecN<int, 4> getRoughArea() const override;
			virtual VecN<int, 2> getClosestPoint(VecN<int, 2> p) override;
			virtual VecN<float, 2> getNormal(VecN<int, 2> p, CompassDirection& dir) override;
			virtual BoundsManager::BoundsType getType() const override;
			virtual VecN<int, 2> getCenter() const override;
			virtual void draw(Surface& surf, bool filled = true) const override;

		private:
			uint rad;
		};

		class BoundsLine : public BoundsBase {
		public:
			BoundsLine(VecN<int, 2> pos, VecN<int, 2> pos2, Color col);
			~BoundsLine();

			virtual VecN<int, 4> getRoughArea() const override;
			virtual VecN<int, 2> getClosestPoint(VecN<int, 2> p) override;
			virtual VecN<float, 2> getNormal(VecN<int, 2> p, CompassDirection& dir) override;
			virtual BoundsManager::BoundsType getType() const override;
			virtual VecN<int, 2> getCenter() const override;
			virtual void draw(Surface& surf, bool filled = true) const override;

			inline VecN<int, 2> getPos2() const {
				return pos2;
			}

		private:
			VecN<int, 2> pos2;
		};

		class BoundsBox : public BoundsBase {
		public:
			//pos is the upper-right hand corner of the box
			BoundsBox(VecN<int, 2> pos, int w, int h, Color col);
			~BoundsBox();

			virtual VecN<int, 4> getRoughArea() const override;
			virtual VecN<int, 2> getClosestPoint(VecN<int, 2> p) override;
			virtual VecN<float, 2> getNormal(VecN<int, 2> p, CompassDirection& dir) override;
			virtual BoundsManager::BoundsType getType() const override;
			virtual VecN<int, 2> getCenter() const override;
			virtual void draw(Surface& surf, bool filled = true) const override;

			inline int getWidth() const {
				return width;
			}

			inline int getHeight() const {
				return height;
			}

		private:
			int width;
			int height;
			VecN<VecN<int, 2>, 4> corners;
			VecN<int, 2> center;
		};

	}

}
#endif