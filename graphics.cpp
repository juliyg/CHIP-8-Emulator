#include <graphics.hpp>
#include <iostream> 
using namespace std;

Graphics::Graphics() {
    WINDOW_HEIGHT = 1080;
    WINDOW_WIDTH = 1980; 
    if(!SDL_Init(SDL_INIT_VIDEO)) 
        cerr << "Could not initialize SDL! SDL_ERROR " << SDL_GetError() << endl;
    else if(!SDL_CreateWindowAndRenderer("CHIP-8 Emulator", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_FULLSCREEN, &window, &renderer)) 
        cerr << "Could not create window and renderer! SDL_ERROR: " << SDL_GetError() << endl;
    else {
        screen = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, 64, 32);
        if(screen == NULL)
            cerr << "Could not create texture! SDL_ERROR: " << SDL_GetError() << endl;
        SDL_SetTextureScaleMode(screen, SDL_SCALEMODE_NEAREST);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
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
    SDL_RenderTexture(renderer, screen, NULL, NULL);
    SDL_RenderPresent(renderer);
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

