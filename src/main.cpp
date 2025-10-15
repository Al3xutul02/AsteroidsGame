#include <iostream>
#include <SDL3/SDL.h>
#include "AsteroidsGameConfig.h"
#include "application.h"

int main(int argc, char** argv) {
    std::cout << "Version: " << AsteroidsGame_VERSION_MAJOR << '.'
                             << AsteroidsGame_VERSION_MINOR << '.'
                             << AsteroidsGame_VERSION_PATCH << '\n';

    Application app;
    bool success = app.Initialize(AsteroidsGame_Target_FPS);
    if (success) {
        app.RunLoop();
    }
    app.Shutdown();
    
    return 0;
}