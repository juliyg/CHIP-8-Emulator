#include "graphics.hpp"
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <iomanip>
using namespace std;

// Graphics class constructor 
// Intializes: gamePosition, SDL audio and video subsystems, SDL texture formatting, SDL audiostreams
Graphics::Graphics() {
    gamePosition.y = WINDOW_HEIGHT/10; 
    gamePosition.x = WINDOW_HEIGHT/20; 
    gamePosition.h = (WINDOW_HEIGHT*4)/5; 
    gamePosition.w = WINDOW_WIDTH/1.75; 
    debugHeight = WINDOW_HEIGHT/20; 
    debugWidth = WINDOW_WIDTH/7;
    debugXpos = gamePosition.x + gamePosition.w + WINDOW_WIDTH/20; 
    debugYpos = gamePosition.y;
    if(!TTF_Init()) {
        cerr << "Could not initialize TTF! SDL_ERROR: " << SDL_GetError() << endl;
    } 
    font = TTF_OpenFont("PressStart2P-regular.ttf", 21);
    if(!font) {
        cerr << "Could not open font! SDL_ERROR: " << SDL_GetError() << endl;
    }
    if(!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) 
        cerr << "Could not initialize SDL Video or Audio! SDL_ERROR: " << SDL_GetError() << endl;
    else if(!SDL_CreateWindowAndRenderer("CHIP-8 Emulator", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL, &window, &renderer))
        cerr << "Could not create window and renderer! SDL_ERROR: " << SDL_GetError() << endl;
    else {
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
        }
    }
}

// Graphics class destructer helper method
// Destroys: window, renderer, textures, audiostream, SDL overhead
void Graphics::cleanUp() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyTexture(screen);
    TTF_CloseFont(font);
    SDL_DestroyAudioStream(stream); 
    for(int i = 0; i < 16; i++) {
        regTextures[i].destroy();
        stackTextures[i].destroy();
    }
    pcTexture.destroy();
    TTF_Quit();
    SDL_Quit(); 
}

// Graphics class destructor
Graphics::~Graphics() {
    cleanUp();
}

// Return the pitch of the display game texture
int Graphics::getPitch() const {
    return pitch; 
}

// Acces the renderer
SDL_Renderer* Graphics::getRenderer() {
    return renderer; 
}

// Access the pixels inside the game display texture
uint8_t* Graphics::accessPixels() {
    if(!SDL_LockTexture(screen, NULL, &pixels, &pitch)) 
        cerr << "Could not lock texture! STD_ERROR: " << SDL_GetError() << endl;
    framebuffer = (uint8_t *) pixels; 
    return framebuffer;
}

// Make pixels inside game display texture available to read
void Graphics::updatePixels() {
    SDL_UnlockTexture(screen);
}

// Display updates to the computer screen
void Graphics::updateScreen(uint8_t registers[16], uint16_t stack[16], uint16_t pc) {
    SDL_RenderClear(renderer);
    updateHardware(registers, stack, pc);
    SDL_RenderTexture(renderer, screen, NULL, &gamePosition);
    SDL_RenderPresent(renderer);
}

// Generate the audio beep sound buffer
void Graphics::generateAudio(bool on) {
    if(on) {
        for(int i = 0; i < BUFF_LENGTH; i++) {
            audio_buf[i] = 0.5f * (sinf(2 * M_PI * time1) + sinf(2 * M_PI * time2)); 
            time1 += FREQUENCY1/(float)SAMPLING_RATE; 
            time2 += FREQUENCY2/(float)SAMPLING_RATE;

            if(time1 >= 1.0) {
                time1 -= 1.0;
            }
            if(time2 >= 1.0) {
                time2 -= 1.0;
            }
        } 
    } 
}

// Send audio beep buffer to the output stream
void Graphics::playSound(bool on) {
    if(on) {
        if(!SDL_PutAudioStreamData(stream, audio_buf, LENGTH)) {
            cerr << "Could not load data into audio stream! SDL_ERROR: " << SDL_GetError() << endl; 
        } 
    } else {
        if(!SDL_ClearAudioStream(stream)) {
            cerr << "Could not clear audio stream! SDL_ERROR: " << SDL_GetError() << endl;
        }
    }
}

// Input buffer for input keys 
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

// Debug methods  
void Graphics::updateHardware(uint8_t registers[16], uint16_t stack[16], uint16_t pc) {
    this->pc = pc;
    SDL_Surface *segment; 
    SDL_Color color{255,255,255,255};
    stringstream ss; 
    string text; 

    for(int i = 0; i < 16; i++) {
        registerLines[i] = registers[i];
        stackLines[i] = stack[i]; 

        ss.str("");
        ss.clear();
        ss << "V" << std::hex << std::uppercase << i 
            << ": 0x" << std::setw(2) << std::setfill('0') << static_cast<int>(registers[i]);
        text = ss.str();
        segment = TTF_RenderText_Blended(font, text.c_str(), 0, color);

        if(!segment) { 
            cerr << "Could not render text! SDL_ERROR: " << SDL_GetError() << endl; 
            return; 
        }

        regTextures[i].destroy();
        regTextures[i].outLine = SDL_CreateTextureFromSurface(renderer, segment);
        SDL_DestroySurface(segment); 
        regTextures[i].textPos.x = debugXpos; 
        regTextures[i].textPos.y = debugYpos + debugHeight * i; 
        regTextures[i].textPos.w = debugWidth;
        regTextures[i].textPos.h = debugHeight;
        SDL_RenderTexture(renderer, regTextures[i].outLine, NULL, &regTextures[i].textPos);

        ss.str("");
        ss.clear();
        ss << std::dec << i + 1 << ": 0x" << std::hex 
            << std::uppercase << std::hex << std::setw(3) << std::setfill('0') << stack[i];
        text = ss.str();
        segment = TTF_RenderText_Blended(font, text.c_str(), 0, color);

        if(!segment) { 
            cerr << "Could not render text! SDL_ERROR: " << SDL_GetError() << endl; 
            return; 
        }

        stackTextures[i].destroy();
        stackTextures[i].outLine = SDL_CreateTextureFromSurface(renderer, segment);
        SDL_DestroySurface(segment); 
        stackTextures[i].textPos.x = debugXpos + debugWidth + WINDOW_WIDTH/20; 
        stackTextures[i].textPos.y = debugYpos + debugHeight * i; 
        stackTextures[i].textPos.w = debugWidth;
        stackTextures[i].textPos.h = debugHeight;
        SDL_RenderTexture(renderer, stackTextures[i].outLine, NULL, &stackTextures[i].textPos);
    }
}


