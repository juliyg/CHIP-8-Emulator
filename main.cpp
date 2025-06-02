#define SDL_MAIN_HANDLED
#include "CHIP8.hpp"
#include <chrono> 
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
using namespace std;
using namespace chrono_literals;

int main(int argc, char *argv[]) {
    chip8 emulator; 
    emulator.loadROM(argv[1]); 
    SDL_Event event; 
    bool quit = false; 
    auto previousTime = chrono::high_resolution_clock::now(); 
    auto IPSCounter = chrono::high_resolution_clock::now();

    while(!quit) {
        while(SDL_PollEvent(&event) != 0) {
            if(event.type == SDL_EVENT_QUIT) {
                quit = true; 
                break;
            }
            emulator.inputBuffer(event);
        } 
        auto currentTime = chrono::high_resolution_clock::now(); 
        auto currentIPS = chrono::high_resolution_clock::now(); 
        chrono::duration<float, milli> deltaTime = currentTime - previousTime;
        chrono::duration<float, milli> deltaIPSTime = currentIPS - IPSCounter;

        if(deltaIPSTime > 2.25ms) {
            IPSCounter = currentIPS;
            emulator.emulateCycle();
            emulator.updateDisplay();
        }
        emulator.generateAudio();
        emulator.playSound();
        
        if(deltaTime > 16.67ms) {
            previousTime = currentTime;
            emulator.decrementCounters();
        }
    }
    return 0;
}