#include "graphics.hpp"
#include <SDL3/SDL.h>
#include <iostream>
using namespace std;

Graphics::Graphics() {
    WINDOW_HEIGHT = 1080;
    WINDOW_WIDTH = 1980; 
    if(!SDL_Init(SDL_INIT_VIDEO)) 
        cerr << "Could not initialize SDL! SDL_ERROR " << SDL_GetError() << endl;
    else if(!SDL_CreateWindowAndRenderer("CHIP-8 Emulator", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL, &window, &renderer)) 
        cerr << "Could not create window and renderer! SDL_ERROR: " << SDL_GetError() << endl;
    else {
        screen = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, 64, 32);
        if(screen == NULL)
            cerr << "Could not create texture! SDL_ERROR: " << SDL_GetError() << endl;
        SDL_SetTextureScaleMode(screen, SDL_SCALEMODE_NEAREST);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_SetTextureBlendMode(screen, SDL_BLENDMODE_NONE);
    }
}

int Graphics::getPitch() const {
    return pitch; 
}


SDL_Renderer* Graphics::getRenderer() {
    return renderer; 
}

uint8_t* Graphics::accessPixels() {
    if(!SDL_LockTexture(screen, NULL, &pixels, &pitch)) 
        cerr << "Could not lock texture! STD_ERROR: " << SDL_GetError() << endl;
    framebuffer = (uint8_t *) pixels; 
    return framebuffer;
}

void Graphics::updatePixels() {
    SDL_UnlockTexture(screen);
}

void Graphics::updateScreen() {
    SDL_RenderTexture(renderer, screen, NULL, NULL);
    SDL_RenderPresent(renderer);
}

void Graphics::inputBuffer(uint8_t *keyBuff, SDL_Event keyEvent) {
    bool validKey = false;
    int keyIndex = 0; 
    while(SDL_PollEvent(&keyEvent) != 0) {
        if(keyEvent.type == SDL_EVENT_KEY_DOWN) {
            cout << "Down Event Detected " << endl;
            switch(keyEvent.key.scancode) {
                case SDL_SCANCODE_1: validKey = true; keyIndex = 0; break;
                case SDL_SCANCODE_2: validKey = true; keyIndex = 1; break;
                case SDL_SCANCODE_3: validKey = true; keyIndex = 2; break;
                case SDL_SCANCODE_4: validKey = true; keyIndex = 3; break;
                case SDL_SCANCODE_Q: validKey = true; keyIndex = 4; break;
                case SDL_SCANCODE_W: validKey = true; keyIndex = 5; break;
                case SDL_SCANCODE_E: validKey = true; keyIndex = 6; break;
                case SDL_SCANCODE_R: validKey = true; keyIndex = 7; break;
                case SDL_SCANCODE_A: validKey = true; keyIndex = 8; break;
                case SDL_SCANCODE_S: validKey = true; keyIndex = 9; break;
                case SDL_SCANCODE_D: validKey = true; keyIndex = 10; break;
                case SDL_SCANCODE_F: validKey = true; keyIndex = 11; break;
                case SDL_SCANCODE_Z: validKey = true; keyIndex = 12; break;
                case SDL_SCANCODE_X: validKey = true; keyIndex = 13; break;
                case SDL_SCANCODE_C: validKey = true; keyIndex = 14; break;
                case SDL_SCANCODE_V: validKey = true; keyIndex = 15; break;
            }

            if(validKey) {
                fill(keyBuff, keyBuff + 16, 0);
                keyBuff[keyIndex] = 1; 
            }
            validKey = false;
        } else if(keyEvent.type == SDL_EVENT_KEY_UP) {
            cout << "Up Event Detected " << endl;
            switch(keyEvent.key.scancode) {
                case SDL_SCANCODE_1: keyBuff[0] = 0; break;
                case SDL_SCANCODE_2: keyBuff[1] = 0; break;
                case SDL_SCANCODE_3: keyBuff[2] = 0; break;
                case SDL_SCANCODE_4: keyBuff[3] = 0; break;
                case SDL_SCANCODE_Q: keyBuff[4] = 0; break;
                case SDL_SCANCODE_W: keyBuff[5] = 0; break;
                case SDL_SCANCODE_E: keyBuff[6] = 0; break;
                case SDL_SCANCODE_R: keyBuff[7] = 0; break;
                case SDL_SCANCODE_A: keyBuff[8] = 0; break;
                case SDL_SCANCODE_S: keyBuff[9] = 0; break;
                case SDL_SCANCODE_D: keyBuff[10] = 0; break;
                case SDL_SCANCODE_F: keyBuff[11] = 0; break;
                case SDL_SCANCODE_Z: keyBuff[12] = 0; break;
                case SDL_SCANCODE_X: keyBuff[13] = 0; break;
                case SDL_SCANCODE_C: keyBuff[14] = 0; break;
                case SDL_SCANCODE_V: keyBuff[15] = 0; break;
            }
        }
    }
}

void Graphics::cleanUp() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyTexture(screen);
    SDL_Quit(); 
}

Graphics::~Graphics() {
    cleanUp();
}