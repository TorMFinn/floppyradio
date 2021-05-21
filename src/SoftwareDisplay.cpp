/**
 * Software Rendered GUI/Display for visualizing
 * Module playback and player state
 * Copyright (C) Tor Morten Finnesand 2021
 */

#include "floppyradio/SoftwareDisplay.hpp"
#include "StarField.hpp"

#include <spdlog/spdlog.h>

#include <SDL2/SDL.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <thread>

namespace floppyradio {
    
    struct SoftDisplay::Data {
        Data() {
            if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0) {
                throw std::runtime_error(std::string("Failed to init SDL") + SDL_GetError());
            }

            m_win = SDL_CreateWindow("floppyradio",
                                     SDL_WINDOWPOS_UNDEFINED,
                                     SDL_WINDOWPOS_UNDEFINED,
                                     640, 480, 0);
            if (!m_win) {
                throw_err("Failed to create window");
            }

            m_renderer = SDL_CreateRenderer(m_win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            if (!m_renderer) {
                throw_err("Failed to create renderer");
            }

            m_star_field = std::make_unique<StarField>(m_renderer, 100, 640, 480);
        }

        ~Data() {
            SDL_DestroyRenderer(m_renderer);
            SDL_DestroyWindow(m_win);
            SDL_QuitSubSystem(SDL_INIT_VIDEO);
        }

        void throw_err(const std::string &msg) {
            throw std::runtime_error(msg);
        }

        void HandleKeyDown_(const SDL_KeyboardEvent &event) {
            switch (event.keysym.sym) {
            case SDLK_DOWN:
                spdlog::info("KEY DOWN");
                break;
            case SDLK_UP:
                spdlog::info("KEY UP");
                break;
            case SDLK_LEFT:
                spdlog::info("KEY LEF");
                break;
            case SDLK_RIGHT:
                spdlog::info("KEY RIGHT");
                break;
            }
        }

        SDL_Window *m_win;
        SDL_Renderer *m_renderer;
        bool quit;

        // Effects
        StarFieldPtr m_star_field;
    };

    SoftDisplay::SoftDisplay()
        : m_data(new Data()) {
    }

    SoftDisplay::~SoftDisplay() = default;

    void SoftDisplay::SetTrackName(const std::string &name) {
    }

    void SoftDisplay::SetDuration(int seconds) {
    }

    void SoftDisplay::SetCurrentTime(int seconds) {
    }

    void SoftDisplay::Start() {
        m_data->quit = false;

        SDL_Event event;
        while(!m_data->quit) {
            while(SDL_PollEvent(&event)) {
                switch (event.type) {
                case SDL_QUIT:
                    m_data->quit = true;
                    break;
                case SDL_KEYDOWN:
                    m_data->HandleKeyDown_(event.key);
                    break;
                }
            }

            auto stars = m_data->m_star_field->Iterate();

            SDL_SetRenderDrawColor(m_data->m_renderer, 0, 0, 0, 0xFF);
            SDL_RenderClear(m_data->m_renderer);

            // White stars yo
            SDL_SetRenderDrawColor(m_data->m_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
            SDL_RenderFillRects(m_data->m_renderer, stars->data(), stars->size());
            SDL_RenderDrawRects(m_data->m_renderer, stars->data(), stars->size());
            SDL_RenderPresent(m_data->m_renderer);
        }
    }
}
