//Dustin Gehm

#include "TextSurface.h"

#include "WindowManager.h"

TextSurface::TextSurface(string fontName, int size, string displayText, Color textColor)
: Surface(1, 1){
	font = fontName;
	fontSize = size;
	text = displayText;
	color = textColor;

	//TODO find out why destroying, recreating, and re-rendering the texture
	//		is the only way for it to show up correctly
	//so now its not happening?!?! but ill leave this here just in case
	//renderTexture();
	renderTexture();
}

TextSurface::~TextSurface(){
	//
}

void TextSurface::setFont(string fontName){
	font = fontName;

	markDirty();
}

void TextSurface::setFontSize(int size){
	fontSize = size;

	markDirty();
}

void TextSurface::setText(string displayText){
	text = displayText;

	markDirty();
}

void TextSurface::setTextPadding(uint pad){
	textPadding = pad;

	markDirty();
}

void TextSurface::setColor(Color textColor){
	color = textColor;

	markDirty();
}

void TextSurface::setBackgroundColor(Color color){
	backgroundColor = color;

	markDirty();
}

void TextSurface::renderTexture(){
	Surface::renderTexture();

	if (texture){
		SDL_DestroyTexture(texture);
		texture = NULL;
	}

	if (!FontManager::getSingleton().hasFont(font, fontSize)){
		FontManager::getSingleton().addFont(font, fontSize);
	}

	if (text.size() > 0){
		SDL_Surface* tempSurf = TTF_RenderText_Solid(FontManager::getSingleton().getFont(font, fontSize), text.c_str(), { color.getRed(), color.getGreen(), color.getBlue() });

		if (tempSurf == NULL){
			Error("Unable to render text", TTF_GetError());

			return;
		}

		width = tempSurf->w + 2 * textPadding;
		height = tempSurf->h + 2 * textPadding;

		SDL_Texture* tex = NULL;

		tex = SDL_CreateTextureFromSurface(WindowManager::getSingleton().getRenderer(), tempSurf);

		if (!tex){
			Error("Unable to create texture from rendered text", SDL_GetError());
		}

		Uint32 pixelFormat = WindowManager::getSingleton().getPixelFormat();

		texture = SDL_CreateTexture(WindowManager::getSingleton().getRenderer(), pixelFormat, SDL_TEXTUREACCESS_TARGET, width, height);
		setBlendMode(BLEND_BLEND);

		SDL_Rect dest = { static_cast<int>(textPadding), static_cast<int>(textPadding), tempSurf->w, tempSurf->h };

		Surface::saveRenderTarget();

		setRenderTarget();
		clear();
		fill(backgroundColor);
		SDL_RenderCopy(WindowManager::getSingleton().getRenderer(), tex, NULL, &dest);

		Surface::restoreRenderTarget();

		SDL_DestroyTexture(tex);

		SDL_FreeSurface(tempSurf);
	}
	else{
		width = MAX(1, 2 * textPadding);
		height = MAX(1, 2 * textPadding);

		Uint32 pixelFormat = WindowManager::getSingleton().getPixelFormat();

		texture = SDL_CreateTexture(WindowManager::getSingleton().getRenderer(), pixelFormat, SDL_TEXTUREACCESS_TARGET, width, height);
		setBlendMode(BLEND_BLEND);

		Surface::saveRenderTarget();

		setRenderTarget();
		clear();
		fill(backgroundColor);

		Surface::restoreRenderTarget();
	}
}