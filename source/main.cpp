#include <fixed.h>
#include <engine/engine.h>
#include <game/screens/ingame.h>

int main(int argc, char **argv)
{
	Engine().Graphics()->ClearColor(32, 32, 32);
	Engine().ChangeScreen(new CScreenInGame);

	while (Engine().IsPlaying())
	{
		uint32_t start = Engine().GetUSec();

		Engine().Update();

		if (Engine().Input()->Pressed() & CInput::EXIT)
			Engine().Quit();

		// Render 3D scene
		// ---------------

		Engine().Graphics()->Clear();
		Engine().Graphics()->SetClientState();
		Engine().Graphics()->MatrixMode(CGraphics::MAT_MODELVIEW);

		Engine().Graphics()->PushMatrix();

		Engine().Graphics()->Scale(itof32(SCALE_VERTICES), itof32(SCALE_VERTICES), itof32(SCALE_VERTICES));

		Engine().Render();

		Engine().Graphics()->PopMatrix();

		Engine().Graphics()->Flush();

		uint32_t frameTime = Engine().GetUSec() - start;
		printf("%d usec\n", frameTime);
		if (frameTime < 33333) // limit to 30 fps
			Engine().Sleep(33333 - frameTime);

		Engine().PlatformPostUpdate();
		// Synchronize game loop to the screen refresh
		//swiWaitForVBlank();
	}

	return 0;
}
