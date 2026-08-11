#include "graphics_sdl2.h"

#include <GL/glew.h>
#include <GL/glu.h>

#include <fixed.h>
#include <engine/sdl2/texture_sdl2.h>


CGraphics_SDL2::CGraphics_SDL2() : CGraphics()
{
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 2 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY );

	m_Window = SDL_CreateWindow(
        "ffx-runner-ds",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        640, 480,
        SDL_WINDOW_OPENGL
    );

    m_Context = SDL_GL_CreateContext(m_Window);
    SDL_GL_SetSwapInterval(1);

    glewExperimental = GL_TRUE;
	glewInit();

	//glEnable(GL_TEXTURE_2D);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	glDepthMask( GL_TRUE );
	glDepthFunc( GL_LEQUAL );
	glEnable( GL_DEPTH_TEST );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glEnable( GL_BLEND );
	glColor4f(1,1,1,1);

	glViewport(0, 0, 640, 480);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(64, 256.f / 192.f, 0.1f, 40);

	//glMaterialf(GL_AMBIENT, RGB15(16, 16, 16));
	//glMaterialf(GL_DIFFUSE, RGB15(31, 31, 31));
	//glLight(0, RGB15(31,31,31), 0, inttov10(1), 0);
	// create lighting on the scene
	GLfloat light_position[] = { 0, 1, 0, 0 };
	GLfloat light_diffuse[] = { 1, 1, 1, 1 };
	GLfloat light_model[] = { 0.5f, 0.5f, 0.5f, 1 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light_model);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}

CGraphics_SDL2::~CGraphics_SDL2()
{
	SDL_GL_DeleteContext( m_Context );
    SDL_DestroyWindow( m_Window );
}

void CGraphics_SDL2::ClearColor(int r, int g, int b)
{
	glClearColor(r / 255.f, g / 255.f, b / 255.f, 1.f);
}

void CGraphics_SDL2::Clear()
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}

void CGraphics_SDL2::Flush()
{
	SDL_GL_SwapWindow( m_Window );
}

void CGraphics_SDL2::SetClientState()
{
	glColor3f(1, 1, 1);
}

void CGraphics_SDL2::MatrixMode(int mode)
{
	glMatrixMode(mode);
}

void CGraphics_SDL2::MatrixIdentity()
{
	glLoadIdentity();
}

void CGraphics_SDL2::PushMatrix()
{
	glPushMatrix();
}

void CGraphics_SDL2::PopMatrix()
{
	glPopMatrix();
}

void CGraphics_SDL2::LookAt(int x, int y, int z, int targetX, int targetY, int targetZ, int upX, int upY, int upZ)
{
	gluLookAt(f32tof(x), f32tof(y), f32tof(z), f32tof(targetX), f32tof(targetY), f32tof(targetZ), f32tof(upX), f32tof(upY), f32tof(upZ));
}

void CGraphics_SDL2::Scale(int x, int y, int z)
{
	glScalef(f32tof(x), f32tof(y), f32tof(z));
}

void CGraphics_SDL2::Translate(int x, int y, int z)
{
	glTranslatef(f32tof(x), f32tof(y), f32tof(z));
}

void CGraphics_SDL2::Rotate(int axis, int x, int y, int z)
{
	glRotatef(axis / (float)DEGREES_IN_CIRCLE * 360.0f, f32tof(x), f32tof(y), f32tof(z));
}

CTexture* CGraphics_SDL2::LoadTextureImpl(std::string filename)
{
	return new CTexture_SDL2(this, filename);
}
