#ifdef _WIN32
#define WIN32_MEAN_AND_LEAN
#include <Windows.h>
#endif

#include "AConsole.h"
#include "Font.h"
#include "AScreen.h"

AConsole Console;

AConsole::AConsole() {
    m_vLines = {};
}

bool AConsole::Init() {
#ifdef DEBUG
#ifdef _WIN32
    AllocConsole();
#endif
    FILE* f = nullptr;
    f = freopen("CONIN$", "r", stdin);
    f = freopen("CONOUT$", "w", stdout);
    f = freopen("CONOUT$", "w", stderr);
    std::setvbuf(stdout, NULL, _IONBF, 0);
#endif

    return true;
}

void AConsole::WriteLine(std::string str) {
#ifdef DEBUG
    std::cout << str << std::endl;
    m_vLines.push_back(str);
#endif
}

void AConsole::Shutdown() {
#ifdef DEBUG
#ifdef _WIN32
    FreeConsole();
#endif
#endif
}

void AConsole::Flush() {
#ifdef DEBUG
    m_vLines.clear();
#endif
}
