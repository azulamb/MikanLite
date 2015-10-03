#ifndef __MIKAN_LITE_LIB
#define __MIKAN_LITE_LIB

#include "MikanLite.h"

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
	bool gameloop;
	bool permitscreensaver;
	class GameView *now, *next;

	void Update();
public:
	MikanLiteSystem();
	virtual int Init( HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow );
	virtual int Main();
	virtual void SetPermitScreenSaver( bool enable = true );
	virtual bool IsPermitScreenSaver();
	virtual void WaitNextFrame();
	virtual void SetGameLoop( bool run = false );

	virtual void SetGameView( class GameView *gview );
};

#endif

