#include <Objects/Object.h>

Object::Object(SDL_FRect activeArea, SDL_FRect rect, SDL_Color color) :
    rect(rect), color(color), activeArea(activeArea) {}

Object::~Object() = default;

void Object::setRect(float w, float h, float x, float y) {
    rect.w = w, rect.h = h;
    rect.x = x, rect.y = y;
}

SDL_FRect const Object::getRect() const {
    return rect;
}

void Object::setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    color.r = r, color.g = g, color.b = b, color.a = a;
}

const SDL_Color Object::getColor() const {
    return color;
}

void Object::setSpeed(float speed) {
    this->speed = speed;
}

const float Object::getSpeed() const {
    return speed;
}

void Object::setPos(float x, float y) {
    rect.x = x, rect.y = y;
}

void Object::draw(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRectF(renderer, &rect);
}