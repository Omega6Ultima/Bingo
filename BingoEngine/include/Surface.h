//Dustin Gehm
//A renderable surface
/*a class used to make a surface to draw to and be drawn to*/

#pragma once
#ifndef _SURFACE_H
#define _SURFACE_H

#include <string>
#include <vector>

#include <SDL_rect.h>
#include <SDL_render.h>

#include "Color.h"
#include "Exception.h"
#include "Positional.h"
#include "VecN.h"

#define MAX_SAVED_TARGETS 20

using std::vector;

namespace Bingo {

	namespace Surfaces {

		//using Surfaces::WindowManager;

		using Colors::Color;
		using Math::Positional;
		using Math::VecN;

		class AnimSurface;

		class Surface : virtual public Positional {
		public:
			enum BlendMode { BLEND_NONE, BLEND_BLEND, BLEND_ADD, BLEND_MOD = 4 };
			enum FlipMode { FLIP_NONE, FLIP_HORIZONTAL, FLIP_VERTICAL };

			Surface(string path);
			Surface(string path, Color colorkey);
			Surface(int width, int height);
			Surface(const Surface& surf);
			Surface& operator =(const Surface& surf);
			~Surface();

			//private:
			//	/*contains code to be called by all constructors*/
			//	void initializerHelper();

		protected:
			SDL_Surface* createSurface(string path);
			SDL_Texture* createTexture(uint width, uint height);
			/*checks to see if this surface is the current render target, called before drawing*/
			void checkRenderTarget();

		protected:
			void markDirty();
			inline bool isDirty() const {
				return dirty;
			}

		public:
			//fetch the surfaces pixels into a buffer to poll for them
			//this can slow
			void fetchPixels();

			//release the pixels fetched by fetchPixels
			void releasePixels();

			//get the pixel at the specified x and y
			//if fetchPixels has not been called this will call it
			Color getPixelAt(uint x, uint y);

			/*returns the width of the surface*/
			inline int getWidth() const {
				return static_cast<int>(width * scaleFactor[0]);
			}
			/*returns the height of the surface*/
			inline int getHeight() const {
				return static_cast<int>(height * scaleFactor[1]);
			}
			/*returns the draw color of the surface*/
			inline const Color& getDrawColor() const {
				return drawColor;
			}

			/*Sets the rotation angle for the surface*/
			void setRotation(uint angle);
			/*Get the rotation angle for the surface*/
			inline uint getRotation() const {
				return rotation;
			}

			void setScale(float factor);
			void setScaleX(float factor);
			void setScaleY(float factor);

			inline float getScaleX() const {
				return scaleFactor[0];
			}

			inline float getScaleY() const {
				return scaleFactor[1];
			}

			/*Sets the center for the surface, the point the surface rotates around
			Defaults to the center of the surface*/
			void setCenter(VecN<int, 2> point);
			/*Gets the center point*/
			inline VecN<int, 2> getCenter() const {
				return VecN<int, 2>({ static_cast<int>(center[0]), static_cast<int>(center[1]) });
			}

			/*Sets the flip state for the surface*/
			void setFlip(FlipMode flip);
			/*Gets the flip state for the surface*/
			FlipMode getFlip() const;

			/*sets the color used for basic geometry*/
			void setDrawColor(Color color);

			/*sets this surface as the render target for all draws*/
			void setRenderTarget();
			/*sets the render target back to the window*/
			void clearRenderTarget();
			/*Saves the current render target*/
			static void saveRenderTarget();
			/*Restores the saved render target*/
			static void restoreRenderTarget();

			/*fills the entire surface with the draw color*/
			void fill(Color color);
			/*clears all the pixels of the surface*/
			void clear();
			/*causes the Surface to redraw its texture*/
			virtual void renderTexture();

			/*draw surf onto the current render target at its own position*/
			void draw(Surface& surf);
			/*draw surf onto the current render target at (x, y)*/
			void draw(Surface& surf, int x, int y);
			/*draw surf onto the current render target at (x, y)*/
			inline void draw(Surface& surf, VecN<int, 2> pos) {
				draw(surf, pos[0], pos[1]);
			}

			///*draw a scaled copy of surf onto the current render target at is own position
			//scale is measured between 0 and 1*/
			//void drawScaled(Surface& surf, float xscale, float yscale);
			///*draw a scaled copy of surf onto the current render target at (x, y)
			//scale is measured between 0 and 1*/
			//void drawScaled(Surface& surf, int x, int y, float xscale, float yscale);
			///*draw a scaled copy of surf onto the current render target at (x, y)
			//scale is measured between 0 and 1*/
			//inline void drawScaled(Surface& surf, VecN<int, 2> pos, float xscale, float yscale) {
			//	drawScaled(surf, pos[0], pos[1], xscale, yscale);
			//}

			/*draw the clip of surf onto the current render target at its own position*/
			void drawClip(Surface& surf, uint clip);
			/*draw the clip of surf onto the current render target at (x, y)*/
			void drawClip(Surface& surf, int x, int y, uint clip);
			/*draw the clip of surf onto the current render target at (x, y)*/
			inline void drawClip(Surface& surf, VecN<int, 2> pos, uint clip) {
				drawClip(surf, pos[0], pos[1], clip);
			}

			/*draw a single point at (x, y)*/
			void drawPoint(int x, int y);
			/*draw a single point at (x, y)*/
			inline void drawPoint(VecN<int, 2> pos) {
				drawPoint(pos[0], pos[1]);
			}

			/*draw a line between (x, y) and (x2, y2)*/
			void drawLine(int x, int y, int x2, int y2);
			/*draw a line between (x, y) and (x2, y2)*/
			inline void drawLine(VecN<int, 2> pos, VecN<int, 2> pos2) {
				drawLine(pos[0], pos[1], pos2[0], pos2[1]);
			}

			/*draw multiple lines, a line will be drawn between all consequetive*/
			void drawLines(vector<VecN<int, 2>>& points);

			/*draw a circle with a center at (x, y) and a radius of rad*/
			void drawCircle(int x, int y, uint rad, bool filled = false);
			/*draw a circle with a center at (x, y) and a radius of rad*/
			inline void drawCircle(VecN<int, 2> pos, uint rad, bool filled = false) {
				drawCircle(pos[0], pos[1], rad, filled);
			}

			/*draw a triangle with a center at (x, y) and diameters specified and
			the "top" of the triangle pointing in angle direction*/
			void drawTriangle(int x, int y, uint horiz, uint vert, float angle, bool filled = false);
			/*draw a triangle with a center at (x, y) and diameters specified and
			the "top" of the triangle pointing in angle direction*/
			inline void drawTriangle(VecN<int, 2> pos, uint horiz, uint vert, float angle, bool filled = false) {
				drawTriangle(pos[0], pos[1], horiz, vert, angle, filled);
			}

			/*draw a rectangle with a top left corner at (x, y) and
			a width of  and height of y*/
			void drawRect(int x, int y, int w, int h, bool filled = false);
			/*draw a rectangle with a top left corner at (x, y) and
			a width of  and height of y*/
			inline void drawRect(VecN<int, 2> pos, int w, int h, bool filled = false) {
				drawRect(pos[0], pos[1], w, h, filled);
			}

			/*draw a diamond with a center at (x, y) and diameters specified*/
			void drawDiamond(int x, int y, uint horiz, uint vert, bool filled = false);
			/*draw a diamond with a center at (x, y) and diameters specified*/
			inline void drawDiamond(VecN<int, 2> pos, uint horiz, uint vert, bool filled = false) {
				drawDiamond(pos[0], pos[1], horiz, vert);
			}

			/*modulates the surface colors with the given color*/
			void setColorMod(Color color);
			/*returns the color the surface has been modulated with*/
			inline const Color getColorMod();
			/*modulates the surface colors witht the given alpha*/
			void setAlphaMod(uchar alpha);
			/*returns the alpha the surface has been modulated with*/
			uchar getAlphaMod();
			/*sets the blending mode for this surface*/
			void setBlendMode(BlendMode blend);

			/*add a viewport to this surface described by a rectangle
			of w width and h height at (x, y)
			returns the index used to access the viewport later*/
			uint addViewport(int x, int y, int w, int h);
			/*deletes the viewport at the specified index*/
			void delViewport(uint index);
			/*sets the viewport at the specified index as the active viewport
			all draws will only happen within the bounds of the viewport*/
			void setViewport(uint index);
			/*clears the active viewport so that all draws will
			happen to the entire surface*/
			void clearViewport();

			/*adds a clip to the surface described by a rectangle
			of w width and h wieght at (x, y)
			when active a clip will limit reads
			from the surface to its area only
			returns the index used to access the clip later*/
			uint addClip(int x, int y, int w, int h);
			/*deletes the clip at the specified index*/
			void delClip(uint index);
			/*sets the clip at the specified index as the active clip*/
			void setClip(uint index);
			/*clears the active clip so that all reads from the surface
			will happen to the entire surface*/
			void clearClip();

		protected:
			SDL_Texture* texture = NULL;
			bool dirty = true;
			bool hasPos = false;
			void* pixels = NULL;
			int width = 1, height = 1;
			vector<SDL_Rect*> newViewports;
			vector<SDL_Rect*> newClips;
			SDL_Rect* activeClip = NULL;
			uint rotation = 0;
			VecN<float, 2> scaleFactor = { 1.0f, 1.0f };
			VecN<float, 2> center;
			SDL_Point sdl_center;
			SDL_RendererFlip flipState = SDL_FLIP_NONE;
			Color drawColor = Color(255, 255, 255);

			static uchar nextRenderTarget;
			static Surface* oldRenderTargets[];

			friend class WindowManager;
		};

	}

}
#endif