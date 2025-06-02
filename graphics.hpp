#include <SDL3/SDL.h>

class Graphics {
    int WINDOW_HEIGHT{1080}; 
    int WINDOW_WIDTH{1980};
    static const int SAMPLING_RATE{44100};
    static const int FREQUENCY{441}; 
    static const int BUFF_LENGTH{2048}; 
    static const int LENGTH = BUFF_LENGTH * sizeof(float); 
    void *pixels{}; 
    uint8_t *framebuffer{}; 
    int pitch{}; 
    SDL_Window *window{}; 
    SDL_Renderer *renderer{}; 
    SDL_Texture *screen{};
    SDL_AudioStream *stream{};
    SDL_AudioSpec spec{SDL_AUDIO_F32LE, 1, SAMPLING_RATE}; 
    float audio_buf[BUFF_LENGTH]{}; 
    float time{}; 

public:
    Graphics();
    SDL_Renderer* getRenderer();
    int getPitch() const;
    void setupDisplay(); 
    uint8_t *accessPixels();
    void inputBuffer(uint8_t *inputBuffer, SDL_Event keyEvent);
    void updateScreen();
    void updatePixels();
    void generateAudio(bool on); 
    void cleanUp(); 
    void playSound(bool on); 
    ~Graphics(); 
};

//Initialize Audio Subsystem
//Define default output 
//Audio Streams
