#include <Objects/Object.h>

Object::Object(SDL_Window* window, SDL_Renderer* renderer, SDL_Rect rect, SDL_Color color) :
    rect(rect), color(color), ptrRenderer(&(*renderer)) {
    SDL_GetWindowSize(window, &win_w, &win_h);
}

Object::~Object() = default;

void Object::setRect(int w, int h, int x, int y) {
    rect.w = w, rect.h = h;
    rect.x = x, rect.y = y;
}

SDL_Rect Object::getRect() {
    return rect;
}

void Object::setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    color.r = r, color.g = g, color.b = b, color.a = a;
}

SDL_Color Object::getColor() {
    return color;
}

void Object::setPos(int x, int y) {
    rect.x = x, rect.y = y;
}

void Object::draw() {
    SDL_SetRenderDrawColor(ptrRenderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(ptrRenderer, &rect);
}