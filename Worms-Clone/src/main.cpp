#include "pch.h"
#include "Game.h"

#if(defined(_MSC_VER) && defined(_DEBUG))
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <stdlib.h>
#endif

int main()
{
    #if(defined(_MSC_VER) && defined(_DEBUG))
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    #endif

    // Moved this game to the heap according to the error:
    // warning C6262: Function uses '104688' bytes of stack: exceeds /analyze:stacksize '16384'. Consider moving some data to heap
    auto game = std::make_unique<Game>();
    game->run();

    return 0;
}