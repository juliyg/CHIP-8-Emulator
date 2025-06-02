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
    SDL_AudioStream *stream{};
    SDL_AudioSpec spec{}; 
    uint8_t *audio_buf{}; 
    uint32_t audio_len{}; 
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
    void playSound(uint8_t soundTimer); 
    ~Graphics(); 
};

//Initialize Audio Subsystem
//Define default output 
//Audio Streams
