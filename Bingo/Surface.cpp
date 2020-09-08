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
	SDL_RenderCopy(WindowManager::getSingleton().getRenderer(), tex, NULL, NULL);

	restoreRenderTarget();

	SDL_DestroyTexture(tex);

	SDL_FreeSurface(tempSurf);

	initializerHelper();
}

Surface::Surface(string path, Color colorKey) {
	SDL_Surface* tempSurf = createSurface(path);

	SDL_SetColorKey(tempSurf, true, SDL_MapRGB(tempSurf->format, colorKey.getRed(), colorKey.getGreen(), colorKey.getBlue()));

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
	SDL_RenderCopy(WindowManager::getSingleton().getRenderer(), tex, NULL, NULL);

	restoreRenderTarget();

	SDL_DestroyTexture(tex);

	SDL_FreeSurface(tempSurf);

	initializerHelper();
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

	initializerHelper();
}

Surface::Surface(const Surface& surf) {
	width = surf.width;
	height = surf.height;

	for (uint c = 0; c < MAX_VIEWPORTS; c++) {
		if (surf.viewports[c]) {
			viewports[c] = new SDL_Rect{ surf.viewports[c]->x, \
				surf.viewports[c]->y, \
				surf.viewports[c]->w, \
				surf.viewports[c]->h };
		}
	}

	for (uint c = 0; c < MAX_CLIPS; c++) {
		if (surf.clips[c]) {
			clips[c] = new SDL_Rect{ surf.clips[c]->x, \
				surf.clips[c]->y, \
				surf.clips[c]->w, \
				surf.clips[c]->h };

			if (surf.activeClip == surf.clips[c]) {
				activeClip = clips[c];
			}
		}
	}

	rotation = surf.rotation;
	setCenter(surf.getCenter());
	flipState = surf.flipState;
	drawColor = surf.drawColor;

	//Uint32 format = NULL;

	////get exsisting texture's format
	//if (SDL_QueryTexture(surf.texture, &format, NULL, NULL, NULL)){
	//	Error("Could not get texture attributes", SDL_GetError());
	//}

	//texture = SDL_CreateTexture(WindowManager::getSingleton().getRenderer(), format, SDL_TEXTUREACCESS_TARGET, width, height);
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

	for (uint c = 0; c < MAX_VIEWPORTS; c++) {
		if (viewports[c]) {
			delete viewports[c];
		}

		if (surf.viewports[c]) {
			viewports[c] = new SDL_Rect{ surf.viewports[c]->x, \
				surf.viewports[c]->y, \
				surf.viewports[c]->w, \
				surf.viewports[c]->h };
		}
	}

	activeClip = NULL;

	for (uint c = 0; c < MAX_CLIPS; c++) {
		if (clips[c]) {
			delete clips[c];
		}

		if (surf.clips[c]) {
			clips[c] = new SDL_Rect{ surf.clips[c]->x, \
				surf.clips[c]->y, \
				surf.clips[c]->w, \
				surf.clips[c]->h };

			if (surf.activeClip == surf.clips[c]) {
				activeClip = clips[c];
			}
		}
	}

	rotation = surf.rotation;
	setCenter(surf.getCenter());
	flipState = surf.flipState;
	drawColor = surf.drawColor;

	SDL_DestroyTexture(texture);

	//Uint32 format = NULL;

	//if (SDL_QueryTexture(surf.texture, &format, NULL, NULL, NULL)){ //get exsisting texture's format
	//	Error("Could not get texture attributes", SDL_GetError());
	//}

	//texture = SDL_CreateTexture(WindowManager::getSingleton().getRenderer(), format, SDL_TEXTUREACCESS_TARGET, width, height);
	texture = createTexture(width, height);
	setBlendMode(BLEND_BLEND);

	saveRenderTarget();

	setRenderTarget();
	draw(const_cast<Surface&>(surf), 0, 0);

	restoreRenderTarget();

	return *this;
}

Surface::~Surface() {
	for (uint c = 0; c < MAX_VIEWPORTS; c++) {
		if (viewports[c]) {
			delete viewports[c];
		}
	}

	for (uint c = 0; c < MAX_CLIPS; c++) {
		if (clips[c]) {
			delete clips[c];
		}
	}

	SDL_DestroyTexture(texture);
}

void Surface::initializerHelper() {
	memset(viewports, 0, sizeof(SDL_Rect*) * MAX_VIEWPORTS);
	memset(clips, 0, sizeof(SDL_Rect*) * MAX_CLIPS);

	//if (!oldRenderTargetsInit){
	//	memset(oldRenderTargets, 0, sizeof(Surface*) * MAX_SAVED_TARGETS);
	//	oldRenderTargetsInit = true;
	//}
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

void Surface::lock() {
	SDL_LockTexture(texture, NULL, &texPixels, &texPitch);
}

void Surface::unlock() {
	SDL_UnlockTexture(texture);

	texPixels = NULL;
	texPitch = 0;
}

void Surface::setRotation(uint angle) {
	rotation = angle % 360;
}

void Surface::setCenter(VecN<int, 2> point) {
	center = VecN<float, 2>({ static_cast<float>(point[0]), static_cast<float>(point[1]) });
	sdl_center = { point[0], point[1] };
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
	//oldRenderTarget = WindowManager::getSingleton().curRenderTarget;
	if (nextRenderTarget >= MAX_SAVED_TARGETS) {
		Warn("Too many nested saved render targets");
	}
	else {
		oldRenderTargets[nextRenderTarget] = WindowManager::getSingleton().curRenderTarget;
	}

	nextRenderTarget++;
}

void Surface::restoreRenderTarget() {
	//if (oldRenderTarget){
	//	oldRenderTarget->setRenderTarget();
	//}
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

	SDL_RenderClear(WindowManager::getSingleton().getRenderer());

	WindowManager::getSingleton().setDrawColor(oldColor);
}

void Surface::clear() {
	checkRenderTarget();

	Color oldColor = WindowManager::getSingleton().getDrawColor();

	WindowManager::getSingleton().setDrawColor(TRANSPARENT);

	SDL_RenderClear(WindowManager::getSingleton().getRenderer());

	WindowManager::getSingleton().setDrawColor(oldColor);
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
		dest.w = surf.activeClip->w;
		dest.h = surf.activeClip->h;
	}

	SDL_RenderCopyEx(WindowManager::getSingleton().getRenderer(), surf.texture, surf.activeClip, &dest, surf.rotation, &surf.sdl_center, surf.flipState);
}

void Surface::draw(Surface& surf, int x, int y) {
	checkRenderTarget();

	if (surf.isDirty()) {
		surf.renderTexture();
	}

	SDL_Rect dest = { x, y, surf.getWidth(), surf.getHeight() };

	if (surf.activeClip) {
		dest.w = surf.activeClip->w;
		dest.h = surf.activeClip->h;
	}

	SDL_RenderCopyEx(WindowManager::getSingleton().getRenderer(), surf.texture, surf.activeClip, &dest, surf.rotation, &surf.sdl_center, surf.flipState);
}

void Surface::drawScaled(Surface& surf, float xscale, float yscale) {
	checkRenderTarget();

	if (surf.isDirty()) {
		surf.renderTexture();
	}

	SDL_Rect dest = { surf.getPosX(), surf.getPosY(), \
		static_cast<int>(surf.getWidth() * xscale), \
		static_cast<int>(surf.getHeight() * yscale) };

	if (surf.activeClip) {
		dest.w = static_cast<int>(surf.activeClip->w * xscale);
		dest.h = static_cast<int>(surf.activeClip->h * yscale);
	}

	SDL_RenderCopyEx(WindowManager::getSingleton().getRenderer(), surf.texture, surf.activeClip, &dest, surf.rotation, &surf.sdl_center, surf.flipState);
}

void Surface::drawScaled(Surface& surf, int x, int y, float xscale, float yscale) {
	checkRenderTarget();

	if (surf.isDirty()) {
		surf.renderTexture();
	}

	SDL_Rect dest = { x, y, \
		static_cast<int>(surf.getWidth() * xscale), \
		static_cast<int>(surf.getHeight() * yscale) };

	if (surf.activeClip) {
		dest.w = static_cast<int>(surf.activeClip->w * xscale);
		dest.h = static_cast<int>(surf.activeClip->h * yscale);
	}

	SDL_RenderCopyEx(WindowManager::getSingleton().getRenderer(), surf.texture, surf.activeClip, &dest, surf.rotation, &surf.sdl_center, surf.flipState);
}

void Surface::drawPoint(int x, int y) {
	checkRenderTarget();

	Color oldColor = WindowManager::getSingleton().getDrawColor();

	WindowManager::getSingleton().setDrawColor(drawColor);

	SDL_RenderDrawPoint(WindowManager::getSingleton().getRenderer(), x, y);

	WindowManager::getSingleton().setDrawColor(oldColor);
}

void Surface::drawLine(int x, int y, int x2, int y2) {
	checkRenderTarget();

	Color oldColor = WindowManager::getSingleton().getDrawColor();

	WindowManager::getSingleton().setDrawColor(drawColor);

	SDL_RenderDrawLine(WindowManager::getSingleton().getRenderer(), x, y, x2, y2);

	WindowManager::getSingleton().setDrawColor(oldColor);
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

	SDL_RenderDrawLines(WindowManager::getSingleton().getRenderer(), sdlPoints, points.size());

	WindowManager::getSingleton().setDrawColor(oldColor);

	delete[] sdlPoints;
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

	SDL_RenderDrawPoints(WindowManager::getSingleton().getRenderer(), &(points[0]), points.size());
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

	SDL_RenderDrawLines(WindowManager::getSingleton().renderer, &(points[0]), points.size());
#endif

	WindowManager::getSingleton().setDrawColor(oldColor);
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

	SDL_RenderDrawLines(WindowManager::getSingleton().getRenderer(), points.data(), points.size());
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

	SDL_RenderDrawLines(WindowManager::getSingleton().getRenderer(), points.data(), points.size());
}

void Surface::drawTriangle(int x, int y, uint horiz, uint vert, float angleDeg, bool filled) {
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

	//drawCircle(points[0].x, points[0].y, 10, false);
	//setDrawColor(BLUE);
	//drawCircle(points[1].x, points[1].y, 8, false);
	//setDrawColor(RED);
	//drawCircle(points[2].x, points[2].y, 10, false);
	//setDrawColor(GREEN);
	//drawCircle(x, y, 10, false);

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
		SDL_RenderDrawLines(WindowManager::getSingleton().getRenderer(), points, 4);
	}

	WindowManager::getSingleton().setDrawColor(oldColor);
}

void Surface::drawRect(int x, int y, int w, int h, bool filled) {
	checkRenderTarget();

	SDL_Rect rect = { x, y, w, h };
	Color oldColor = WindowManager::getSingleton().getDrawColor();

	WindowManager::getSingleton().setDrawColor(drawColor);

	if (filled) {
		SDL_RenderFillRect(WindowManager::getSingleton().getRenderer(), &rect);
	}
	else {
		SDL_RenderDrawRect(WindowManager::getSingleton().getRenderer(), &rect);
	}

	WindowManager::getSingleton().setDrawColor(oldColor);
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
		SDL_RenderDrawLines(WindowManager::getSingleton().getRenderer(), points, 5);
	}

	WindowManager::getSingleton().setDrawColor(oldColor);
}

void Surface::setColorMod(Color color) {
	SDL_SetTextureColorMod(texture, color.getRed(), color.getGreen(), color.getBlue());
}

const Color Surface::getColorMod() {
	uchar col[3];

	SDL_GetTextureColorMod(texture, col, &(col[1]), &(col[2]));

	return Color(col[0], col[1], col[2]);
}

void Surface::setAlphaMod(uchar alpha) {
	SDL_SetTextureAlphaMod(texture, alpha);
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
}

uint Surface::addViewport(int x, int y, int w, int h) {
	int freeSlot = -1;

	for (int c = 0; c < MAX_VIEWPORTS; c++) {
		if (viewports[c] == NULL) {
			freeSlot = c;
			viewports[c] = new SDL_Rect{ x, y, w, h };
			break;
		}
	}

	return freeSlot;
}

void Surface::delViewport(uint index) {
	if (index > MAX_VIEWPORTS || viewports[index] == NULL) {
		throw Exception("Deleting an invalid viewport index\n");
	}
	else {
		delete viewports[index];
		viewports[index] = NULL;
	}
}

void Surface::setViewport(uint index) {
	if (WindowManager::getSingleton().curRenderTarget != this) {
		Warn("Setting a viewport while this surface isnt the render target\n");
	}

	if (index > MAX_VIEWPORTS || viewports[index] == NULL) {
		throw Exception("Setting an invalid viewport index\n");
	}

	SDL_RenderSetViewport(WindowManager::getSingleton().getRenderer(), viewports[index]);
}

void Surface::clearViewport() {
	SDL_RenderSetViewport(WindowManager::getSingleton().getRenderer(), NULL);
}

uint Surface::addClip(int x, int y, int w, int h) {
	int freeSlot = -1;

	for (int c = 0; c < MAX_CLIPS; c++) {
		if (clips[c] == NULL) {
			freeSlot = c;
			clips[c] = new SDL_Rect{ x, y, w, h };
			break;
		}
	}

	return freeSlot;
}

void Surface::delClip(uint index) {
	if (index > MAX_CLIPS || clips[index] == NULL) {
		throw Exception("Deleting an invalid clip index\n");
	}
	else {
		delete clips[index];
		clips[index] = NULL;
	}
}

void Surface::setClip(uint index) {
	if (index > MAX_CLIPS || clips[index] == NULL) {
		throw Exception("Setting an invalid clip index\n");
	}
	else {
		activeClip = clips[index];
	}
}

void Surface::clearClip() {
	activeClip = NULL;
}

//Surface* Surface::oldRenderTarget = NULL;
uchar Surface::nextRenderTarget = 0;
Surface* Surface::oldRenderTargets[MAX_SAVED_TARGETS];