#pragma once
#include <SDL2/SDL.h>
#include <vector>
#include <memory>

namespace floppyradio {
class StarField {
    public:
    StarField(SDL_Renderer* renderer, int stars, int win_width, int win_height);

    /**
     * Each iteration of the starfield moves the stars around
     * And represents them in the new texture to be rendered
     */
    std::vector<SDL_Rect>* Iterate();

    private:
    SDL_Rect PlaceNew();
    int m_win_width, m_win_height;
    SDL_Renderer *m_renderer;
    std::vector<SDL_Rect> m_stars;
    };

    using StarFieldPtr = std::unique_ptr<StarField>;
}
