#include "input_sdl2.h"

#include <unordered_map>
#include <SDL2/SDL.h>

static std::unordered_map<SDL_Keycode, int> mappings = {
	{SDLK_UP, CInput::UP},
	{SDLK_DOWN, CInput::DOWN},
	{SDLK_LEFT, CInput::LEFT},
	{SDLK_RIGHT, CInput::RIGHT},
	{SDLK_SPACE, CInput::ACTION},
	{SDLK_z, CInput::OK},
	{SDLK_x, CInput::BACK},
	{SDLK_ESCAPE, CInput::EXIT},
};

void CInput_SDL2::Update()
{
	m_Pressed = m_Released = CInput::NONE;

	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch(event.type)
        {
            case SDL_KEYDOWN:
                if (mappings.count(event.key.keysym.sym))
                {
					m_Pressed |= mappings[event.key.keysym.sym];
					m_Held |= mappings[event.key.keysym.sym];
                }
                break;

            case SDL_KEYUP:
            	if (mappings.count(event.key.keysym.sym))
                {
					m_Released |= mappings[event.key.keysym.sym];
					m_Held &= ~mappings[event.key.keysym.sym];
                }
                break;
        }
	}
}
