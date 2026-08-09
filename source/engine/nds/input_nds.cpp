#include "input_nds.h"

#include <unordered_map>
#include <nds.h>

static std::unordered_map<uint32_t, int> mappings = {
	{KEY_UP, CInput::UP},
	{KEY_DOWN, CInput::DOWN},
	{KEY_LEFT, CInput::LEFT},
	{KEY_RIGHT, CInput::RIGHT},
	{KEY_X, CInput::ACTION},
	{KEY_A, CInput::OK},
	{KEY_B, CInput::BACK},
	{KEY_START, CInput::EXIT},
};

void CInput_NDS::Update()
{
	scanKeys();

	uint32_t held = keysHeld();
	uint32_t pressed = keysDown();
	uint32_t released = keysUp();

	m_Held = m_Pressed = m_Released = CInput::NONE;

	for (const auto& key : mappings)
	{
		if (held & key.first) m_Held |= key.second;
		if (pressed & key.first) m_Pressed |= key.second;
		if (released & key.first) m_Released |= key.second;
	}
}
