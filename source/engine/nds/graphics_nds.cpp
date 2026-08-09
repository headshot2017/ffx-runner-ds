#include "graphics_nds.h"

#include <nds.h>
#include <engine/nds/texture_nds.h>


CGraphics_NDS::CGraphics_NDS() : CGraphics()
{
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

	glInit();

	glEnable(GL_TEXTURE_2D);

	glViewport(0, 0, 255, 191);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(64, 256.f / 192.f, 0.1f, 40);

	glMaterialf(GL_AMBIENT, RGB15(16, 16, 16));
	glMaterialf(GL_DIFFUSE, RGB15(31, 31, 31));
	glLight(0, RGB15(31,31,31), 0, inttov10(1), 0);
}

CGraphics_NDS::~CGraphics_NDS()
{

}

void CGraphics_NDS::ClearColor(int r, int g, int b)
{
	glClearColor(r >> 3, g >> 3, b >> 3, 31);
	glClearDepth(GL_MAX_DEPTH);
}

void CGraphics_NDS::Clear()
{

}

void CGraphics_NDS::Flush()
{
	glFlush(0);
}

void CGraphics_NDS::SetClientState()
{
	glPolyFmt(POLY_ALPHA(31) | POLY_CULL_BACK | POLY_FORMAT_LIGHT0 | POLY_RENDER_FAR_POLYS);
	glColor3f(1, 1, 1);
}

void CGraphics_NDS::MatrixMode(int mode)
{
	glMatrixMode((GL_MATRIX_MODE_ENUM)mode);
}

void CGraphics_NDS::MatrixIdentity()
{
	glLoadIdentity();
}

void CGraphics_NDS::PushMatrix()
{
	glPushMatrix();
}

void CGraphics_NDS::PopMatrix()
{
	glPopMatrix(1);
}

void CGraphics_NDS::LookAt(int x, int y, int z, int targetX, int targetY, int targetZ, int upX, int upY, int upZ)
{
	gluLookAtf32(x, y, z, targetX, targetY, targetZ, upX, upY, upZ);
}

void CGraphics_NDS::Scale(int x, int y, int z)
{
	glScalef32(x, y, z);
}

void CGraphics_NDS::Translate(int x, int y, int z)
{
	glTranslatef32(x, y, z);
}

void CGraphics_NDS::Rotate(int axis, int x, int y, int z)
{
	glRotatef32i(axis, x, y, z);
}

CTexture* CGraphics_NDS::LoadTextureImpl(std::string filename)
{
	return new CTexture_NDS(this, filename);
}
