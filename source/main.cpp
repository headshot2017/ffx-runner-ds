#include <stdio.h>
#include <time.h>
#include <sys/stat.h>

#include <nds.h>
#include <fat.h>
#include <filesystem.h>

#include "glext.h"
#include "world.h"
#include "entities/player.h"
#include "camera.h"

static int fps = 0;
static int irq_frame_count = 0;

static void handler()
{
	fps = irq_frame_count;
	irq_frame_count = 0;
}

int main(int argc, char **argv)
{
	defaultExceptionHandler();

	videoSetMode(MODE_0_3D);
	videoSetModeSub(MODE_0_2D);
	vramSetBankA(VRAM_A_TEXTURE);
	vramSetBankB(VRAM_B_TEXTURE);
	vramSetBankC(VRAM_C_TEXTURE);
	vramSetBankD(VRAM_D_TEXTURE);
	vramSetBankE(VRAM_E_TEX_PALETTE);
	vramSetBankF(VRAM_F_TEX_PALETTE);
	vramSetBankG(VRAM_G_TEX_PALETTE);
	vramSetBankH(VRAM_H_SUB_BG);

	// Setup sub screen for the text console
	consoleInit(NULL, 0, BgType_Text4bpp, BgSize_T_256x256, 0, 1, false, true);

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

	glInit();

	glEnable(GL_TEXTURE_2D);
	//glEnable(GL_ANTIALIAS);

	glClearColor(4, 4, 4, 31);
	//glClearPolyID(63);

	glClearDepth(0x7FFF);

	glViewport(0, 0, 255, 191);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70, 256.f / 192.f, 0.1f, 40);

	glMaterialf(GL_AMBIENT, RGB15(16, 16, 16));
    glMaterialf(GL_DIFFUSE, RGB15(31, 31, 31));

	CWorld* world = new CWorld("models/world", "textures/cielo_giorno.png");
	CPlayer* player = new CPlayer(world, floattof32(-0.0535f * SCALE_VERTICES), floattof32(0.1f * SCALE_VERTICES), floattof32(3.1f * SCALE_VERTICES));
	CCamera* camera = new CCamera;

	camera->AttachToEntity(player);
	camera->SnapToTarget();

	timerStart(0, ClockDivider_1024, timerFreqToTicks_1024(1), handler);
	systemCounterSetup();

	while (1)
	{
		u32 start = systemCounterGetTicks();

		// Print some text in the demo console
		// -----------------------------------

		/*
		consoleClear();
		printf("%d %d %d    %d\n", player->GetX(), player->GetY(), player->GetZ(), player->GetFaceAngle());
		*/

		// Handle user input
		// -----------------

		scanKeys();

		uint16_t keys = keysHeld();

		if (keys & KEY_START)
			break;

		world->Update();

		// Render 3D scene
		// ---------------

		glMatrixMode(GL_MODELVIEW);

		glPushMatrix();

		glScalef32(inttof32(SCALE_VERTICES), inttof32(SCALE_VERTICES), inttof32(SCALE_VERTICES));

		camera->Update();

		glPolyFmt(POLY_ALPHA(31) | POLY_CULL_BACK | POLY_FORMAT_LIGHT0 | POLY_RENDER_FAR_POLYS);
		glLight(0, RGB15(31,31,31), 0, inttov10(1), 0);

		glColor3f(1, 1, 1);

		world->Render();

		glPopMatrix(1);

		glFlush(0);

		u32 frameTime = systemCounterTicksToUsec(systemCounterGetTicks() - start);
		printf("%d usec\n", frameTime);
		if (frameTime < 33333) // limit to 30 fps
			usleep(33333 - frameTime);

		// Synchronize game loop to the screen refresh
		//swiWaitForVBlank();
		if (irq_frame_count == 0)
			printf("%d FPS\n", fps);
		irq_frame_count++;
	}

	return 0;
}
