#ifndef __MIKAN_LITE_SYSTEM
#define __MIKAN_LITE_SYSTEM

#include "Common.h"

#define _CRTDBG_MAP_ALLOC

#include <stdlib.h>
#include <crtdbg.h>
#ifdef _DEBUG
#  define new                new(_NORMAL_BLOCK, __FILE__, __LINE__)
#  define malloc(s)          _malloc_dbg(s, _NORMAL_BLOCK, __FILE__, __LINE__)
#  define calloc(c, s)       _calloc_dbg(c, s, _NORMAL_BLOCK, __FILE__, __LINE__)
#  define realloc(p, s)      _realloc_dbg(p, s, _NORMAL_BLOCK, __FILE__, __LINE__)
#  define _recalloc(p, c, s) _recalloc_dbg(p, c, s, _NORMAL_BLOCK, __FILE__, __LINE__)
#  define _expand(p, s)      _expand_dbg(p, s, _NORMAL_BLOCK, __FILE__, __LINE__)
#endif

class MikanLiteSystem :public MikanLite
{
private:
	HINSTANCE instance, prev;
	PSTR cmdline;
	int cmdshow;

	bool gameloop;
	bool permitscreensaver;
	bool active;
	bool notruninactive;
	class GameView *now, *next;

	void Update( void );
public:
	MikanLiteSystem( void );
	virtual int Init( HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow );
	virtual int Main( class DrawManagement * draw, class InputManagement *input );

	virtual HINSTANCE GetInstance( void );
	virtual HINSTANCE GetPrevInstance( void );
	virtual PSTR GetCommand( void );
	virtual int GetCommandShow( void );

	virtual bool IsPermitScreenSaver( void );
	virtual void SetPermitScreenSaver( bool enable = true );

	virtual bool IsActive( void );
	virtual void SetActive( bool active );

	virtual bool CannotRunInactiveWindow( void );
	virtual void SetInactiveWindow( bool notruninactive = true );

	virtual void WaitNextFrame( void );

	virtual void SetGameLoop( bool run = false );

	virtual void SetGameView( class GameView *gview );
};

#endif

