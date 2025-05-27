#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <CHIP8.hpp> 
#include <graphics.hpp>

class Graphics {
    int WINDOW_HEIGHT{}; 
    int WINDOW_WIDTH{};
    void *pixels{}; 
    uint8_t *framebuffer{}; 
    int pitch{}; 
    SDL_Window *window{}; 
    SDL_Renderer *renderer{}; 
    SDL_Texture *screen{};
    SDL_Event event{};
public:
    Graphics();
    SDL_Renderer* getRenderer();
    int getPitch() const;
    void setupDisplay(); 
    uint8_t *accessPixels();
    void updatePixels();
    void cleanUp(); 
    ~Graphics(); 
};

