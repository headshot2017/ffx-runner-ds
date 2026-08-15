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

	m_pRecording = b3CreateRecording( 0 );    // 0 = default capacity (64 KiB)
	b3World_StartRecording( m_pWorld->GetB3World(), m_pRecording );        // snapshots the world, then logs calls
}

CScreenInGame::~CScreenInGame()
{
	b3World_StopRecording( m_pWorld->GetB3World() );
	b3SaveRecordingToFile( m_pRecording, "session.b3rec" );
	b3DestroyRecording( m_pRecording );

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
