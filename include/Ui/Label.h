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
        Label(SDL_Renderer* renderer, Font* font, SDL_Color color);
        ~Label();

        void setFont(Font* font, SDL_Color color);
        int setText(std::string  text);
        void setPosition(int posX, int posY);
        SDL_Rect getRect();
        void draw();

    private:
        Font* font;
        SDL_Renderer* ptrRenderer;
        SDL_Texture* texture;
        SDL_Rect rect;
        SDL_Color color;
};

namespace utils {
    int initFonts();
    Font* openFont(std::string path, float size);
    void closeFont(Font* font);
}