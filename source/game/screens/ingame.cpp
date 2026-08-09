#include "ingame.h"

#include <fixed.h>
#include <game/world.h>
#include <game/camera.h>
#include <game/entities/player.h>

CScreenInGame::CScreenInGame() : CScreen()
{
	m_pWorld = new CWorld("models/world", "models/skybox");
	m_pPlayer = new CPlayer(m_pWorld, ftof32(-0.0535f * SCALE_VERTICES), 0, ftof32(3.1f * SCALE_VERTICES));

	m_pWorld->GetCamera()->AttachToEntity(m_pPlayer);
}

CScreenInGame::~CScreenInGame()
{
	delete m_pWorld;
	m_pPlayer = 0; // m_pPlayer gets deleted by CWorld destructor
}

void CScreenInGame::Update()
{
	m_pWorld->Update();
}

void CScreenInGame::Render()
{
	m_pWorld->Render();
}
