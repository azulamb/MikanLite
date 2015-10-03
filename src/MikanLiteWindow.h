#ifndef __MIKAN_LITE_WINDOW
#define __MIKAN_LITE_WINDOW

#include "Common.h"

#define MIKANLITEWINDOWCLASSNAME "MIKANLITEWINDOW"

#define WF_WIDTH 640
#define WF_HEIGHT 480

//#define WS_DEFAULT WS_OVERLAPPED|WS_VISIBLE |WS_BORDER | WS_CAPTION
//#define WS_DEFAULT WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_BORDER | WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX
//#define WS_DEFAULT WS_VISIBLE | WS_BORDER | WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX
#define WS_DEFAULT            WS_VISIBLE | WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX
#define WS_NORMAL             WS_OVERLAPPEDWINDOW | WS_VISIBLE
#define WS_NORESIZEFULLSCREEN WS_POPUP | WS_VISIBLE
#define WS_NOFRAME            WS_NORESIZEFULLSCREEN
#define WS_FULLSCREEN         WS_NORESIZEFULLSCREEN

class MikanLiteWindow :public MikanWindow
{
private:
	HWND parent;
	HWND window;

	HMENU menu;
	HDC devicecontext;

	WNDCLASSEX wcex;

	WCHAR *name;
	long styleex;

	unsigned int width, height;
	int x, y;

	void *param;

	unsigned long style;

	unsigned int flags;
	HWND winpos;

	HDROP drophandle;
	int dragfilesmax;
	int( *ddfunc )( int files );
	wchar_t dragfilepath[ MAX_PATH ];

	bool fullscreen;

	bool registedwindowclass;

	int display;
public:
	MikanLiteWindow( HINSTANCE instance, HINSTANCE dll );
	virtual ~MikanLiteWindow( void );

	virtual bool RegistWindowClassEx( void );

	virtual unsigned long Create( int nCmdShow );

	//virtual void Init( void );

	virtual void SetWindowType( enum WINDOWTYPE type = WT_RENEWAL, int display = 0 );

	virtual const wchar_t * GetWindowClassNameEx( void );

	virtual const wchar_t * GetTitle( void );
	virtual void SetTitle( const char * name );
	virtual void SetTitle( const wchar_t * name );

	virtual void SetIcon( int smalliconnum, int bigiconnum );
	virtual void SetIcon( HINSTANCE instance, int smalliconnum, int bigiconnum );
	virtual void SetIcon( const char *smalliconname, const char *bigiconname );
	virtual void SetIcon( HINSTANCE instance, const char *smalliconname, const char *bigiconname );
	virtual void SetIcon( const wchar_t *smalliconname, const wchar_t *bigiconname );
	virtual void SetIcon( HINSTANCE instance, const wchar_t *smalliconname, const wchar_t *bigiconname );

	virtual unsigned long GetWindowStyle( void );
	virtual void SetWindowStyle( unsigned long ws );

	virtual unsigned long GetWindowStyleEx( void );
	virtual void SetWindowStyleEx( unsigned long wse );

	virtual unsigned int GetWidth( bool calc = false );
	virtual unsigned int GetHeight( bool calc = false );
	virtual void SetWidth( unsigned int width );
	virtual void SetHeight( unsigned int height );
	virtual void SetWindowSize( unsigned int width, unsigned int height );
	virtual void SetScreenSize( unsigned int width, unsigned int height );

	virtual int GetX( bool calc = false );
	virtual int GetY( bool calc = false );
	virtual void SetX( int x );
	virtual void SetY( int y );
	virtual void SetPosition( int x, int y );
	virtual void CenteringWindow( void );
	virtual void CenteringWindow( int display );

	virtual HWND GetWindowHandle( void );
	virtual void SetWindowHandle( HWND wh );
	virtual HWND GetParentWindowHandle( void );
	virtual void SetParentWindowHandle( HWND wh );

	virtual WNDCLASSEX GetWindowClassEx( void );
	virtual void SetWindowClassEx( WNDCLASSEX wc );

	virtual HMENU GetMenuHandle( void );
	virtual void SetMenuHandle( HMENU menu );

	virtual void SetDeviceContext( HDC dc );

	virtual HINSTANCE GetInstanceHandle( void );
	virtual void SetInstanceHandle( HINSTANCE instance );

	virtual void SetWindowProc( WNDPROC winproc );

	virtual void * GetParameter( void );

	virtual void SetReceiveDragFileFunction( int( *func )( int files ) = NULL );
	virtual const wchar_t * GetReceiveDragFilePath( int num );
	virtual void ReceiveDragFiles( HDROP hdrop );
};

#endif
