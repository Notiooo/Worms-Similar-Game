#include "Game.h"

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>


int main()
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    // Moved this game to the heap according to the error:
    // warning C6262: Function uses '104688' bytes of stack: exceeds /analyze:stacksize '16384'. Consider moving some data to heap
    auto* game = new Game;
    game->run();

    delete game;
    return 0;
}
