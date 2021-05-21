#include "StarField.hpp"
#include <random>
#include <ctime>

namespace floppyradio {
    StarField::StarField(SDL_Renderer* renderer, int stars, int win_width, int win_height) {
        m_renderer = renderer;
        m_win_width = win_width;
        m_win_height = win_height;
        
        std::srand(std::time(0));
        for (int i; i < stars; i++) {
            m_stars.push_back(PlaceNew());
        }
    }

    std::vector<SDL_Rect>* StarField::Iterate() {
        // Update positions
        for (auto &star : m_stars) {
            if (star.x <= (m_win_width / 2) && star.y >= m_win_height / 2) {
                // Bottom Left Quadrant
                star.x -= 1;
                    star.y += 1;
            } else if (star.x >= (m_win_width / 2) && star.y >= m_win_height / 2) {
                // Bottom Right Quadrant
                star.x += 1;
                star.y += 1;
            } else if (star.x <= (m_win_width / 2) && star.y <= m_win_height / 2) {
                // Top Left quadrant
                star.x -= 1;
                star.y -= 1;
            } else if (star.x >= (m_win_width / 2) && star.y <= m_win_height / 2) {
                star.x += 1;
                star.y -= 1;
            }
            
            // If no longer in view, create a new position around origin
            if (star.x > m_win_width || star.x < 0) {
                star = PlaceNew();
            } else if (star.y < 0 || star.y > m_win_height) {
                star = PlaceNew();
            }
        }
        return &m_stars;
    }

    SDL_Rect StarField::PlaceNew() {
        SDL_Rect star;
        // Stars are 2x2 squares
        int size = 1 + std::rand() % 3;
        star.h = size;
        star.w = size;
        
        star.x = std::rand() % m_win_width - 2;
        star.y = std::rand() % m_win_height - 2;
        return star;
    }
}
