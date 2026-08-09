#include "engine.h"

#include "screen.h"
#include "graphics.h"

CEngine::CEngine()
{
	m_pScreen = 0;
	m_pNextScreen = 0;

	Init();
	m_Playing = true;
}

CEngine::~CEngine()
{
	delete m_pScreen;
	delete m_pGraphics;
	delete m_pInput;
}

void CEngine::Quit()
{
	m_Playing = false;
}

void CEngine::ChangeScreen(CScreen* nextScreen)
{
	m_pNextScreen = nextScreen;
}

void CEngine::Update()
{
	m_pInput->Update();

	if (m_pNextScreen)
	{
		delete m_pScreen;
		m_pScreen = m_pNextScreen;
		m_pNextScreen = 0;
	}

	if (!m_pScreen) return;

	m_pScreen->Update();
}

void CEngine::Render()
{
	if (!m_pScreen) return;

	m_pScreen->Render();
}

CEngine& Engine()
{
	static CEngine s;
	return s;
}
