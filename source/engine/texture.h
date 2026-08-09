#ifndef TEXTURE_H_INCLUDED
#define TEXTURE_H_INCLUDED

#include <cstdint>
#include <string>

class CGraphics;

class CTexture
{
public:
	CTexture(CGraphics* graphics, std::string filename) : m_pGraphics(graphics), m_w(0), m_h(0) {}
	virtual ~CTexture() {}

	virtual void Bind() = 0;

	uint32_t Width() {return m_w;}
	uint32_t Height() {return m_h;}

protected:
	CGraphics* m_pGraphics;

	uint32_t m_w;
	uint32_t m_h;
};

#endif // TEXTURE_H_INCLUDED
