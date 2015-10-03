#include "MikanLiteSystem.h"

void MikanLiteSystem::Update()
{
	if ( !next )
	{
		return;
	}
	if ( now )
	{
		now->Final();
		delete( now );
	}
	now = next;
	next = NULL;
	now->Init();
}

MikanLiteSystem::MikanLiteSystem()
{
	SetGameLoop( true );
	SetPermitScreenSaver( false );
	SetActive( false );
	SetInactiveWindow( true );
	now = next = NULL;
	SetGameView( new GameView() );
}

int MikanLiteSystem::Init( HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow )
{
	instance = hInstance;
	prev = hPrevInstance;
	cmdline = lpCmdLine;
	cmdshow = nCmdShow;
	return 0;
}

int MikanLiteSystem::Main( class DrawManagement * draw, class InputManagement *input )
{
	MSG msg = {};

	Update();

	while ( gameloop )
	{
		while ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
		{
			//if (msg.message == WM_QUIT) { return EXIT_SUCCESS; }
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}

		if ( CannotRunInactiveWindow() && !IsActive() )
		{
			Sleep( 20 );
			continue;
		}

		// Update input.
		input->Update();

		draw->Begin();
		if ( ! now->Loop() )
		{
			break;
		}

		Update();

		draw->End();
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

HINSTANCE MikanLiteSystem::GetInstance( void )
{
	return instance;
}

HINSTANCE MikanLiteSystem::GetPrevInstance( void )
{
	return prev;
}

PSTR MikanLiteSystem::GetCommand( void )
{
	return cmdline;
}

int MikanLiteSystem::GetCommandShow( void )
{
	return cmdshow;
}

bool MikanLiteSystem::IsPermitScreenSaver( void )
{
	return permitscreensaver;
}

void MikanLiteSystem::SetPermitScreenSaver( bool enable )
{
	permitscreensaver = enable;
}

bool MikanLiteSystem::IsActive( void )
{
	return active;
}

void MikanLiteSystem::SetActive( bool active )
{
	this->active = active;
}

bool MikanLiteSystem::CannotRunInactiveWindow( void )
{
	return notruninactive;
}

void MikanLiteSystem::SetInactiveWindow( bool notruninactive )
{
	this->notruninactive = notruninactive;
}

void MikanLiteSystem::WaitNextFrame( void )
{
	Sleep( 20 );
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
