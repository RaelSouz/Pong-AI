#include <Ui/Label.h>

Label::Label(Font* font, SDL_Color color) :
    texture(nullptr), color(color), rect({0}) {
    setFont(font, color);
}

Label::~Label() {
    if(texture) SDL_DestroyTexture(texture);
    if(font) font = nullptr;
}

void Label::setFont(Font* font, SDL_Color color) {
    this->font = font;
    this->color = color;
}

int Label::setText(SDL_Renderer* renderer, std::string  text) {
    SDL_Surface* surface;
    if(!(surface = TTF_RenderUTF8_Blended(font->src, text.c_str(), color))) return -1;
    if(texture) SDL_DestroyTexture(texture);
	if(!(texture = SDL_CreateTextureFromSurface(renderer, surface))) {
        SDL_FreeSurface(surface);
		return -1;
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

void Label::draw(SDL_Renderer* renderer) {
    SDL_RenderCopy(renderer, texture, NULL, &rect);
}

int utils::initFonts() {
    return TTF_Init();
}

Font* utils::openFont(std::string path, float size) {
	Font* font = new Font;
    if(!(font->src = TTF_OpenFont(path.c_str(), size))) return NULL;
    font->size = size;
    return font;
}

void utils::closeFont(Font* font) {
    TTF_CloseFont(font->src);
}