#include <SDL3/SDL.h>

class Graphics {
    int WINDOW_HEIGHT{}; 
    int WINDOW_WIDTH{};
    void *pixels{}; 
    uint8_t *framebuffer{}; 
    int pitch{}; 
    SDL_Window *window{}; 
    SDL_Renderer *renderer{}; 
    SDL_Texture *screen{};
public:
    Graphics();
    SDL_Renderer* getRenderer();
    int getPitch() const;
    void setupDisplay(); 
    uint8_t *accessPixels();
    void inputBuffer(uint8_t *inputBuffer, SDL_Event keyEvent);
    void updateScreen();
    void updatePixels();
    void cleanUp(); 
    ~Graphics(); 
};

