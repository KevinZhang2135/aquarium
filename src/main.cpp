#include <iostream>

#include "fish.h"
#include "screen.h"
#include "vector2.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800

using namespace std;

Screen* screen = nullptr;
void run() {
    screen = new Screen(SCREEN_WIDTH, SCREEN_HEIGHT);

    Uint64 current_time = SDL_GetPerformanceCounter();
    Uint64 last_time = 0;
    double delta_time = 0; // Elapsed milliseconds

    while (screen->Running()) {
        last_time = current_time;
        current_time = SDL_GetPerformanceCounter();

        delta_time = (double)(current_time - last_time) * 1000 / SDL_GetPerformanceFrequency();

        screen->HandleEvents();
        screen->Update(delta_time);
        screen->Render();
    }

    delete screen;
}

int main(int argc, char* args[]) {
    run();
    return 0;
}