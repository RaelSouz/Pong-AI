#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <string>

struct Font {
    TTF_Font* src = nullptr;
    int size = 0;
};

class Label {
    public:
        Label(Font* font, SDL_Color color);
        ~Label();

        void setFont(Font* font, SDL_Color color);
        int setText(SDL_Renderer* renderer, std::string  text);
        void setPosition(float posX, float posY);
        const SDL_FRect getRect() const;
        void draw(SDL_Renderer* renderer);

    private:
        Font* font;
        SDL_Texture* texture;
        SDL_FRect rect;
        SDL_Color color;
};

namespace utils {
    int initFonts();
    Font* openFont(std::string path, float size);
    void closeFont(Font* font);
}