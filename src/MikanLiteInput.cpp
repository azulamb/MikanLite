#include "MikanLiteInput.h"

MikanLiteInput::MikanLiteInput( void )
{
	mouse.point.x = 0;
	mouse.point.y = 0;
	mouse.buf[ 0 ] = mouse.buf[ 1 ] = mouse.buf[ 2 ] = false;
	mouse.frame[ 0 ] = mouse.frame[ 1 ] = mouse.frame[ 2 ] = 0;
	InitKeyboard();
}

void MikanLiteInput::Update( void )
{
	ReflectMouse();
	ReflectKeyboard();
}

void MikanLiteInput::UpdateMouse( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch ( uMsg )
	{
	case WM_LBUTTONDOWN:
		mouse.buf[ 0 ] = true;
		break;
	case WM_LBUTTONUP:
		mouse.buf[ 0 ] = false;
		break;
	case WM_MBUTTONDOWN:
		mouse.buf[ 1 ] = true;
		break;
	case WM_MBUTTONUP:
		mouse.buf[ 1 ] = false;
		break;
	case WM_RBUTTONDOWN:
		mouse.buf[ 2 ] = true;
		break;
	case WM_RBUTTONUP:
		mouse.buf[ 2 ] = false;
		break;
	}
}

void MikanLiteInput::ReflectMouse( void )
{
	GetCursorPos( &mouse.point );
	ScreenToClient( GetActiveWindow(), &mouse.point );

	if ( mouse.buf[ 0 ] )
	{
		++mouse.frame[ 0 ];
	} else if ( 0 < mouse.frame[ 0 ] )
	{
		mouse.frame[ 0 ] = -1;
	} else
	{
		mouse.frame[ 0 ] = 0;
	}

	if ( mouse.buf[ 1 ] )
	{
		++mouse.frame[ 1 ];
	} else if ( 0 < mouse.frame[ 1 ] )
	{
		mouse.frame[ 1 ] = -1;
	} else
	{
		mouse.frame[ 1 ] = 0;
	}

	if ( mouse.buf[ 2 ] )
	{
		++mouse.frame[ 2 ];
	} else if ( 0 < mouse.frame[ 2 ] )
	{
		mouse.frame[ 2 ] = -1;
	} else
	{
		mouse.frame[ 2 ] = 0;
	}
}

int MikanLiteInput::GetMouseX( void )
{
	return mouse.point.x;
}

int MikanLiteInput::GetMouseY( void )
{
	return mouse.point.y;
}

void MikanLiteInput::SetMousePosition( int x, int y )
{
	mouse.point.x = x;
	mouse.point.y = y;
	ClientToScreen( GetActiveWindow(), &mouse.point );
	SetCursorPos( mouse.point.x, mouse.point.y );
}

int MikanLiteInput::GetMouseFrame( unsigned int button )
{
	if ( 3 < button ) { return 0; }
	return mouse.frame[ button ];
}

void MikanLiteInput::UpdateKeyboard( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch ( uMsg )
	{
	case WM_KEYDOWN:
		key.buf[ ( key.last = wParam ) ] = true;
		break;
	case WM_KEYUP:
		key.buf[ wParam ] = false;
		break;
	}
}

void MikanLiteInput::InitKeyboard( void )
{
	int i;
	key.last = 0;
	for ( i = 0; i < 0xFF; ++i )
	{
		key.buf[ i ] = false;
		key.frame[ i ] = 0;
	}
}

void MikanLiteInput::ReflectKeyboard( void )
{
	int i;
	for ( i = 0; i < 0xFF; ++i )
	{
		if ( key.buf[ i ] )
		{
			++key.frame[ i ];
		} else if ( 0 < key.frame[ i ] )
		{
			key.frame[ i ] = -1;
		} else
		{
			key.frame[ i ] = 0;
		}
	}
}

int MikanLiteInput::GetKeyFrame( unsigned char keycode )
{
	return key.frame[ keycode ];
}

unsigned char MikanLiteInput::GetWhichKey( void )
{
	return key.buf[ key.last ] ? key.last : 0;
}
