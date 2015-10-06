#ifndef __MIKAN_LITE
#define __MIKAN_LITE

#define MIKANLITE_VER "0.0.2"
#define	MIKANLITE_VERSION 0,0,2,0
#define MIKANLITE_VERCODE 1

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
	if ( !( dll = ::LoadLibraryEx( TEXT( MIKANLITE_DLL ), NULL, 0 ) ) ) { fprintf( stderr, "%s not found.", MIKANLITE_DLL ); return 1; }\
	MLVer = ( int( *)() )::GetProcAddress( dll, "MikanLiteVerCode" );\
	if ( MLVer() != MIKANLITE_VERCODE ){ fprintf( stderr, "Mikan Lite DLL version is different." ); return 1; }\
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
	/// <summary>Get instance.(1ft Main arg)</summary>
	/// <returns>HINSTANCE</returns>
	virtual HINSTANCE GetInstance( void ) = 0;
	/// <summary>Get prev instance.(2nd Main arg)</summary>
	/// <returns>HINSTANCE</returns>
	virtual HINSTANCE GetPrevInstance( void ) = 0;
	/// <summary>Get command.(3rd Main arg)</summary>
	/// <returns>Command.</returns>
	virtual PSTR GetCommand( void ) = 0;
	/// <summary>Get command show.(4th Main arg)</summary>
	/// <returns>Command show.</returns>
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

	/// <summary>Cannot run when inactive window.</summary>
	/// <returns>teur = cannot run inactive window.</returns>
	virtual bool CannotRunInactiveWindow( void ) = 0;
	/// <summary>Set run when inactive window.</summary>
	/// <returns>HINSTANCE</returns>
	virtual void SetInactiveWindow( bool notruninactive = true ) = 0;

	/// <summary>Set FPSController.</summary>
	/// <param name="fps">class FPSController.</param>
	virtual void SetFPSController( class FPSController *fps ) = 0;
	/// <summary>Get now FPS.</summary>
	/// <returns>FPS.</returns>
	virtual float GetNowFPS( void ) = 0;
	/// <summary>Set FPS.</summary>
	/// <param name="fps">class FPS.</param>
	virtual void SetFPS( int fps ) = 0;
	/// <summary>Wait next frame.</summary>
	virtual void WaitNextFrame( void ) = 0;

	/// <summary>Set game loop.</summary>
	/// <param name="run">false = Exit game before next frame.</param>
	virtual void SetGameLoop( bool run = false ) = 0;

	/// <summary>Set GameView.</summary>
	/// <param name="gview">Change GameView when next frame.</param>
	virtual void SetGameView( class GameView *gview ) = 0;
};

class FPSController
{
public:
	/// <summary>Get now FPS.</summary>
	/// <returns>FPS.</returns>
	virtual float GetNowFPS( void ) = 0;
	/// <summary>Get FPS(config).</summary>
	/// <returns>FPS.</returns>
	virtual int GetFPS( void ) = 0;
	/// <summary>Set FPS.</summary>
	/// <param name="fps">FPS.</param>
	virtual void SetFPS( int fps ) = 0;
	/// <summary>Wait next frame.</summary>
	virtual void Wait( void ) = 0;
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

#define TRC_NONE 0
#define TRC_ZERO 1

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
	/// <param name="x">X position.(Left top)</param>
	/// <param name="t">Y position.(Left top)</param>
	/// <param name="w">Width.</param>
	/// <param name="h">Height.</param>
	/// <param name="color">Color code.(0xAARRGGBB)</param>
	virtual void DrawBox( int x, int y, int w, int h, unsigned long color ) = 0;
	/// <summary>Draw box.</summary>
	/// <param name="cx">X position.(Center)</param>
	/// <param name="ct">Y position.(Center)</param>
	/// <param name="w">Width.</param>
	/// <param name="h">Height.</param>
	/// <param name="color">Color code.(0xAARRGGBB)</param>
	virtual void DrawBoxC( int cx, int cy, int w, int h, unsigned long color ) = 0;

	/// <summary>Create texture.(Load image file.)</summary>
	/// <param name="tex">Texture number.(Max 31)</param>
	/// <param name="filename">Image filename.</param>
	/// <returns>Error code.(HRESULT)</returns>
	virtual long CreateTexture( unsigned int tex, const wchar_t *filename, int trc = TRC_NONE ) = 0;
	virtual unsigned int GetTextureWidth( unsigned int tex ) = 0;
	virtual unsigned int GetTextureHeight( unsigned int tex ) = 0;

	/// <summary>Draw texture.</summary>
	/// <param name="tex">Texture number.(Max 31)</param>
	/// <param name="rx">Read X.</param>
	/// <param name="ry">Read Y.</param>
	/// <param name="w">Width.</param>
	/// <param name="h">Height.</param>
	/// <param name="dx">Draw X.</param>
	/// <param name="dx">Draw Y.</param>
	virtual void DrawTexture( unsigned int tex, int rx, int ry, int w, int h, int dx, int dy ) = 0;
	/// <summary>Draw texture.</summary>
	/// <param name="tex">Texture number.(Max 31)</param>
	/// <param name="rx">Read X.</param>
	/// <param name="ry">Read Y.</param>
	/// <param name="w">Width.</param>
	/// <param name="h">Height.</param>
	/// <param name="dx">Draw center X.</param>
	/// <param name="dx">Draw center Y.</param>
	virtual void DrawTextureC( unsigned int tex, int rx, int ry, int w, int h, int dx, int dy ) = 0;

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
	/// <summary>Get mouse X position.</summary>
	/// <returns>Mouse X posiiton.</returns>
	virtual int GetMouseX( void ) = 0;
	/// <summary>Get mouse Y position.</summary>
	/// <returns>Mouse Y posiiton.</returns>
	virtual int GetMouseY( void ) = 0;
	virtual void SetMousePosition( int x, int y ) = 0;
	/// <summary>Get mouse click frame.</summary>
	/// <param name="button">Mouse button num.(0 = left, 1 = right, 2 = wheel)</param>
	/// <returns>Frame.(0 = no, Greater than 1 = click frame, -1 = Release button. )</returns>
	virtual int GetMouseFrame( unsigned int button ) = 0;

	/// <summary>Get keyboard press frame.</summary>
	/// <param name="keycode">Virtual keycode.(K_A,K_1,K_ENTER,K_ESC,etc ...)</param>
	/// <returns>Frame.(0 = no, Greater than 1 = click frame, -1 = Release button. )</returns>
	virtual int GetKeyFrame( unsigned char keycode ) = 0;
	/// <summary>Get which key pressed.</summary>
	/// <returns>Virtual keycode.</returns>
	virtual unsigned char GetWhichKey( void ) = 0;
};

#define K_BACK     VK_BACK
#define K_TAB      VK_TAB
#define K_ENTER    VK_RETURN
#define K_RETURN   VK_RETURN
#define K_SHIFT    VK_SHIFT
#define K_CTRL     VK_CONTROL
#define K_ALT      VK_MENU
#define K_PAUSE    VK_PAUSE
#define K_CAPSLOCK VK_CAPITAL
#define K_KANA     VK_KANA
#define K_KANJI    VK_KANJI
#define K_ESC      VK_ESCAPE
#define K_CONV     VK_CONVERT
#define K_NONCONV  VK_NONCONVERT
#define K_SPACE    VK_SPACE
#define K_PAGEUP   VK_PRIOR
#define K_PAGEDOWN VK_NEXT
#define K_END      VK_END
#define K_HOME     VK_HOME
#define K_LEFT     VK_LEFT
#define K_UP       VK_UP
#define K_RIGHT    VK_RIGHT
#define K_DOWN     VK_DOWN
#define K_PRINT    VK_PRINT
#define K_0        0x30
#define K_1        0x31
#define K_2        0x32
#define K_3        0x33
#define K_4        0x34
#define K_5        0x35
#define K_6        0x36
#define K_7        0x37
#define K_8        0x38
#define K_9        0x39
#define K_A        0x41
#define K_B        0x42
#define K_C        0x43
#define K_D        0x44
#define K_E        0x45
#define K_F        0x46
#define K_G        0x47
#define K_H        0x48
#define K_I        0x49
#define K_J        0x4A
#define K_K        0x4B
#define K_L        0x4C
#define K_M        0x4D
#define K_N        0x4E
#define K_O        0x4F
#define K_P        0x50
#define K_Q        0x51
#define K_R        0x52
#define K_S        0x53
#define K_T        0x54
#define K_U        0x55
#define K_V        0x56
#define K_W        0x57
#define K_X        0x58
#define K_Y        0x59
#define K_Z        0x5A
#define K_LWIN     VK_LWIN
#define K_RWIN     VK_RWIN
#define K_APPS     VK_APPS
#define K_NUM_0    VK_NUMPAD0
#define K_NUM_1    VK_NUMPAD1
#define K_NUM_2    VK_NUMPAD2
#define K_NUM_3    VK_NUMPAD3
#define K_NUM_4    VK_NUMPAD4
#define K_NUM_5    VK_NUMPAD5
#define K_NUM_6    VK_NUMPAD6
#define K_NUM_7    VK_NUMPAD7
#define K_NUM_8    VK_NUMPAD8
#define K_NUM_9    VK_NUMPAD9
#define K_NUM_MUL  VK_MULTIPLY
#define K_NUM_ADD  VK_ADD
#define K_NUM_SUB  VK_SUBTRACT
#define K_NUM_DEC  VK_DECIMAL
#define K_NUM_DIV  VK_DIVIDE
#define K_F1       VK_F1
#define K_F2       VK_F2
#define K_F3       VK_F3
#define K_F4       VK_F4
#define K_F5       VK_F5
#define K_F6       VK_F6
#define K_F7       VK_F7
#define K_F8       VK_F8
#define K_F9       VK_F9
#define K_F10      VK_F10
#define K_F11      VK_F11
#define K_F12      VK_F12
#define K_F13      VK_F13
#define K_F14      VK_F14
#define K_F15      VK_F15
#define K_F16      VK_F16
#define K_F17      VK_F17
#define K_F18      VK_F18
#define K_F19      VK_F19
#define K_F20      VK_F20
#define K_F21      VK_F21
#define K_F22      VK_F22
#define K_F23      VK_F23
#define K_F24      VK_F24
#define K_NUMLOCK  VK_NUMLOCK
#define K_SCROLL   VK_SCROLL
#define K_LSHIFT   VK_LSHIFT
#define K_RSHIFT   VK_RSHIFT
#define K_LCTRL    VK_LCONTROL
#define K_RCTRL    VK_RCONTROL
#define K_LALT     VK_LMENU
#define K_RALT     VK_RMENU

#endif
