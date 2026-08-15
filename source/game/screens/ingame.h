#ifndef INGAME_H_INCLUDED
#define INGAME_H_INCLUDED

#include <engine/screen.h>
#include <game/world.h>
#include <game/entities/player.h>

class CScreenInGame : public CScreen
{
public:
	CScreenInGame();
	~CScreenInGame();

	void Update();
	void Render();

private:
	CWorld* m_pWorld;
	CPlayer* m_pPlayer;
	b3Recording* m_pRecording;
};

#endif // INGAME_H_INCLUDED
