#include "graphics.hpp"
#include <SDL3/SDL.h>
#include <iostream>
using namespace std;

Graphics::Graphics() {
    WINDOW_HEIGHT = 1080;
    WINDOW_WIDTH = 1980; 
    if(!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) 
        cerr << "Could not initialize SDL Video or Audio! SDL_ERROR: " << SDL_GetError() << endl;
    else if(!SDL_CreateWindowAndRenderer("CHIP-8 Emulator", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL, &window, &renderer))
        cerr << "Could not create window and renderer! SDL_ERROR: " << SDL_GetError() << endl;
    else {
        if(!SDL_LoadWAV("beep.wav", &spec, &audio_buf, &audio_len)) 
            cerr << "Could not load audio file beep.wav SDL_ERROR: " << SDL_GetError() << endl;
        screen = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, 64, 32);
        stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, NULL, NULL); 
        if(screen == NULL)
            cerr << "Could not create texture! SDL_ERROR: " << SDL_GetError() << endl;
        if(!stream) 
            cerr << "Could not open audio stream! SDL_ERROR: " << SDL_GetError() << endl;
        SDL_SetTextureScaleMode(screen, SDL_SCALEMODE_NEAREST);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_SetTextureBlendMode(screen, SDL_BLENDMODE_NONE);
        if(!SDL_ResumeAudioStreamDevice(stream)) {
            cerr << "Audio stream failed! SDL_ERROR: " << SDL_GetError() << endl; 
            return; 
        }
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

void Graphics::playSound(uint8_t soundTimer) {
    if(soundTimer > 0) {
        for(uint8_t i = 0; i < soundTimer; i++) {
            if(!SDL_PutAudioStreamData(stream, audio_buf, audio_len)) {
                cerr << "Audio enqueue failed! SDL_ERROR: " << SDL_GetError() << endl; 
                return; 
            }
        }
    } else {
        SDL_ClearAudioStream(stream);
    }
    
}

void Graphics::inputBuffer(uint8_t *keyBuff, SDL_Event keyEvent) {
    bool validKey = false;
    int keyIndex = 0; 
        if(keyEvent.type == SDL_EVENT_KEY_DOWN) {
            switch(keyEvent.key.scancode) {
                case SDL_SCANCODE_1: validKey = true; keyIndex = 1; break;
                case SDL_SCANCODE_2: validKey = true; keyIndex = 2; break;
                case SDL_SCANCODE_3: validKey = true; keyIndex = 3; break;
                case SDL_SCANCODE_4: validKey = true; keyIndex = 0xC; break;
                case SDL_SCANCODE_Q: validKey = true; keyIndex = 4; break;
                case SDL_SCANCODE_W: validKey = true; keyIndex = 5; break;
                case SDL_SCANCODE_E: validKey = true; keyIndex = 6; break;
                case SDL_SCANCODE_R: validKey = true; keyIndex = 0xD; break;
                case SDL_SCANCODE_A: validKey = true; keyIndex = 7; break;
                case SDL_SCANCODE_S: validKey = true; keyIndex = 8; break;
                case SDL_SCANCODE_D: validKey = true; keyIndex = 9; break;
                case SDL_SCANCODE_F: validKey = true; keyIndex = 0xE; break;
                case SDL_SCANCODE_Z: validKey = true; keyIndex = 0xA; break;
                case SDL_SCANCODE_X: validKey = true; keyIndex = 0x0; break;
                case SDL_SCANCODE_C: validKey = true; keyIndex = 0xB; break;
                case SDL_SCANCODE_V: validKey = true; keyIndex = 0xF; break;
            }

            if(validKey) {
                fill(keyBuff, keyBuff + 16, 0);
                keyBuff[keyIndex] = 1; 
            }
            validKey = false;
        } else if(keyEvent.type == SDL_EVENT_KEY_UP) {
            switch(keyEvent.key.scancode) {
                case SDL_SCANCODE_1: keyBuff[1] = 0; break;
                case SDL_SCANCODE_2: keyBuff[2] = 0; break;
                case SDL_SCANCODE_3: keyBuff[3] = 0; break;
                case SDL_SCANCODE_4: keyBuff[0xC] = 0; break;
                case SDL_SCANCODE_Q: keyBuff[4] = 0; break;
                case SDL_SCANCODE_W: keyBuff[5] = 0; break;
                case SDL_SCANCODE_E: keyBuff[6] = 0; break;
                case SDL_SCANCODE_R: keyBuff[0xD] = 0; break;
                case SDL_SCANCODE_A: keyBuff[7] = 0; break;
                case SDL_SCANCODE_S: keyBuff[8] = 0; break;
                case SDL_SCANCODE_D: keyBuff[9] = 0; break;
                case SDL_SCANCODE_F: keyBuff[0xE] = 0; break;
                case SDL_SCANCODE_Z: keyBuff[0xA] = 0; break;
                case SDL_SCANCODE_X: keyBuff[0x0] = 0; break;
                case SDL_SCANCODE_C: keyBuff[0xB] = 0; break;
                case SDL_SCANCODE_V: keyBuff[0xF] = 0; break;
            }
        }
}

void Graphics::cleanUp() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyTexture(screen);
    SDL_DestroyAudioStream(stream); 
    SDL_free(audio_buf);
    SDL_Quit(); 
}

Graphics::~Graphics() {
    cleanUp();
}