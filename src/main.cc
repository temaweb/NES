/*
 * This file is part of the NES-6502 distribution (https://github.com/temaweb/NES-6502).
 * Copyright (c) 2021 Artem Okonechnikov.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <SDL.h>
#include "cpu.hpp"

void loop()
{
    SDL_Event e;

    while (true)
    while (SDL_PollEvent(&e))
    {
        switch (e.type)
        {
            case SDL_QUIT:
            case SDL_KEYDOWN:
            case SDL_MOUSEBUTTONDOWN:
                return;
        }
    }
}

int main()
{
    SDL_Init(SDL_INIT_VIDEO);

    auto window = SDL_CreateWindow ("Emulator", 

        // the x position of the window
        SDL_WINDOWPOS_UNDEFINED, 

        // the y position of the window
        SDL_WINDOWPOS_UNDEFINED, 

        800,  // the width of the window, in screen coordinates
        600,  // the height of the window, in screen coordinates
        0     // 0, or one or more SDL_WindowFlags OR'd together
    );

    auto renderer = SDL_CreateRenderer (window, -1, SDL_RENDERER_ACCELERATED);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    loop();

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}