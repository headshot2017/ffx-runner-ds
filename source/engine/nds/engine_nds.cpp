#include <engine/engine.h>

#include <stdio.h>
#include <stdarg.h>
#include <time.h>

#include <nds.h>
#include <fat.h>
#include <filesystem.h>

#include <engine/nds/graphics_nds.h>
#include <engine/nds/input_nds.h>

static int fps = 0;
static int irq_frame_count = 0;

static void handler()
{
	fps = irq_frame_count;
	irq_frame_count = 0;
}

void CEngine::Init()
{
	defaultExceptionHandler();

	m_pGraphics = new CGraphics_NDS;
	m_pInput = new CInput_NDS;

	if (!nitroFSInit(0))
	{
		printf("Failed to init NitroFS\n");
		while (1) swiWaitForVBlank();
	}

	if (chdir("nitro:/data/ffx-runner-ds"))
	{
		printf("nitro:/data/ffx-runner-ds/\nDirectory is missing\nCannot continue\n");
		while (1) swiWaitForVBlank();
	}

	timerStart(0, ClockDivider_1024, timerFreqToTicks_1024(1), handler);
	systemCounterSetup();
}

void CEngine::PlatformPostUpdate()
{
	if (irq_frame_count == 0)
		printf("%d FPS\n", fps);
	irq_frame_count++;
}

void CEngine::Sleep(uint32_t usec)
{
	usleep(usec);
}

uint32_t CEngine::GetUSec()
{
	return systemCounterTicksToUsec(systemCounterGetTicks());
}
