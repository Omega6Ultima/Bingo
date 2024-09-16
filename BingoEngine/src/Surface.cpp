//Dustin Gehm

#include "Surface.h"

#include <SDL_image.h>

#include "GUI.h"
#include "MovableSurface.h"
#include "ThreadManager.h"
#include "WindowManager.h"

#define USE_BRESENHAM_CIRCLE 1
#define USE_BRESENHAM_TRIANGLE 1

using std::cos;
using std::sin;
using std::vector;

using Bingo::Colors::Color;
using Bingo::Surfaces::Surface;
using Bingo::Surfaces::WindowManager;
using Bingo::Utils::degToRadians;
using Bingo::Utils::Error;
using Bingo::Utils::Warn;

Surface::Surface(string path) {
	SDL_Surface* tempSurf = createSurface(path);

	width = tempSurf->w;
	height = tempSurf->h;
	setCenter({ width / 2, height / 2 });

	SDL_Texture* tex = NULL;

	tex = SDL_CreateTextureFromSurface(WindowManager::getSingleton().getRenderer(), tempSurf);

	if (!tex) {
		Error("Could not load texture from surface", SDL_GetError());
	}

	texture = createTexture(width, height);
	setBlendMode(BLEND_BLEND);

	saveRenderTarget();
	setRenderTarget();

	clear();

	// TODO This line is the code that is sometimes necessary to get stuff to draw @TestMeshManager::TestDraw
	if (SDL_RenderCopy(WindowManager::getSingleton().getRenderer(), tex, NULL, NULL)) {
		throw Exception("RenderCopy failed");
	}

	restoreRenderTarget();

	SDL_DestroyTexture(tex);
	SDL_FreeSurface(tempSurf);
}

Surface::Surface(string path, Color colorKey) {
	SDL_Surface* tempSurf = createSurface(path);

	SDL_SetColorKey(tempSurf, true, SDL_MapRGBA(tempSurf->format, colorKey.getRed(), colorKey.getGreen(), colorKey.getBlue(), colorKey.getAlpha()));

	width = tempSurf->w;
	height = tempSurf->h;
	setCenter({ width / 2, height / 2 });

	SDL_Texture* tex = NULL;

	tex = SDL_CreateTextureFromSurface(WindowManager::getSingleton().getRenderer(), tempSurf);

	if (!tex) {
		Error("Could not load texture from surface", SDL_GetError());
	}

	texture = createTexture(width, height);
	setBlendMode(BLEND_BLEND);

	saveRenderTarget();

	setRenderTarget();

	clear();

	if (SDL_RenderCopy(WindowManager::getSingleton().getRenderer(), tex, NULL, NULL)) {
		throw Exception("RenderCopy failed");
	}

	restoreRenderTarget();

	SDL_DestroyTexture(tex);

	SDL_FreeSurface(tempSurf);
}

Surface::Surface(int width, int height) {
	if (width < 1 || height < 1) {
		throw Exception("Cannot have a 0-dimension texture");
	}

	this->width = width;
	this->height = height;
	setCenter({ width / 2, height / 2 });

	texture = createTexture(width, height);
	setBlendMode(BLEND_BLEND);
}

Surface::Surface(const Surface& surf) {
	width = surf.width;
	height = surf.height;

	for (auto iter = surf.newViewports.begin(); iter != surf.newViewports.end(); iter++) {
		newViewports.push_back(new SDL_Rect{ (*iter)->x,
											(*iter)->y,
											(*iter)->w,
											(*iter)->h });
	}

	for (auto iter = surf.newClips.begin(); iter != surf.newClips.end(); iter++) {
		newClips.push_back(new SDL_Rect{ (*iter)->x,
										(*iter)->y,
										(*iter)->w,
										(*iter)->h });

		if (surf.activeClip == *iter) {
			activeClip = newClips.back();
		}
	}

	rotation = surf.rotation;
	setCenter(surf.getCenter());
	flipState = surf.flipState;
	drawColor = surf.drawColor;

	texture = createTexture(width, height);
	setBlendMode(BLEND_BLEND);

	saveRenderTarget();

	setRenderTarget();
	draw(const_cast<Surface&>(surf), 0, 0);

	restoreRenderTarget();
}

Surface& Surface::operator =(const Surface& surf) {
	width = surf.width;
	height = surf.height;

	//initializerHelper();

	for (auto iter = newViewports.begin(); iter != newViewports.end(); iter++) {
		if (*iter) {
			delete* iter;
		}
	}

	newViewports.resize(0);

	for (auto iter = surf.newViewports.begin(); iter != surf.newViewports.end(); iter++) {
		newViewports.push_back(new SDL_Rect{ (*iter)->x,
											(*iter)->y,
											(*iter)->w,
											(*iter)->h });
	}

	activeClip = NULL;

	for (auto iter = newClips.begin(); iter != newClips.end(); iter++) {
		if (*iter) {
			delete* iter;
		}
	}

	newClips.resize(0);

	for (auto iter = surf.newClips.begin(); iter != surf.newClips.end(); iter++) {
		newClips.push_back(new SDL_Rect{ (*iter)->x,
										(*iter)->y,
										(*iter)->w,
										(*iter)->h });

		if (surf.activeClip == *iter) {
			activeClip = newClips.back();
		}
	}

	rotation = surf.rotation;
	setCenter(surf.getCenter());
	flipState = surf.flipState;
	drawColor = surf.drawColor;

	SDL_DestroyTexture(texture);

	texture = createTexture(width, height);
	setBlendMode(BLEND_BLEND);

	saveRenderTarget();

	setRenderTarget();
	draw(const_cast<Surface&>(surf), 0, 0);

	restoreRenderTarget();

	return *this;
}

Surface::~Surface() {
	for (auto iter = newViewports.begin(); iter != newViewports.end(); iter++) {
		if (*iter) {
			delete* iter;
		}
	}

	for (auto iter = newClips.begin(); iter != newClips.end(); iter++) {
		if (*iter) {
			delete* iter;
		}
	}

	if (WindowManager::getSingleton().curRenderTarget == this) {
		WindowManager::getSingleton().curRenderTarget = NULL;
	}

	releasePixels();

	SDL_DestroyTexture(texture);
}

SDL_Surface* Surface::createSurface(string path) {
	SDL_Surface* tempSurf = NULL;

	ATOMIC_LOCK(ThreadManager::fileLock);

	tempSurf = IMG_Load(path.c_str()); //load the image with SDL_image's load

	ATOMIC_UNLOCK(ThreadManager::fileLock);

	if (!tempSurf) {
		Error("Failed to create surface", IMG_GetError());
	}

	return tempSurf;
}

SDL_Texture* Surface::createTexture(uint width, uint height) {
	SDL_Texture* tex = NULL;
	Uint32 pixelFormat = WindowManager::getSingleton().getPixelFormat();

	tex = SDL_CreateTexture(WindowManager::getSingleton().getRenderer(), pixelFormat, SDL_TEXTUREACCESS_TARGET, width, height);

	if (!tex) {
		Error("Could not create blank texture", SDL_GetError());
	}

	return tex;
}

void Surface::checkRenderTarget() {
	//TODO consider replacing calls to this with save/restore render target
	if (WindowManager::getSingleton().curRenderTarget != this) {
		Warn("The target surface isnt the current render target\n");
	}
}

void Surface::markDirty() {
	dirty = true;
}

void Surface::fetchPixels() {
	if (isDirty()) { // !pixels || 
		releasePixels();

		pixels = new char[getWidth() * getHeight() * 5];
		memset(pixels, 0, getWidth() * getHeight() * 5);

		saveRenderTarget();
		setRenderTarget();

		if (SDL_RenderReadPixels(WindowManager::getSingleton().getRenderer(), NULL, SDL_PIXELFORMAT_RGBA8888, pixels, getWidth() * 4)) {
			Warn("Could not read pixels from renderer", SDL_GetError());
		}

		restoreRenderTarget();
	}
}

void Surface::releasePixels() {
	if (pixels) {
		delete pixels;
		pixels = NULL;
	}
}

Color Surface::getPixelAt(uint x, uint y) {
	fetchPixels();

	Uint32 pixel = 0;
	uint w = getWidth();
	uint h = getHeight();

	switch (getFlip()) {
	case FlipMode::FLIP_NONE:
		pixel = static_cast<Uint32*>(pixels)[y * w + x];
		break;
	case FlipMode::FLIP_HORIZONTAL:
		pixel = static_cast<Uint32*>(pixels)[y * w + (w - 1 - x)];
		break;
	case FlipMode::FLIP_VERTICAL:
		pixel = static_cast<Uint32*>(pixels)[(h - 1 - y) * w + x];
		break;
	default:
		throw Exception("Invalid FlipMode");
		break;
	}

	return Color((pixel >> 24) & 0xFF,
		(pixel >> 16) & 0xFF,
		(pixel >> 8) & 0xFF,
		(pixel & 0xFF));
}

void Surface::setRotation(uint angle) {
	rotation = angle % 360;

	markDirty();
}

void Surface::setScale(float factor) {
	scaleFactor = { factor, factor };

	markDirty();
}

void Surface::setScaleX(float factor) {
	scaleFactor[0] = factor;

	markDirty();
}

void Surface::setScaleY(float factor) {
	scaleFactor[1] = factor;

	markDirty();
}

void Surface::setCenter(VecN<int, 2> point) {
	center = VecN<float, 2>({ static_cast<float>(point[0]), static_cast<float>(point[1]) });
	sdl_center = { point[0], point[1] };

	markDirty();
}

void Surface::setFlip(FlipMode flip) {
#if 0
	switch (flip) {
	case FLIP_NONE:
		flipState = SDL_FLIP_NONE;
		break;
	case FLIP_HORIZONTAL:
		flipState = SDL_FLIP_HORIZONTAL;
		break;
	case FLIP_VERTICAL:
		flipState = SDL_FLIP_VERTICAL;
		break;
	default:
		throw Exception("Invalid FlipMode");
	}
#else
	flipState = static_cast<SDL_RendererFlip>(flip);
#endif

	markDirty();
}

Surface::FlipMode Surface::getFlip() const {
#if 0
	switch (flipState) {
	case SDL_FLIP_NONE:
		return FLIP_NONE;
	case SDL_FLIP_HORIZONTAL:
		return FLIP_HORIZONTAL;
	case SDL_FLIP_VERTICAL:
		return FLIP_VERTICAL;
	default:
		throw Exception("Invalid surface flipstate\n");
	}
#else
	return static_cast<FlipMode>(flipState);
#endif
}

void Surface::setRenderTarget() {
	if (SDL_SetRenderTarget(WindowManager::getSingleton().getRenderer(), texture)) {
		Error("Could not change render target", SDL_GetError());
	}

	WindowManager::getSingleton().curRenderTarget = this;
}

void Surface::clearRenderTarget() {
	if (SDL_SetRenderTarget(WindowManager::getSingleton().getRenderer(), NULL)) {
		Error("Could not change render target\n", SDL_GetError());
	}

	WindowManager::getSingleton().curRenderTarget = NULL;
}

void Surface::saveRenderTarget() {
	if (nextRenderTarget >= MAX_SAVED_TARGETS) {
		Warn("Too many nested saved render targets");
	}
	else {
		oldRenderTargets[nextRenderTarget] = WindowManager::getSingleton().curRenderTarget;
	}

	nextRenderTarget++;
}

void Surface::restoreRenderTarget() {
	nextRenderTarget--;

	if (nextRenderTarget >= MAX_SAVED_TARGETS) {
		Warn("Restoring from too many targets");
	}
	else {
		if (oldRenderTargets[nextRenderTarget]) {
			oldRenderTargets[nextRenderTarget]->setRenderTarget();
			oldRenderTargets[nextRenderTarget] = NULL;
		}
	}
}

void Surface::setDrawColor(Color color) {
	drawColor = color;
}

void Surface::fill(Color color) {
	checkRenderTarget();
	Color oldColor = WindowManager::getSingleton().getDrawColor();

	WindowManager::getSingleton().setDrawColor(color);

	if (SDL_RenderClear(WindowManager::getSingleton().getRenderer())) {
		throw Exception("RenderClear failed");
	}

	WindowManager::getSingleton().setDrawColor(oldColor);

	markDirty();
}

void Surface::clear() {
	checkRenderTarget();

	Color oldColor = WindowManager::getSingleton().getDrawColor();

	WindowManager::getSingleton().setDrawColor(TRANSPARENT);

	if (SDL_RenderClear(WindowManager::getSingleton().getRenderer())) {
		throw Exception("RenderClear failed");
	}

	WindowManager::getSingleton().setDrawColor(oldColor);

	markDirty();
}

void Surface::renderTexture() {
	dirty = false;
}

void Surface::draw(Surface& surf) {
	checkRenderTarget();

	if (surf.isDirty()) {
		surf.renderTexture();
	}

	SDL_Rect dest = { surf.getPosX(), surf.getPosY(), surf.getWidth(), surf.getHeight() };

	if (surf.activeClip) {
		dest.w = static_cast<int>(surf.activeClip->w * surf.scaleFactor[0]);
		dest.h = static_cast<int>(surf.activeClip->h * surf.scaleFactor[1]);
	}

	if (SDL_RenderCopyEx(WindowManager::getSingleton().getRenderer(), surf.texture, surf.activeClip, &dest, surf.rotation, &surf.sdl_center, surf.flipState)) {
		throw Exception("RenderCopyEx");
	}

	markDirty();
}

void Surface::draw(Surface& surf, int x, int y) {
	checkRenderTarget();

	if (surf.isDirty()) {
		surf.renderTexture();
	}

	SDL_Rect dest = { x, y, surf.getWidth(), surf.getHeight() };

	if (surf.activeClip) {
		dest.w = static_cast<int>(surf.activeClip->w * surf.scaleFactor[0]);
		dest.h = static_cast<int>(surf.activeClip->h * surf.scaleFactor[1]);
	}

	if (SDL_RenderCopyEx(WindowManager::getSingleton().getRenderer(), surf.texture, surf.activeClip, &dest, surf.rotation, &surf.sdl_center, surf.flipState)) {
		throw Exception("RenderCopyEx failed");
	}

	markDirty();
}

void Surface::drawClip(Surface& surf, uint clip) {
	checkRenderTarget();

	if (surf.isDirty()) {
		surf.renderTexture();
	}

	if (clip >= surf.newClips.size() || surf.newClips[clip] == NULL) {
		throw Exception("Drawing an invalid clip index\n");
	}

	SDL_Rect dest = { surf.getPosX(), surf.getPosY(), surf.newClips[clip]->w, surf.newClips[clip]->h };

	if (SDL_RenderCopyEx(WindowManager::getSingleton().getRenderer(), surf.texture, surf.newClips[clip], &dest, surf.rotation, &surf.sdl_center, surf.flipState)) {
		throw Exception("RenderCopyEx failed");
	}

	markDirty();
}

void Surface::drawClip(Surface& surf, int x, int y, uint clip) {
	checkRenderTarget();

	if (surf.isDirty()) {
		surf.renderTexture();
	}

	if (clip >= surf.newClips.size() || surf.newClips[clip] == NULL) {
		throw Exception("Drawing an invalid clip index\n");
	}

	SDL_Rect dest = { x, y, surf.newClips[clip]->w, surf.newClips[clip]->h };

	if (SDL_RenderCopyEx(WindowManager::getSingleton().getRenderer(), surf.texture, surf.newClips[clip], &dest, surf.rotation, &surf.sdl_center, surf.flipState)) {
		throw Exception("RenderCopyEx failed");
	}

	markDirty();
}

void Surface::drawPoint(int x, int y) {
	checkRenderTarget();

	Color oldColor = WindowManager::getSingleton().getDrawColor();

	WindowManager::getSingleton().setDrawColor(drawColor);

	if (SDL_RenderDrawPoint(WindowManager::getSingleton().getRenderer(), x, y)) {
		throw Exception("RenderDrawPoint failed");
	}

	WindowManager::getSingleton().setDrawColor(oldColor);

	markDirty();
}

void Surface::drawLine(int x, int y, int x2, int y2) {
	checkRenderTarget();

	Color oldColor = WindowManager::getSingleton().getDrawColor();

	WindowManager::getSingleton().setDrawColor(drawColor);

	if (SDL_RenderDrawLine(WindowManager::getSingleton().getRenderer(), x, y, x2, y2)) {
		throw Exception("RenderDrawLine failed");
	}

	WindowManager::getSingleton().setDrawColor(oldColor);

	markDirty();
}

void Surface::drawLines(vector<VecN<int, 2>>& points) {
	SDL_Point* sdlPoints = new SDL_Point[points.size()];

	for (uint c = 0; c < points.size(); c++) {
		sdlPoints[c].x = points[c][0];
		sdlPoints[c].y = points[c][1];
	}

	checkRenderTarget();

	Color oldColor = WindowManager::getSingleton().getDrawColor();

	WindowManager::getSingleton().setDrawColor(drawColor);

	if (SDL_RenderDrawLines(WindowManager::getSingleton().getRenderer(), sdlPoints, points.size())) {
		throw Exception("RenderDrawLines failed");
	}

	WindowManager::getSingleton().setDrawColor(oldColor);

	delete[] sdlPoints;

	markDirty();
}

void Surface::drawCircle(int x, int y, uint rad, bool filled) {
	checkRenderTarget();

	vector<SDL_Point> points;
	SDL_Point p;

	Color oldColor = WindowManager::getSingleton().getDrawColor();
	WindowManager::getSingleton().setDrawColor(drawColor);

#if USE_BRESENHAM_CIRCLE
	//algorithm is an adaptation of Bresenham's circle algorithm tweaked by fins
	//http://stackoverflow.com/questions/1201200/fast-algorithm-for-drawing-filled-circles
	//further tweaked by me to allow the drawing of non-filled circles

	int xs = rad;
	int ys = 0;
	int xchange = 1 - (rad << 1);
	int ychange = 0;
	int radError = 0;

	while (xs >= ys) {
		if (filled) {
			for (int c = x - xs; c <= x + xs; c++) {
				p.x = c;
				p.y = y + ys;
				points.push_back(p);

				//p.x = c;
				p.y = y - ys;
				points.push_back(p);
			}
		}
		else {
			p.x = x - xs;
			p.y = y + ys;
			points.push_back(p);

			//p.x = x - xs;
			p.y = y - ys;
			points.push_back(p);

			p.x = x + xs;
			//p.y = y - ys;
			points.push_back(p);

			//p.x = x + xs;
			p.y = y + ys;
			points.push_back(p);
		}

		if (filled) {
			for (int c = x - ys; c < x + ys; c++) {
				p.x = c;
				p.y = y + xs;
				points.push_back(p);

				//p.x = c;
				p.y = y - xs;
				points.push_back(p);
			}
		}
		else {
			p.x = x - ys;
			p.y = y + xs;
			points.push_back(p);

			//p.x = x - ys;
			p.y = y - xs;
			points.push_back(p);

			p.x = x + ys;
			//p.y = y - xs;
			points.push_back(p);

			//p.x = x + ys;
			p.y = y + xs;
			points.push_back(p);
		}

		ys++;
		radError += ychange;
		ychange += 2;

		if (((radError << 1) + xchange) > 0) {
			xs--;
			radError += xchange;
			xchange += 2;
		}
	}

	if (SDL_RenderDrawPoints(WindowManager::getSingleton().getRenderer(), &(points[0]), points.size())) {
		throw Exception("RenderDrawPoints failed");
	}
#else
	uint precision = MAX(10, static_cast<uint>(rad / 1.5));

	if (filled) {
		precision *= 10;
		for (int c = 0; c < precision; c++) {
			p.x = x;
			p.y = y;
			points.push_back(p);

			p.x = x + static_cast<int>(rad * cos(((2 * M_PI) / precision) * c));
			p.y = y - static_cast<int>(rad * sin(((2 * M_PI) / precision) * c));
			points.push_back(p);
		}

		p.x = x;
		p.y = y;
		points.push_back(p);
	}
	else {
		for (int c = 0; c < precision; c++) {
			p.x = x + static_cast<int>(rad * cos(((2 * M_PI) / precision) * c));
			p.y = y - static_cast<int>(rad * sin(((2 * M_PI) / precision) * c));
			points.push_back(p);
		}
	}

	p.x = x + static_cast<int>(rad * cos(0));
	p.y = y + static_cast<int>(rad * sin(0));
	points.push_back(p);

	if (SDL_RenderDrawLines(WindowManager::getSingleton().renderer, &(points[0]), points.size())) {
		throw Exception("RenderDrawLines failed");
	}
#endif

	WindowManager::getSingleton().setDrawColor(oldColor);

	markDirty();
}

static void fillFlatBottomTri(SDL_Point& p0, SDL_Point& p1, SDL_Point& p2) {
	float slopes[] = { static_cast<float>(p1.x - p0.x) / (p1.y - p0.y),
		static_cast<float>(p2.x - p0.x) / (p2.y - p0.y) };
	float curX[] = { static_cast<float>(p0.x), static_cast<float>(p0.x) };

	vector<SDL_Point> points;

	for (int c = p0.y; c <= p1.y; c++) {
		points.push_back({ static_cast<int>(curX[0]), c });
		points.push_back({ static_cast<int>(curX[1]), c });

		curX[0] += slopes[0];
		curX[1] += slopes[1];
	}

	if (SDL_RenderDrawLines(WindowManager::getSingleton().getRenderer(), points.data(), points.size())) {
		throw Bingo::Exception("RenderDrawLines failed");
	}
}

static void fillFlatTopTri(SDL_Point& p0, SDL_Point& p1, SDL_Point& p2) {
	float slopes[] = { static_cast<float>(p2.x - p0.x) / (p2.y - p0.y),
		static_cast<float>(p2.x - p1.x) / (p2.y - p1.y) };
	float curX[] = { static_cast<float>(p2.x), static_cast<float>(p2.x) };

	vector<SDL_Point> points;

	for (int c = p2.y; c > p0.y; c--) {
		points.push_back({ static_cast<int>(curX[0]), c });
		points.push_back({ static_cast<int>(curX[1]), c });

		curX[0] -= slopes[0];
		curX[1] -= slopes[1];
	}

	if (SDL_RenderDrawLines(WindowManager::getSingleton().getRenderer(), points.data(), points.size())) {
		throw Bingo::Exception("RenderDrawLines failed");
	}
}

void Surface::drawTriangle(int x, int y, uint horiz, uint vert, float angleDeg, bool filled) {
	using Utils::operator==;

	checkRenderTarget();

	Color oldColor = WindowManager::getSingleton().getDrawColor();

	WindowManager::getSingleton().setDrawColor(drawColor);

	uint halfHoriz = horiz / 2;
	uint halfVert = vert / 2;
	double c = cos(degToRadians(angleDeg));
	double s = sin(degToRadians(angleDeg));
	SDL_Point points[4];

	points[0] = { x + static_cast<int>(halfVert * s), y - static_cast<int>(halfVert * c) };
	points[1] = { x - static_cast<int>(halfHoriz * c) - static_cast<int>(halfVert * s), y - static_cast<int>(halfHoriz * s) + static_cast<int>(halfVert * c) };
	points[2] = { x + static_cast<int>(halfHoriz * c) - static_cast<int>(halfVert * s), y + static_cast<int>(halfHoriz * s) + static_cast<int>(halfVert * c) };
	points[3] = points[0];

	drawCircle(points[0].x, points[0].y, 10, false);
	setDrawColor(Bingo::Colors::BLUE);
	drawCircle(points[1].x, points[1].y, 8, false);
	setDrawColor(Bingo::Colors::RED);
	drawCircle(points[2].x, points[2].y, 10, false);
	setDrawColor(Bingo::Colors::GREEN);
	drawCircle(x, y, 10, false);

	if (filled) {
#if USE_BRESENHAM_TRIANGLE
		for (int c = 0; c < 2; c++) {
			if (points[1].y < points[0].y) {
				SDL_Point temp = points[1];
				points[1] = points[0];
				points[0] = temp;
			}

			if (points[2].y < points[1].y) {
				SDL_Point temp = points[2];
				points[2] = points[1];
				points[1] = temp;
			}
		}

		if (points[1] == points[2]) {
			fillFlatBottomTri(points[0], points[1], points[2]);
		}
		else if (points[0] == points[1]) {
			fillFlatTopTri(points[0], points[1], points[2]);
		}
		else {
			SDL_Point extraPoint = { static_cast<int>(points[0].x + (static_cast<float>(points[1].y - points[0].y) / (points[2].y - points[0].y) * (points[2].x - points[0].x))), points[1].y };

			fillFlatBottomTri(points[0], points[1], extraPoint);
			fillFlatTopTri(points[1], extraPoint, points[2]);
		}
#endif
	}
	else {
		if (SDL_RenderDrawLines(WindowManager::getSingleton().getRenderer(), points, 4)) {
			throw Exception("RenderDrawLines failed");
		}
	}

	WindowManager::getSingleton().setDrawColor(oldColor);

	markDirty();
}

void Surface::drawRect(int x, int y, int w, int h, bool filled) {
	checkRenderTarget();

	SDL_Rect rect = { x, y, w, h };
	Color oldColor = WindowManager::getSingleton().getDrawColor();

	WindowManager::getSingleton().setDrawColor(drawColor);

	if (filled) {
		if (SDL_RenderFillRect(WindowManager::getSingleton().getRenderer(), &rect)) {
			throw Exception("RenderFillRect failed");
		}
	}
	else {
		if (SDL_RenderDrawRect(WindowManager::getSingleton().getRenderer(), &rect)) {
			throw Exception("RenderDrawRect failed");
		}
	}

	WindowManager::getSingleton().setDrawColor(oldColor);

	markDirty();
}

void Surface::drawDiamond(int x, int y, uint horiz, uint vert, bool filled) {
	checkRenderTarget();

	Color oldColor = WindowManager::getSingleton().getDrawColor();

	WindowManager::getSingleton().setDrawColor(drawColor);

	uint halfHoriz = horiz / 2;
	uint halfVert = vert / 2;
	SDL_Point points[5];

	points[0] = { x, static_cast<int>(y - halfVert) };
	points[1] = { static_cast<int>(x + halfHoriz), y };
	points[2] = { x, static_cast<int>(y + halfVert) };
	points[3] = { static_cast<int>(x - halfHoriz), y };
	points[4] = points[0];

	if (filled) {
		fillFlatBottomTri(points[0], points[1], points[3]);
		fillFlatTopTri(points[3], points[1], points[2]);
	}
	else {
		if (SDL_RenderDrawLines(WindowManager::getSingleton().getRenderer(), points, 5)) {
			throw Exception("RenderDrawLines failed");
		}
	}

	WindowManager::getSingleton().setDrawColor(oldColor);

	markDirty();
}

void Surface::setColorMod(Color color) {
	SDL_SetTextureColorMod(texture, color.getRed(), color.getGreen(), color.getBlue());

	markDirty();
}

const Color Surface::getColorMod() {
	uchar col[3];

	SDL_GetTextureColorMod(texture, col, &(col[1]), &(col[2]));

	return Color(col[0], col[1], col[2]);
}

void Surface::setAlphaMod(uchar alpha) {
	SDL_SetTextureAlphaMod(texture, alpha);

	markDirty();
}

uchar Surface::getAlphaMod() {
	uchar alpha;

	SDL_GetTextureAlphaMod(texture, &alpha);

	return alpha;
}

void Surface::setBlendMode(BlendMode blend) {
#if 0
	SDL_BlendMode mode;

	switch (blend) {
	case BLEND_NONE:
		mode = SDL_BLENDMODE_NONE;
		break;
	case BLEND_BLEND:
		mode = SDL_BLENDMODE_BLEND;
		break;
	case BLEND_ADD:
		mode = SDL_BLENDMODE_ADD;
		break;
	case BLEND_MOD:
		mode = SDL_BLENDMODE_MOD;
		break;
	default:
		throw Exception("Invalid BlendMode");
	}

	SDL_SetTextureBlendMode(texture, mode);
#else
	SDL_SetTextureBlendMode(texture, static_cast<SDL_BlendMode>(blend));
#endif

	markDirty();
}

uint Surface::addViewport(int x, int y, int w, int h) {
	newViewports.push_back(new SDL_Rect{ x, y, w, h });

	return newViewports.size() - 1;
}

void Surface::delViewport(uint index) {
	if (index > newViewports.size() || newViewports[index] == NULL) {
		throw Exception("Deleting an invalid viewport index\n");
	}

	delete newViewports[index];

	newViewports[index] = NULL;
}

void Surface::setViewport(uint index) {
	if (WindowManager::getSingleton().curRenderTarget != this) {
		Warn("Setting a viewport while this surface isnt the render target\n");
	}

	if (index >= newViewports.size() || newViewports[index] == NULL) {
		throw Exception("Setting an invalid viewport index\n");
	}

	if (SDL_RenderSetViewport(WindowManager::getSingleton().getRenderer(), newViewports[index])) {
		throw Exception("RenderSetViewport failed");
	}

}

void Surface::clearViewport() {
	if (SDL_RenderSetViewport(WindowManager::getSingleton().getRenderer(), NULL)) {
		throw Exception("RenderSetViewport failed");
	}
}

uint Surface::addClip(int x, int y, int w, int h) {
	newClips.push_back(new SDL_Rect{ x, y, w, h });

	return newClips.size() - 1;
}

void Surface::delClip(uint index) {
	if (index >= newClips.size() || newClips[index] == NULL) {
		throw Exception("Deleting an invalid clip index\n");
	}

	delete newClips[index];

	newClips[index] = NULL;
}

void Surface::setClip(uint index) {
	if (index >= newClips.size() || newClips[index] == NULL) {
		throw Exception("Setting an invalid clip index\n");
	}

	activeClip = newClips[index];
}

void Surface::clearClip() {
	activeClip = NULL;
}

uchar Surface::nextRenderTarget = 0;
Surface* Surface::oldRenderTargets[MAX_SAVED_TARGETS];