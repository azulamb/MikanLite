#include "MikanLiteWindow.h"
#include "Common.h"

MikanLiteWindow::MikanLiteWindow( HINSTANCE instance, HINSTANCE dll )
{
	SetInstanceHandle( instance );

	SetWindowProc( DefWindowProc );

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.cbSize = sizeof( WNDCLASSEX );
	wcex.hCursor = LoadCursor( NULL, IDC_ARROW );
	wcex.hIcon = LoadIcon( NULL, IDI_APPLICATION );
	wcex.hIconSm = LoadIcon( NULL, IDI_APPLICATION );
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hbrBackground = (HBRUSH)GetStockObject( BLACK_BRUSH );
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = TEXT( MIKANLITEWINDOWCLASSNAME );

	SetWindowType( WT_DEFAULT );

	//GetClassName( TEXT( MIKANLITEWINDOWCLASSNAME ) );

	if ( dll ) { SetIcon( dll, "MIKAN_LITE_SICON", "MIKAN_LITE_BICON" ); }

	SetWindowStyleEx( NULL );

	SetPosition( 0, 0 );

	SetScreenSize( WF_WIDTH, WF_HEIGHT );

	SetWindowHandle( NULL );
	SetParentWindowHandle( NULL );
	SetMenuHandle( NULL );

	SetDeviceContext( NULL );

	dragfilepath[ 0 ] = '\0';
	fullscreen = false;

	name = NULL;
	SetTitle( TEXT( "Title" ) );

	param = NULL;

	drophandle = NULL;
	dragfilesmax = 0;
	SetReceiveDragFileFunction();

	registedwindowclass = false;

	display = 0;
}

MikanLiteWindow::~MikanLiteWindow( void )
{
	free( name );
}

bool MikanLiteWindow::RegistWindowClassEx( void )
{
	unsigned long err;
	unsigned long atom;

	atom = RegisterClassEx( &( GetWindowClassEx() ) );
	if ( atom == NULL )
	{
		err = GetLastError();
		return false;
	}
	registedwindowclass = true;
	return true;
}

unsigned long MikanLiteWindow::Create( int nCmdShow )
{
	unsigned long err;

	if ( !registedwindowclass )
	{
		RegistWindowClassEx();
	}

	if ( parent )
	{
		SetWindowStyle( GetWindowStyle() | WS_CHILD );
	} else
	{
		SetWindowStyle( GetWindowStyle() & ( ~WS_CHILD ) );
	}

	window = CreateWindowEx(
		GetWindowStyleEx(),
		GetWindowClassNameEx(),
		GetTitle(),
		GetWindowStyle(),
		GetX(), GetY(),
		GetWidth(), GetHeight(),
		GetParentWindowHandle(),
		GetMenuHandle(),
		GetInstanceHandle(),
		GetParameter()
		);

	if ( window == NULL )
	{
		err = GetLastError();
		return err;
	}

	ShowWindow( window, nCmdShow );
	UpdateWindow( window );

	return 0;
}

/*void MikanLiteWindow::Init( void )
{
	//SetDeviceContext( CreateCompatibleDC( GetDC( GetWindowHandle() ) ) );
}*/

void MikanLiteWindow::SetWindowType( enum WINDOWTYPE type, int display )
{
	HWND myhwnd = NULL;
	RECT myrect;

	// TODO: Calc window position from display.
	this->display = display;

	switch ( type )
	{
	case WT_DEFAULT:
		SetWindowStyle( WS_DEFAULT );

		SetWindowLong( GetWindowHandle(), GWL_STYLE, GetWindowStyle() );
		flags = SWP_SHOWWINDOW;
		winpos = HWND_TOP;

		SetWindowPos(
			GetWindowHandle(), winpos,
			GetX(), GetY(),
			GetWidth(), GetHeight(),
			flags
			);

		SetPosition( GetX( true ), GetY( true ) );
		return;
	case WT_NORMAL:
		SetWindowStyle( WS_NORMAL );

		SetWindowLong( GetWindowHandle(), GWL_STYLE, GetWindowStyle() );
		flags = SWP_SHOWWINDOW;
		winpos = HWND_TOP;

		SetWindowPos(
			GetWindowHandle(), winpos,
			GetX(), GetY(),
			GetWidth(), GetHeight(),
			flags
			);

		SetPosition( GetX( true ), GetY( true ) );
		return;
	case WT_NOFRAME:
	case WT_FULLSCREEN:
		SetWindowStyle( WS_NOFRAME );

		SetWindowLong( GetWindowHandle(), GWL_STYLE, GetWindowStyle() );

		flags = SWP_SHOWWINDOW;
		winpos = HWND_TOP;

		SetWindowPos(
			GetWindowHandle(), winpos,
			GetX(), GetY(),
			GetWidth(), GetHeight(),
			flags
			);
		return;
	case WT_NORESIZEFULLSCREEN:
		SetWindowStyle( WS_NORESIZEFULLSCREEN );

		if ( GetDisplayRect( display, &myrect ) == 0 )
		{
			SetPosition( myrect.left, myrect.top );
			SetWindowSize( myrect.right - myrect.left, myrect.bottom - myrect.top );
		} else
		{
			myhwnd = GetDesktopWindow();
			if ( myhwnd && GetClientRect( myhwnd, &myrect ) )
			{
				SetPosition( 0, 0 );
				SetWindowSize( myrect.right, myrect.bottom );
			}
		}

		SetWindowLong( GetWindowHandle(), GWL_STYLE, GetWindowStyle() );
		flags = SWP_SHOWWINDOW;
		winpos = HWND_TOPMOST;
		SetWindowPos(
			GetWindowHandle(), winpos,
			GetX(), GetY(),
			GetWidth(), GetHeight(),
			flags
			);
		return;
	default:
		MoveWindow(
			GetWindowHandle(),
			GetX(), GetY(),
			GetWidth(), GetHeight(),
			TRUE
			);
		break;
	}
}

const wchar_t * MikanLiteWindow::GetWindowClassNameEx( void )
{
	return TEXT( MIKANLITEWINDOWCLASSNAME );
}

const wchar_t * MikanLiteWindow::GetTitle( void )
{
	return name;
}

void MikanLiteWindow::SetTitle( const char * name )
{
	size_t len = 0;

	if ( this->name ) { free( this->name ); }
	this->name = Wchar2Char( name );
}

void MikanLiteWindow::SetTitle( const wchar_t * name )
{
	size_t len = wcslen( name ) + 1;

	if ( this->name ) { free( this->name ); }
	this->name = (wchar_t *)malloc( sizeof( wchar_t ) * len );

	wcscpy_s( this->name, len, name );
}

void MikanLiteWindow::SetIcon( int smalliconnum, int bigiconnum )
{
	if ( wcex.hIcon == NULL )
	{
		wcex.hIcon = LoadIcon( NULL, MAKEINTRESOURCE( bigiconnum ) );
		wcex.hIconSm = LoadIcon( NULL, MAKEINTRESOURCE( smalliconnum ) );
	} else
	{
		wcex.hIcon = LoadIcon( GetInstanceHandle(), MAKEINTRESOURCE( bigiconnum ) );
		wcex.hIconSm = LoadIcon( GetInstanceHandle(), MAKEINTRESOURCE( smalliconnum ) );
		SendMessage( GetWindowHandle(), WM_SETICON, ICON_BIG, (WPARAM)( wcex.hIcon ) );
		SendMessage( GetWindowHandle(), WM_SETICON, ICON_SMALL, (WPARAM)( wcex.hIconSm ) );
	}
}

void MikanLiteWindow::SetIcon( HINSTANCE instance, int smalliconnum, int bigiconnum )
{
	if ( wcex.hIcon == NULL )
	{
		wcex.hIcon = LoadIcon( instance, MAKEINTRESOURCE( bigiconnum ) );
		wcex.hIconSm = LoadIcon( instance, MAKEINTRESOURCE( smalliconnum ) );
	} else
	{
		wcex.hIcon = LoadIcon( instance, MAKEINTRESOURCE( bigiconnum ) );
		wcex.hIconSm = LoadIcon( instance, MAKEINTRESOURCE( smalliconnum ) );
		SendMessage( GetWindowHandle(), WM_SETICON, ICON_BIG, (WPARAM)( wcex.hIcon ) );
		SendMessage( GetWindowHandle(), WM_SETICON, ICON_SMALL, (WPARAM)( wcex.hIconSm ) );
	}
}

void MikanLiteWindow::SetIcon( const char * smalliconname, const char * bigiconname )
{
	WCHAR *str;
	if ( wcex.hIcon == NULL )
	{
		wcex.hIcon = LoadIcon( NULL, ( str = Wchar2Char( bigiconname ) ) );
		free( str );
		wcex.hIconSm = LoadIcon( NULL, ( str = Wchar2Char( smalliconname ) ) );
		free( str );
	} else
	{
		wcex.hIcon = LoadIcon( GetInstanceHandle(), ( str = Wchar2Char( bigiconname ) ) );
		free( str );
		wcex.hIconSm = LoadIcon( GetInstanceHandle(), ( str = Wchar2Char( smalliconname ) ) );
		free( str );
		SendMessage( GetWindowHandle(), WM_SETICON, ICON_BIG, (WPARAM)( wcex.hIcon ) );
		SendMessage( GetWindowHandle(), WM_SETICON, ICON_SMALL, (WPARAM)( wcex.hIconSm ) );
	}
}

void MikanLiteWindow::SetIcon( HINSTANCE instance, const char * smalliconname, const char * bigiconname )
{
	WCHAR *str;
	if ( wcex.hIcon == NULL )
	{
		wcex.hIcon = LoadIcon( instance, ( str = Wchar2Char( bigiconname ) ) );
		free( str );
		wcex.hIconSm = LoadIcon( instance, ( str = Wchar2Char( smalliconname ) ) );
		free( str );
	} else
	{
		wcex.hIcon = LoadIcon( instance, ( str = Wchar2Char( bigiconname ) ) );
		free( str );
		wcex.hIconSm = LoadIcon( instance, ( str = Wchar2Char( smalliconname ) ) );
		free( str );
		SendMessage( GetWindowHandle(), WM_SETICON, ICON_BIG, (WPARAM)( wcex.hIcon ) );
		SendMessage( GetWindowHandle(), WM_SETICON, ICON_SMALL, (WPARAM)( wcex.hIconSm ) );
	}
}

void MikanLiteWindow::SetIcon( const wchar_t * smalliconname, const wchar_t * bigiconname )
{
	if ( wcex.hIcon == NULL )
	{
		wcex.hIcon = LoadIcon( NULL, bigiconname );
		wcex.hIconSm = LoadIcon( NULL, smalliconname );
	} else
	{
		wcex.hIcon = LoadIcon( GetInstanceHandle(), bigiconname );
		wcex.hIconSm = LoadIcon( GetInstanceHandle(), smalliconname );
		SendMessage( GetWindowHandle(), WM_SETICON, ICON_BIG, (WPARAM)( wcex.hIcon ) );
		SendMessage( GetWindowHandle(), WM_SETICON, ICON_SMALL, (WPARAM)( wcex.hIconSm ) );
	}
}

void MikanLiteWindow::SetIcon( HINSTANCE instance, const wchar_t * smalliconname, const wchar_t * bigiconname )
{
	if ( wcex.hIcon == NULL )
	{
		wcex.hIcon = LoadIcon( instance, bigiconname );
		wcex.hIconSm = LoadIcon( instance, smalliconname );
	} else
	{
		wcex.hIcon = LoadIcon( instance, bigiconname );
		wcex.hIconSm = LoadIcon( instance, smalliconname );
		SendMessage( GetWindowHandle(), WM_SETICON, ICON_BIG, (WPARAM)( wcex.hIcon ) );
		SendMessage( GetWindowHandle(), WM_SETICON, ICON_SMALL, (WPARAM)( wcex.hIconSm ) );
	}
}

unsigned long MikanLiteWindow::GetWindowStyle( void )
{
	return style;
}

void MikanLiteWindow::SetWindowStyle( unsigned long ws )
{
	style = ws;
}

unsigned long MikanLiteWindow::GetWindowStyleEx( void )
{
	return styleex;
}

void MikanLiteWindow::SetWindowStyleEx( unsigned long wse )
{
	styleex = wse;
}

unsigned int MikanLiteWindow::GetWidth( bool calc )
{
	RECT myrect;

	if ( calc )
	{
		GetClientRect( GetWindowHandle(), &myrect );
		return myrect.right - myrect.left;
	}

	return width;
}

unsigned int MikanLiteWindow::GetHeight( bool calc )
{
	RECT myrect;

	if ( calc )
	{
		GetClientRect( GetWindowHandle(), &myrect );
		return myrect.bottom - myrect.top;
	}

	return height;
}

void MikanLiteWindow::SetWidth( unsigned int width )
{
	if ( width <= 0 )
	{
		width = WF_WIDTH;
	}
	this->width = width;
}

void MikanLiteWindow::SetHeight( unsigned int height )
{
	if ( height <= 0 )
	{
		height = WF_WIDTH;
	}
	this->height = height;
}

void MikanLiteWindow::SetWindowSize( unsigned int width, unsigned int height )
{
	SetWidth( width );
	SetHeight( height );
}

void MikanLiteWindow::SetScreenSize( unsigned int width, unsigned int height )
{
	RECT rect;
	rect.left = 0;
	rect.right = width;
	rect.top = 0;
	rect.bottom = height;

	AdjustWindowRectEx( &rect, GetWindowStyle(), false, GetWindowStyleEx() );
	SetWindowSize( rect.right - rect.left, rect.bottom - rect.top );
	//SetWindowSize( width + GetSystemMetrics( SM_CXEDGE ) + GetSystemMetrics( SM_CXBORDER ) + GetSystemMetrics( SM_CXDLGFRAME ), height + GetSystemMetrics( SM_CYEDGE ) + GetSystemMetrics( SM_CYBORDER ) + GetSystemMetrics( SM_CYDLGFRAME ) + GetSystemMetrics( SM_CYCAPTION ) );
}

int MikanLiteWindow::GetX( bool calc )
{
	RECT myrect;

	if ( calc )
	{
		GetClientRect( GetWindowHandle(), &myrect );
		return myrect.left;
	}

	return x;
}

int MikanLiteWindow::GetY( bool calc )
{
	RECT myrect;

	if ( calc )
	{
		GetClientRect( GetWindowHandle(), &myrect );
		return myrect.top;
	}

	return y;
}

void MikanLiteWindow::SetX( int x )
{
	this->x = x;
}

void MikanLiteWindow::SetY( int y )
{
	this->y = y;
}

void MikanLiteWindow::SetPosition( int x, int y )
{
	SetX( x );
	SetY( y );
}

void MikanLiteWindow::CenteringWindow( void )
{
	return CenteringWindow( display );
}

void MikanLiteWindow::CenteringWindow( int display )
{
	RECT rect;
	int sx, sy;
	if ( MikanWindow::GetDisplayRect( display, &rect ) )
	{
		MikanWindow::GetDisplayRect( 0, &rect );
	}

	sx = ( rect.right - rect.left - GetWidth() ) / 2;
	sy = ( rect.bottom - rect.top - GetHeight() ) / 2;
	if ( sx < 0 ) { sx = 0; }
	if ( sy < 0 ) { sy = 0; }

	SetPosition( rect.left + sx, rect.top + sy );
}

HWND MikanLiteWindow::GetWindowHandle( void )
{
	return window;
}

void MikanLiteWindow::SetWindowHandle( HWND wh )
{
	window = wh;
}

HWND MikanLiteWindow::GetParentWindowHandle( void )
{
	return parent;
}

void MikanLiteWindow::SetParentWindowHandle( HWND wh )
{
	parent = wh;
}

WNDCLASSEX MikanLiteWindow::GetWindowClassEx( void )
{
	return wcex;
}

void MikanLiteWindow::SetWindowClassEx( WNDCLASSEX wc )
{
	wcex = wc;
}

HMENU MikanLiteWindow::GetMenuHandle( void )
{
	return menu;
}

void MikanLiteWindow::SetMenuHandle( HMENU menu )
{
	this->menu = menu;
}

void MikanLiteWindow::SetDeviceContext( HDC dc )
{
	devicecontext = dc;
}

HINSTANCE MikanLiteWindow::GetInstanceHandle( void )
{
	return wcex.hInstance;
}

void MikanLiteWindow::SetInstanceHandle( HINSTANCE hi )
{
	wcex.hInstance = hi;
}

void MikanLiteWindow::SetWindowProc( WNDPROC winproc )
{
	wcex.lpfnWndProc = winproc;
}

void * MikanLiteWindow::GetParameter( void )
{
	return param;
}

void MikanLiteWindow::SetReceiveDragFileFunction( int( *func )( int files ) )
{
	ddfunc = func;
}

const wchar_t * MikanLiteWindow::GetReceiveDragFilePath( int num )
{
	HDROP hdrop = drophandle;
	if ( hdrop && num < dragfilesmax )
	{
		DragQueryFile( hdrop, num, dragfilepath, MAX_PATH );
		return dragfilepath;
	}
	return NULL;
}

void MikanLiteWindow::ReceiveDragFiles( HDROP hdrop )
{
	drophandle = hdrop;
	dragfilesmax = 0;
	if ( ddfunc )
	{
		dragfilesmax = DragQueryFile( hdrop, -1, NULL, 0 );
		ddfunc( dragfilesmax );
	}
	DragFinish( hdrop );
	drophandle = NULL;
	dragfilesmax = 0;
}

