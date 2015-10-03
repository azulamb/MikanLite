#ifndef __MIKAN_LITE
#define __MIKAN_LITE

#define MIKANLITE_VER "0.0.1"
#define	MIKANLITE_VERSION 0,0,1,0
#define MIKANLITE_VERCODE 0

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <crtdbg.h>

#undef CreateFont

extern int Main();

#ifdef _DEBUG
#  define MIKANLITE_DLL "MikanLiteD.dll"
#else
#  define MIKANLITE_DLL "MikanLite.dll"
#endif

/// <summary>First run.</summary>
#define MikanLiteInit struct MIKAN_LITE Mikan;\
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow ){\
	HINSTANCE dll;\
	int( *MLMain )( int( *)( ), HINSTANCE, struct MIKAN_LITE *, HINSTANCE, HINSTANCE, PSTR, int );\
	int( *MLVer )();\
	if ( !( dll = ::LoadLibraryEx( TEXT( MIKANLITE_DLL ), NULL, 0 ) ) ) { fprintf( stderr, "Mikan Lite DLL version is different." ); return 1; }\
	MLVer = ( int( *)() )::GetProcAddress( dll, "MikanLiteVerCode" );\
	if ( MLVer() != MIKANLITE_VERCODE ){ return 1; }\
	MLMain = ( int( *)( int( *)( ), HINSTANCE, struct MIKAN_LITE *, HINSTANCE, HINSTANCE, PSTR, int ) )::GetProcAddress( dll, "MikanLiteMain" );\
	MLMain( Main, dll, &Mikan, hInstance, hPrevInstance, lpCmdLine, nCmdShow );\
	::FreeLibrary( dll );\
	return 0;\
}

/// <summary>MikanLite data.</summary>
struct MIKAN_LITE{
	class MikanLite *system;
	class MikanWindow *window;
	class MikanDraw *draw;
	class MikanInput *input;
};
extern struct MIKAN_LITE Mikan;

/// <summary>
/// GameView
/// GameView is life cycle in MikanLite world.
/// Init() => Loop() called every frames => Final()
/// </summary>
class GameView
{
public:
	/// <summary>Initialization</summary>
	virtual void Init() {}
	/// <summary>Run every frame.</summary>
	/// <returns>Exit program when return false.</returns>
	virtual bool Loop() { return true; }
	/// <summary>Finalization</summary>
	virtual void Final() {}
};

/// <summary>MikanLite System</summary>
class MikanLite
{
public:
	virtual HINSTANCE GetInstance( void ) = 0;
	virtual HINSTANCE GetPrevInstance( void ) = 0;
	virtual PSTR GetCommand( void ) = 0;
	virtual int GetCommandShow( void ) = 0;

	/// <summary>Can get permit screen saver.</summary>
	/// <returns>true = Permit screen saver.</returns>
	virtual bool IsPermitScreenSaver( void ) = 0;
	/// <summary>Permit screen saver.</summary>
	/// <param name="enable">true = Permit screen saver.</param>
	virtual void SetPermitScreenSaver( bool enable = true ) = 0;

	/// <summary>Can get window active.</summary>
	/// <returns>true = Window active.</returns>
	virtual bool IsActive( void ) = 0;
	/// <summary>Set window active.</summary>
	/// <param name="active">true is window active.</param>
	virtual void SetActive( bool active ) = 0;

	virtual bool CannotRunInactiveWindow( void ) = 0;
	virtual void SetInactiveWindow( bool notruninactive = true ) = 0;

	/// <summary>Wait next frame.</summary>
	virtual void WaitNextFrame( void ) = 0;

	/// <summary>Set game loop.</summary>
	/// <param name="run">false = Exit game before next frame.</param>
	virtual void SetGameLoop( bool run = false ) = 0;

	/// <summary>Set GameView.</summary>
	/// <param name="gview">Change GameView when next frame.</param>
	virtual void SetGameView( class GameView *gview ) = 0;
};

enum WINDOWTYPE
{
	/// <summary>Intact.</summary>
	WT_RENEWAL = 0,
	/// <summary>No resize window.</summary>
	WT_DEFAULT = 1,
	/// <summary>Can resize window.</summary>
	WT_NORMAL = 2,
	/// <summary>Pseudo full screen window.</summary>
	WT_NORESIZEFULLSCREEN = 3,
	/// <summary>No frame window.</summary>
	WT_NOFRAME = 4,
	/// <summary>Full screen.</summary>
	WT_FULLSCREEN = 5,
};

class MikanWindow
{
public:
	/// <summary>Create window.</summary>
	/// <param name="nCmdShow">4th arg in WinMain.</param>
	virtual unsigned long Create( int nCmdShow ) = 0;

	/// <summary>Set window from WINDOWTYPE.</summary>
	/// <param name="type">enum WINDOWTYPE value.</param>
	/// <param name="display">Display number.</param>
	virtual void SetWindowType( enum WINDOWTYPE type = WT_RENEWAL, int display = 0 ) = 0;

	/// <summary>Get window class name.</summary>
	/// <returns>Window class name.</returns>
	virtual const wchar_t * GetWindowClassNameEx( void ) = 0;

	/// <summary>Get window name.(Title bar)</summary>
	/// <returns>Window title.</returns>
	virtual const wchar_t * GetTitle( void ) = 0;
	/// <summary>Set window title.(Refrect when call SetWindow())</summary>
	/// <param name="name">Window title.</param>
	virtual void SetTitle( const char * name ) = 0;
	/// <summary>Set window title.(Refrect when call SetWindow())</summary>
	/// <param name="name">Window title.</param>
	virtual void SetTitle( const wchar_t * name ) = 0;

	/// <summary>Set window icon.</summary>
	/// <param name="smalliconnum">Resource number.(Small icon)</param>
	/// <param name="bigiconnum">Resource number.(Big icon)</param>
	virtual void SetIcon( int smalliconnum, int bigiconnum ) = 0;
	/// <summary>Set window icon.</summary>
	/// <param name="instance">Instance.(Have icon resource)</param>
	/// <param name="smalliconnum">Resource number.(Small icon)</param>
	/// <param name="bigiconnum">Resource number.(Big icon)</param>
	virtual void SetIcon( HINSTANCE instance, int smalliconnum, int bigiconnum ) = 0;
	/// <summary>Set window icon.</summary>
	/// <param name="smalliconname">Resource name.(Small icon)</param>
	/// <param name="bigiconname">Resource name.(Big icon)</param>
	virtual void SetIcon( const char *smalliconname, const char *bigiconname ) = 0;
	/// <summary>Set window icon.</summary>
	/// <param name="instance">Instance.(Have icon resource)</param>
	/// <param name="smalliconname">Resource name.(Small icon)</param>
	/// <param name="bigiconname">Resource name.(Big icon)</param>
	virtual void SetIcon( HINSTANCE instance, const char *smalliconname, const char *bigiconname ) = 0;
	/// <summary>Set window icon.</summary>
	/// <param name="smalliconname">Resource name.(Small icon)</param>
	/// <param name="bigiconname">Resource name.(Big icon)</param>
	virtual void SetIcon( const wchar_t *smalliconname, const wchar_t *bigiconname ) = 0;
	/// <summary>Set window icon.</summary>
	/// <param name="instance">Instance.(Have icon resource)</param>
	/// <param name="smalliconname">Resource name.(Small icon)</param>
	/// <param name="bigiconname">Resource name.(Big icon)</param>
	virtual void SetIcon( HINSTANCE instance, const wchar_t *smalliconname, const wchar_t *bigiconname ) = 0;

	/// <summary>Get window style.</summary>
	/// <returns>Window style.</returns>
	virtual unsigned long GetWindowStyle( void ) = 0;
	/// <summary>Set window style.(Refrect when call SetWindow())</summary>
	/// <param name="ws">Window style.</param>
	virtual void SetWindowStyle( unsigned long ws ) = 0;

	/// <summary>Get window style ex.</summary>
	/// <returns>Window style ex.</returns>
	virtual unsigned long GetWindowStyleEx( void ) = 0;
	/// <summary>Set window style ex.(Refrect when call SetWindow())</summary>
	/// <param name="wse">Window style ex.</param>
	virtual void SetWindowStyleEx( unsigned long wse ) = 0;

	/// <summary>Get window width.</summary>
	/// <param name="calc">true = Examine now window state.</param>
	/// <returns>Window width.</returns>
	virtual unsigned int GetWidth( bool calc = false ) = 0;
	/// <summary>Get window height.</summary>
	/// <param name="calc">true = Examine now window state.</param>
	/// <returns>Window height.</returns>
	virtual unsigned int GetHeight( bool calc = false ) = 0;
	/// <summary>Set window width.(Refrect when call SetWindow())</summary>
	/// <param name="width">Window width.</param>
	virtual void SetWidth( unsigned int width ) = 0;
	/// <summary>Set window height.(Refrect when call SetWindow())</summary>
	/// <param name="width">Window height.</param>
	virtual void SetHeight( unsigned int height ) = 0;
	/// <summary>Set window size.(Refrect when call SetWindow())</summary>
	/// <param name="width">Window width.</param>
	/// <param name="height">Window height.</param>
	virtual void SetWindowSize( unsigned int width, unsigned int height ) = 0;
	/// <summary>Set window size from client size.(Refrect when call SetWindow())</summary>
	/// <param name="width">Client width.</param>
	/// <param name="width">Client height.</param>
	virtual void SetScreenSize( unsigned int width, unsigned int height ) = 0;

	/// <summary>Get window x position.</summary>
	/// <param name="calc">true = Examine now window state.</param>
	/// <returns>Window x position.</returns>
	virtual int GetX( bool calc = false ) = 0;
	/// <summary>Get window y position.</summary>
	/// <param name="calc">true = Examine now window state.</param>
	/// <returns>Window y position.</returns>
	virtual int GetY( bool calc = false ) = 0;
	/// <summary>Set window x positoin.(Refrect when call SetWindow())</summary>
	/// <param name="x">Window x position.</param>
	virtual void SetX( int x ) = 0;
	/// <summary>Set window y position.(Refrect when call SetWindow())</summary>
	/// <param name="y">Window y position.</param>
	virtual void SetY( int y ) = 0;
	/// <summary>Set window position.(Refrect when call SetWindow())</summary>
	/// <param name="x">Window x position.</param>
	/// <param name="y">Window y position.</param>
	virtual void SetPosition( int x, int y ) = 0;
	/// <summary>Set window center in display.(Refrect when call SetWindow())</summary>
	virtual void CenteringWindow( void ) = 0;
	/// <summary>Set window center in display.(Refrect when call SetWindow())</summary>
	/// <param name="display">Target display number.</param>
	virtual void CenteringWindow( int display ) = 0;

	/// <summary>Get window handle.</summary>
	/// <returns>Window handle.</returns>
	virtual HWND GetWindowHandle( void ) = 0;
	/// <summary>Set window handle.</summary>
	/// <param name="wh">Window handle.</param>
	virtual void SetWindowHandle( HWND wh ) = 0;
	/// <summary>Get parent window handle.</summary>
	/// <returns>Window handle.</returns>
	virtual HWND GetParentWindowHandle( void ) = 0;
	/// <summary>Set parent window handle.</summary>
	/// <param name="wh">Parent window handle.</param>
	virtual void SetParentWindowHandle( HWND wh ) = 0;

	/// <summary>Get window class ex.</summary>
	/// <returns>Window class ex.</returns>
	virtual WNDCLASSEX GetWindowClassEx( void ) = 0;
	/// <summary>Set window class ex.</summary>
	/// <param name="wc">Window class ex.</param>
	virtual void SetWindowClassEx( WNDCLASSEX wc ) = 0;

	/// <summary>Get menu handle.</summary>
	/// <returns>Menu handle.</returns>
	virtual HMENU GetMenuHandle( void ) = 0;
	/// <summary>Set menu handle.</summary>
	/// <param name="menu">Menu handle.</param>
	virtual void SetMenuHandle( HMENU menu ) = 0;

	/// <summary>Set device context.</summary>
	/// <param name="dc">Device context.</param>
	virtual void SetDeviceContext( HDC dc ) = 0;

	/// <summary>Get instance handle.</summary>
	/// <returns>Instance handle.</returns>
	virtual HINSTANCE GetInstanceHandle( void ) = 0;
	/// <summary>Set instance handle.</summary>
	/// <param name="instance">Instance handle.</param>
	virtual void SetInstanceHandle( HINSTANCE instance ) = 0;

	/// <summary>Set window procedure.</summary>
	/// <param name="winproc">LRESULT CALLBACK Function( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )</param>
	virtual void SetWindowProc( WNDPROC winproc ) = 0;

	/// <summary>Get parameter.</summary>
	/// <returns>Parameter.</returns>
	virtual void * GetParameter( void ) = 0;

	/// <summary>Set function when drag&amp;drop files.</summary>
	/// <param name="func">int Function( int files )</param>
	virtual void SetReceiveDragFileFunction( int( *func )( int files ) = NULL ) = 0;
	/// <summary>Get file name from drag&amp;drop.</summary>
	/// <param name="num">File number.</param>
	/// <returns>File name.</returns>
	virtual const wchar_t * GetReceiveDragFilePath( int num ) = 0;
	/// <summary>Drag&amp;Drop function called in window procedure.</summary>
	/// <param name="hdrop">3rd arg in window procedure.(cast HDROP)</param>
	virtual void ReceiveDragFiles( HDROP hdrop ) = 0;

	/// <summary>Get connect display max.</summary>
	/// <returns>Display max.</returns>
	static int GetDisplayMax( void );
	/// <summary>Calc display number in window.(Largest area)</summary>
	/// <param name="window">Window handle.</param>
	/// <returns>Display number.</returns>
	static int GetDisplayNumber( HWND window = NULL );
	/// <summary>Get display size.</summary>
	/// <param name="display">Display number.</param>
	/// <param name="width">Get width.(int variable pointer)</param>
	/// <param name="height">Get height.(int variable pointer)</param>
	/// <returns>0 = Success. 1 = Invalid display. 2 = Cannot get size.</returns>
	static int GetDisplaySize( long display, int *width = NULL, int *height = NULL );
	/// <summary>Get display position &amp; size.</summary>
	/// <param name="display">Display number.</param>
	/// <param name="x">Get x position.(int variable pointer)</param>
	/// <param name="y">Get y position.(int variable pointer)</param>
	/// <param name="width">Get width.(int variable pointer)</param>
	/// <param name="height">Get height.(int variable pointer)</param>
	/// <returns>0 = Success. 1 = Invalid display. 2 = Cannot get size.</returns>
	static int GetDisplayRect( long display, int *x = NULL, int *y = NULL, int *width = NULL, int *height = NULL );
	/// <summary>Get display rect.</summary>
	/// <param name="display">Display number.</param>
	/// <param name="rect">Get rect.(RECT variable pointer.)</param>
	/// <returns>0 = Success. 1 = Invalid display. 2 = Cannot get size.</returns>
	static int GetDisplayRect( long display, RECT *rect );
	/// <summary>Get desktop width.</summary>
	/// <returns>Desktop width.</returns>
	static int GetDesktopWidth( void );
	/// <summary>Get desktop height.</summary>
	/// <returns>Desktop height.</returns>
	static int GetDesktopHeight( void );
	/// <summary>Get desktop size.</summary>
	/// <param name="width">Get desktop width.(int variable pointer)</param>
	/// <param name="height">Get desktop height.(int variable pointer)</param>
	static void GetDesktopSize( int *width, int *height );
};

class MikanDraw
{
public:
	/// <summary>Get ID2D1Factory.</summary>
	/// <returns>ID2D1Factory.</returns>
	virtual struct ID2D1Factory *GetD2DFactory( void ) = 0;
	/// <summary>Get IDWriteFactory.</summary>
	/// <returns>IDWriteFactory.</returns>
	virtual struct IDWriteFactory *GetWriteFactory( void ) = 0;
	/// <summary>Get ID2D1HwndRenderTarget.</summary>
	/// <returns>ID2D1HwndRenderTarget.</returns>
	virtual struct ID2D1HwndRenderTarget *GetRenderTarget( void ) = 0;

	/// <summary>Clear screen.</summary>
	virtual void ClearScreen( void ) = 0;

	/// <summary>Draw box.</summary>
	/// <param name="x">X position.</param>
	/// <param name="t">Y position.</param>
	/// <param name="w">Width.</param>
	/// <param name="h">Height.</param>
	virtual void DrawBox( int x, int y, int w, int h, unsigned long color ) = 0;

	/// <summary>Create font.</summary>
	/// <param name="font">Font number.(Max 31)</param>
	/// <param name="fontname">Font name.(Default Meiryo)</param>
	/// <param name="size">Font size.(Default 10px)</param>
	/// <param name="color">Font default color.(Default 0xFFFFFFFF = White)</param>
	/// <param name="weight">Font weight.(Default 400)</param>
	/// <param name="italic">Font italic.(Default 0)</param>
	virtual void CreateFont( unsigned int font, const wchar_t *fontname = L"Meiryo", int size = 10, unsigned long color = 0xFFFFFFFF, int weight = 400, int italic = 0 ) = 0;

	/// <summary>Create font.</summary>
	/// <param name="font">Font number.</param>
	/// <param name="x">Draw x position.</param>
	/// <param name="y">Draw y position.</param>
	/// <param name="str">Print string.</param>
	virtual void Print( unsigned int font, int x, int y, const wchar_t *str ) = 0;
	/// <summary>Create font.</summary>
	/// <param name="font">Font number.</param>
	/// <param name="x">Draw x position.</param>
	/// <param name="y">Draw y position.</param>
	/// <param name="color">Font color.</param>
	/// <param name="str">Print string.</param>
	virtual void Print( unsigned int font, int x, int y, unsigned long color, const wchar_t *str ) = 0;
	/// <summary>Create font.</summary>
	/// <param name="font">Font number.</param>
	/// <param name="x">Draw x position.</param>
	/// <param name="y">Draw y position.</param>
	/// <param name="format">Print string.</param>
	virtual void Printf( unsigned int font, int x, int y, const wchar_t *format, ... ) = 0;
	/// <summary>Create font.</summary>
	/// <param name="font">Font number.</param>
	/// <param name="x">Draw x position.</param>
	/// <param name="y">Draw y position.</param>
	/// <param name="color">Font color.</param>
	/// <param name="format">Print string.</param>
	virtual void Printf( unsigned int font, int x, int y, unsigned long color, const wchar_t *format, ... ) = 0;
};

class MikanInput
{
public:
	virtual int GetMouseX( void ) = 0;
	virtual int GetMouseY( void ) = 0;
	virtual void SetMousePosition( int x, int y ) = 0;
	virtual int GetMouseFrame( unsigned int button ) = 0;
};

#endif
