#include "MikanLiteSystem.h"
#include "MikanLiteWindow.h"
#include "MikanLiteDraw.h"
#include "MikanLiteInput.h"

#define EXPORT extern "C" __declspec(dllexport)

struct MIKAN_LITE _Mikan = {};

BOOL WINAPI DllMain( HINSTANCE hDLL, DWORD dwReason, LPVOID lpReserved )
{
	return TRUE;
}

LRESULT CALLBACK WindowProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch ( uMsg )
	{
	case WM_INPUT:
		( ( class MikanLiteInput * )_Mikan.input )->UpdateKeyboard();
		break;
	//case WM_LBUTTONDBLCLK:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	//case WM_MBUTTONDBLCLK:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	//case WM_RBUTTONDBLCLK:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
		( ( class MikanLiteInput * )_Mikan.input )->UpdateMouse( hWnd, uMsg, wParam, lParam );
		break;
	case WM_TOUCH:
		break;
	case WM_ACTIVATE:
		if ( LOWORD( wParam ) == WA_INACTIVE )
		{
			_Mikan.system->SetActive( false );
		} else
		{
			_Mikan.system->SetActive( true );
		}
		break;
	case WM_SYSCOMMAND:
		if ( wParam == SC_SCREENSAVE && _Mikan.system->IsPermitScreenSaver() )
		{
			return 1; // Cannot run Screen saver
		}
		break;
	case WM_CLOSE:
	case WM_DESTROY:
		//PostQuitMessage( 0 );
		_Mikan.system->SetGameLoop();
		break;
	case WM_PAINT:
		break;
	case WM_SIZE:
		switch ( wParam )
		{
		case SIZE_RESTORED:
			break;
		case SIZE_MAXIMIZED:
			break;
		}
		break;
	case WM_CREATE:
		DragAcceptFiles( hWnd, TRUE );
		break;
	case WM_DROPFILES:
		_Mikan.window->ReceiveDragFiles( (HDROP)wParam );
		break;
	}
	return DefWindowProc( hWnd, uMsg, wParam, lParam );
}

EXPORT int MikanLiteVerCode( void ) { return MIKANLITE_VERCODE; }

EXPORT int MikanLiteMain( int( *func )( ), HINSTANCE dll, struct MIKAN_LITE *Mikan, HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow )
{
#ifdef _CRTDBG_MAP_ALLOC
#  ifdef _DEBUG
	_CrtSetReportMode( _CRT_WARN, _CRTDBG_MODE_DEBUG );
	_CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_DEBUG );
	_CrtSetReportMode( _CRT_ASSERT, _CRTDBG_MODE_DEBUG );
#  else
	//_CrtSetReportMode( _CRT_WARN, _CRTDBG_MODE_WNDW );
	//_CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_WNDW );
	//_CrtSetReportMode( _CRT_ASSERT, _CRTDBG_MODE_WNDW );
#  endif
	//_CrtSetDbgFlag( _CrtSetDbgFlag( 0 ) | _CRTDBG_LEAK_CHECK_DF );
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	class MikanLiteSystem *system = new MikanLiteSystem();
	if ( !system ) { return 1; }

	class MikanLiteWindow *window = new MikanLiteWindow( hInstance, dll );
	if ( !window ) { return 1; }

	class MikanLiteDraw *draw = new MikanLiteDraw();
	if ( !draw ) { return 1; }

	class MikanLiteInput *input = new MikanLiteInput();
	if ( !input ) { return 1; }

	_Mikan.system = Mikan->system = system;
	_Mikan.window = Mikan->window = window;
	_Mikan.draw = Mikan->draw = draw;
	_Mikan.input = Mikan->input = input;

	system->Init( hInstance, hPrevInstance, lpCmdLine, nCmdShow );
	window->SetWindowProc( WindowProc );

	func();

	window->CenteringWindow();
	window->Create( nCmdShow );
	draw->Init( window->GetWindowHandle() );

	system->Main( ( class DrawManagement * )draw, ( class InputManagement * )input );

	delete( system );
	delete( window );
	delete( draw );
	delete( input );

	return 0;
}
