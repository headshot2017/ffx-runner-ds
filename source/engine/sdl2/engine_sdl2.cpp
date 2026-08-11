#include <engine/engine.h>

#include <stdio.h>
#include <unistd.h>

#include <SDL2/SDL.h>

#include <engine/sdl2/graphics_sdl2.h>
#include <engine/sdl2/input_sdl2.h>

void CEngine::Init()
{
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO);

	m_pGraphics = new CGraphics_SDL2;
	m_pInput = new CInput_SDL2;

	if (chdir("data/ffx-runner-ds") && chdir("../fat/data/ffx-runner-ds"))
	{
		printf("data/ffx-runner-ds/\nDirectory is missing\nCannot continue\n");
		exit(1);
	}
}

void CEngine::PlatformPostUpdate()
{

}

void CEngine::Sleep(uint32_t usec)
{
	usleep(usec);
}

uint32_t CEngine::GetUSec()
{
	return SDL_GetPerformanceCounter() / (double)SDL_GetPerformanceFrequency() * 1000000;
}
