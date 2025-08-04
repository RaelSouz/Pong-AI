#include <Ui/Label.h>

Label::Label(SDL_Renderer* renderer, Font* font, SDL_Color color) {
    ptrRenderer = &(*renderer);
    texture = nullptr;
    setFont(font, color);
}

Label::~Label() {
    ptrRenderer = nullptr;
    SDL_DestroyTexture(texture);
    font = nullptr;
}

void Label::setFont(Font* font, SDL_Color color) {
    this->font = font;
    this->color = color;
}

int Label::setText(std::string  text) {
    SDL_Surface* surface = TTF_RenderText_Blended(font->src, text.c_str(), color);

	SDL_DestroyTexture(texture);
	texture = SDL_CreateTextureFromSurface(ptrRenderer, surface);
	if (texture == NULL) {
        std::cerr << "setText: CreateTextureFromSurface error " << SDL_GetError() << std::endl;
		return 1;
	}
	rect.w = surface->w;
	rect.h = surface->h;
    SDL_FreeSurface(surface);
    
    return 0;
}

void Label::setPosition(int posX, int posY) {
    rect.x = posX, rect.y = posY;
}

SDL_Rect Label::getRect() {
    return rect;
}

void Label::draw() {
    SDL_RenderCopy(ptrRenderer, texture, NULL, &rect);
}

int utils::initFonts() {
    if(TTF_Init() == -1) {
        std::cerr << "TTF_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }
    return 0;
}

Font* utils::openFont(std::string path, float size) {
    chdir("..");
	Font* font = new Font;
    font->src = TTF_OpenFont(path.c_str(), size);
	if (font->src == NULL) {
        std::cerr << "TTF_OpenFont Error: " << SDL_GetError() << std::endl;
		return NULL;
	}
    font->size = size;
    return font;
}

void utils::closeFont(Font* font) {
    TTF_CloseFont(font->src);
}