#include "MikanLiteLib.h"

void MikanLiteSystem::Update()
{
	if ( !next )
	{
		return;
	}
	if ( now ) { now->Final(); }
	now = next;
	next = NULL;
	now->Init();
}

MikanLiteSystem::MikanLiteSystem()
{
	SetGameLoop( true );
	SetPermitScreenSaver( false );
	now = next = NULL;
	SetGameView( new GameView() );
}

int MikanLiteSystem::Init( HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow )
{

	return 0;
}

int MikanLiteSystem::Main()
{
	MSG msg = {};

	Update();

	while ( gameloop )
	{
		while ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
		{
			/*if (msg.message == WM_QUIT)
			{
			return EXIT_SUCCESS;
			}*/
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}

		//入力の更新

		//ユーザー実行
		if ( ! now->Loop() )
		{
			break;
		}

		Update();
		WaitNextFrame();
	}
	if ( next ) { delete( next ); next = NULL; }
	if ( now )
	{
		now->Final();
		delete( now );
		now = NULL;
	}

	return 0;
}

void MikanLiteSystem::SetPermitScreenSaver( bool enable )
{
	permitscreensaver = enable;
}

bool MikanLiteSystem::IsPermitScreenSaver()
{
	return permitscreensaver;
}

void MikanLiteSystem::WaitNextFrame()
{
}

void MikanLiteSystem::SetGameLoop( bool run )
{
	gameloop = run;
}

void MikanLiteSystem::SetGameView( GameView * gview )
{
	if ( next ) { delete( next ); }
	next = gview;
}
