#include "MikanLite.h"

int MikanWindow::GetDisplayMax( void )
{
	return GetSystemMetrics( SM_CMONITORS );
}

int MikanWindow::GetDisplayNumber( HWND window )
{
	int max, i, ret = 0, area[ 2 ] = { 0, 0 };
	RECT win, disp;

	if ( window == NULL ) { return 0; }

	GetWindowRect( window, &win );

	max = GetDisplayMax();

	for ( i = 0; i < max; ++i )
	{
		GetDisplayRect( i, &disp );
		if ( win.right < disp.left ||
			disp.right < win.left ||
			win.bottom < disp.top ||
			disp.bottom < win.top )
		{
			continue;
		}

		if ( disp.left < win.left ) { disp.left = win.left; }
		if ( win.right < disp.right ) { disp.right = win.right; }
		if ( win.top < disp.top ) { disp.top = win.top; }
		if ( win.bottom < disp.bottom ) { disp.bottom = win.bottom; }

		area[ 1 ] = ( disp.right - disp.left ) * ( disp.bottom - disp.top );
		if ( area[ 0 ] < area[ 1 ] )
		{
			area[ 0 ] = area[ 1 ];
			ret = i;
		}
	}

	return ret;
}

int MikanWindow::GetDisplaySize( long display, int * width, int * height )
{
	return GetDisplayRect( display, NULL, NULL, width, height );
}

int MikanWindow::GetDisplayRect( long display, int * x, int * y, int * width, int * height )
{
	DISPLAY_DEVICE disdev;
	DEVMODE devmode;

	if ( x ) { *x = 0; }
	if ( y ) { *y = 0; }
	if ( width ) { *width = 0; }
	if ( height ) { *height = 0; }

	memset( &disdev, 0, sizeof( DISPLAY_DEVICE ) );
	disdev.cb = sizeof( DISPLAY_DEVICE );
	if ( EnumDisplayDevices( NULL, display, &disdev, 0 ) == 0 )
	{
		// 不正なディスプレイ番号
		return 1;
	}

	memset( &devmode, 0, sizeof( DEVMODE ) );
	devmode.dmSize = sizeof( DEVMODE );
	if ( EnumDisplaySettingsEx( disdev.DeviceName, /*ENUM_CURRENT_SETTINGS*/ENUM_REGISTRY_SETTINGS, &devmode, EDS_RAWMODE ) == 0 )
	{
		// 設定を取得できなかった
		return 2;
	}
	if ( x ) { *x = devmode.dmPosition.x; }
	if ( y ) { *y = devmode.dmPosition.y; }
	if ( width ) { *width = devmode.dmPelsWidth; }
	if ( height ) { *height = devmode.dmPelsHeight; }

	return 0;
}

int MikanWindow::GetDisplayRect( long display, RECT * rect )
{
	int ret, x, y, w, h;
	if ( rect == NULL ) { return 3; }
	if ( ( ret = GetDisplayRect( display, &x, &y, &w, &h ) ) ) { return ret; }
	rect->left = x;
	rect->top = y;
	rect->right = x + w;
	rect->bottom = y + h;
	return 0;
}

int MikanWindow::GetDesktopWidth( void )
{
	int w;
	MikanWindow::GetDesktopSize( &w, NULL );
	return w;
}

int MikanWindow::GetDesktopHeight( void )
{
	int h;
	MikanWindow::GetDesktopSize( NULL, &h );
	return h;
}

void MikanWindow::GetDesktopSize( int * width, int * height )
{
	HWND myhwnd;
	RECT myrect;

	myhwnd = GetDesktopWindow();

	if ( myhwnd && GetClientRect( myhwnd, &myrect ) )
	{
		if ( width ) { *width = myrect.right; }
		if ( height ) { *height = myrect.bottom; }
	}
}
