//Dustin Gehm

#ifndef _PHYSICALOBJECT_H
#define _PHYSICALOBJECT_H

#include "Positional.h"
#include "VecN.h"

namespace Bingo {

	namespace Math {

		class PhysicalObject : virtual public Positional {
		public:
			PhysicalObject();
			PhysicalObject(VecN<int, 2> p);
			PhysicalObject(VecN<int, 2> p, VecN<float, 2> v);
			PhysicalObject(VecN<int, 2> p, VecN<float, 2> v, VecN<float, 2> a);
			~PhysicalObject();

			void setVel(VecN<float, 2> v);
			inline VecN<float, 2> getVel() const {
				return vel;
			}

			void setVelX(float x);
			inline float getVelX() const {
				return vel[0];
			}

			void setVelY(float y);
			inline float getVelY() const {
				return vel[1];
			}

			void setAcc(VecN<float, 2> a);
			inline VecN<float, 2> getAcc() const {
				return acc;
			}

			void setAccX(float x);
			inline float getAccX() const {
				return acc[0];
			}

			void setAccY(float y);
			inline float getAccY() const {
				return acc[1];
			}

			void addForce(VecN<float, 2> f);
			void addForceX(float x);
			void addForceY(float y);

			void setFriction(float fric);
			inline float getFriction() const {
				return 1.0f - frictionCoeff;
			}

			void setGravityEffects(bool grav);
			inline bool getGravityEffects() const {
				return gravityEffects;
			}

			void setGravityStr(float grav);
			inline float getGravityStr() const {
				return gravityStr;
			}

			// TODO set the format to the same as Particles
			//bounds is in the format (xmin, ymin, xmax, ymax)
			void setScreenBounding(bool bound, int rad, VecN<int, 4> bounds);
			inline bool getScreenBounding() const {
				return screenBounding;
			}

			inline int getRadius() const {
				return radius;
			}

			inline VecN<int, 4> getScreenBounds() const {
				return screenBounds;
			}

			void update(float dtime);

		private:
			VecN<float, 2> vel = VecN<float, 2>({ 0.0f, 0.0f });
			VecN<float, 2> acc = VecN<float, 2>({ 0.0f, 0.0f });
			float frictionCoeff = .95f;
			bool gravityEffects = false;
			float gravityStr = .98f;
			bool screenBounding = false;
			int radius = 0;
			VecN<int, 4> screenBounds = VecN<int, 4>({ -1, -1, -1, -1 });
			VecN<int, 4> objectBounds = VecN<int, 4>({ -1, -1, -1, -1 });
		};

	}

}

#endif