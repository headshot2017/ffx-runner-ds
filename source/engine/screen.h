#ifndef SCREEN_H_INCLUDED
#define SCREEN_H_INCLUDED

class CScreen
{
public:
	virtual ~CScreen() {}

	virtual void Update() = 0;
	virtual void Render() = 0;
};

#endif // SCREEN_H_INCLUDED
