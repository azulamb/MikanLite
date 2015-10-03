#ifndef __MIKAN_LITE_FPSCONTROLL
#define __MIKAN_LITE_FPSCONTROLL

#include "Common.h"

class MikanLiteFPSController :public FPSController
{
private:
	int fps;
	float nowfps;
	unsigned long before;
	int frame;
public:
	MikanLiteFPSController();
	virtual float GetNowFPS( void );
	virtual int GetFPS( void );
	virtual void SetFPS( int fps );
	virtual void Wait( void );
};

#endif