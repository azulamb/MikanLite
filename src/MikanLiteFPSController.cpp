#include "MikanLiteFPSController.h"

#pragma comment(lib, "winmm.lib")

MikanLiteFPSController::MikanLiteFPSController()
{
	SetFPS( 30 );
	nowfps = 0;
}

float MikanLiteFPSController::GetNowFPS( void )
{
	return nowfps;
}

int MikanLiteFPSController::GetFPS( void )
{
	return fps;
}

void MikanLiteFPSController::SetFPS( int fps )
{
	frame = 0;
	before = timeGetTime();
	this->fps = fps;
}

void MikanLiteFPSController::Wait( void )
{
	unsigned long next, now, progress;
	now = timeGetTime();
	if ( ++frame == fps )
	{
		frame = 0;
		nowfps = 1000.0f / ( ( now - before ) / fps );
		before = timeGetTime();
	}
	next = (unsigned long)( frame * 1000.0 / fps );
	progress = now - before;
	if ( next < progress )
	{
		Sleep( 0 );
		return;
	}
	Sleep( next - progress );
}
