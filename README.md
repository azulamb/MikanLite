# MikanLite

MikanLite can run Windows 7 or more.

MikanLite use Direct2D.

## Sample

MikanLite.dll puts in the same location as the exe.

```cpp
#include "MikanLite.h"

MikanLiteInit

class Game :public GameView
{
private:
	int timer;
public:
	void Init()
	{
		timer = 0;
	}
	bool Loop()
	{
		Mikan.draw->ClearScreen();

		Mikan.draw->DrawBox( Mikan.input->GetMouseX(), Mikan.input->GetMouseY(), 50, 50, Mikan.input->GetMouseFrame( 0 ) ? 0xFFFFFFFF : 0xFFFF00FF );

		Mikan.draw->Printf( 0, 0, 0, L"Test:%d", ++timer );
		return true;
	}
};

int Main()
{
	Mikan.system->SetGameView( new Game() );
	return 0;
}

```
