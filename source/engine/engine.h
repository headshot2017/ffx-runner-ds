#ifndef ENGINE_H_INCLUDED
#define ENGINE_H_INCLUDED

#include <cstdint>

#include "graphics.h"
#include "input.h"

class CScreen;

class CEngine
{
public:
	CEngine();
	~CEngine();

	inline bool IsPlaying() {return m_Playing;}
	inline CInput* Input() {return m_pInput;}
	inline CGraphics* Graphics() {return m_pGraphics;}
	inline CScreen* Screen() {return m_pScreen;}

	void Quit();
	void ChangeScreen(CScreen* nextScreen);
	void Update();
	void Render();

	// platform specific
	void Init();
	void PlatformPostUpdate();
	void Sleep(uint32_t usec);
	uint32_t GetUSec();

private:
	bool m_Playing;

	CInput* m_pInput;
	CGraphics* m_pGraphics;

	CScreen* m_pScreen;
	CScreen* m_pNextScreen;
};

extern CEngine& Engine();

#endif // ENGINE_H_INCLUDED
