#ifdef _WIN32
#include <windows.h>
#ifdef _MSC_VER
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif
#endif

#include "ABaseEngine.h"

int main(int argc, char *argv[]) {
#if defined DEBUG && defined _WIN32
    AllocConsole();
    FILE* f = nullptr;
    f = freopen("CONIN$", "r", stdin);
    f = freopen("CONOUT$", "w", stdout);
    f = freopen("CONOUT$", "w", stderr);
    std::setvbuf(stdout, NULL, _IONBF, 0);
#endif

    BaseEngine.Run(argc, argv);
    return false;
}
