#ifdef _WIN32
#include <windows.h>
#endif
#include "include/BlackjackGUI.h"
#include <iostream>


int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif


    try {
        BlackjackGUI gui;
        gui.run();
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}