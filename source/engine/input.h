#ifndef INPUT_H_INCLUDED
#define INPUT_H_INCLUDED

class CInput
{
public:
	enum
	{
		NONE   = 0,
		UP     = 1<<0,
		DOWN   = 1<<1,
		LEFT   = 1<<2,
		RIGHT  = 1<<3,
		ACTION = 1<<4,
		OK     = 1<<5,
		BACK   = 1<<6,
		EXIT   = 1<<7,
	};

	CInput() : m_Held(NONE), m_Pressed(NONE), m_Released(NONE) {}
	virtual ~CInput() {}

	virtual void Update() = 0;

	inline int Held() {return m_Held;}
	inline int Pressed() {return m_Pressed;}
	inline int Released() {return m_Released;}

protected:
	int m_Held;
	int m_Pressed;
	int m_Released;
};

#endif // INPUT_H_INCLUDED
