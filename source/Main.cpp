#ifdef _WIN32
#include <windows.h>
#ifdef _MSC_VER
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif
#endif

#include "ABaseEngine.h"

int main(int argc, char *argv[]) {
#if defined DEBUG && defined _WIN32
    FILE* i;
    AllocConsole();
    i = freopen("CONIN$", "r", stdin);
    i = freopen("CONOUT$", "w", stdout);
    i = freopen("CONOUT$", "w", stderr);
    std::setvbuf(stdout, NULL, _IONBF, 0);
#endif

    std::unique_ptr<ABaseEngine> e = std::make_unique<ABaseEngine>();
    e->Run();
    
    return false;
}
