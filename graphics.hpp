#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>


struct textLine {
    SDL_Texture *outLine = NULL; 
    SDL_FRect textPos; 

    void destroy() {
        if(outLine) {
            SDL_DestroyTexture(outLine);
            outLine = NULL;
        }
    }
}; 

class Graphics {
//Audio handling members and constants
    static const int SAMPLING_RATE{44100};
    static constexpr float FREQUENCY1{440}; 
    static constexpr float FREQUENCY2{660};  
    static const int BUFF_LENGTH{2048}; 
    static const int LENGTH = BUFF_LENGTH * sizeof(float); 
    SDL_AudioSpec spec{SDL_AUDIO_F32LE, 1, SAMPLING_RATE}; 
    float audio_buf[BUFF_LENGTH]{}; 
    float time1{}; 
    float time2{};

//Game display section handling 
    int WINDOW_HEIGHT{1080}; 
    int WINDOW_WIDTH{1980};
    void *pixels{}; 
    uint8_t *framebuffer{}; 
    int pitch{}; 
    SDL_Window *window{}; 
    SDL_Renderer *renderer{}; 
    SDL_Texture *screen{};
    SDL_AudioStream *stream{};
    SDL_FRect gamePosition{}; 

//Debug handling members 
    float debugXpos{};
    float debugYpos{};
    float debugHeight{};
    float debugWidth{};
    TTF_Font *font{};
    uint8_t registerLines[16]{}; 
    uint16_t stackLines[16]{}; 
    uint16_t pc{};
    textLine regTextures[16]{}; 
    textLine stackTextures[16]{}; 
    textLine pcTexture{}; 

public:
//Constructor & destructor function definitions
    Graphics();
    ~Graphics(); 
    void cleanUp(); 

//Getter functions
    SDL_Renderer* getRenderer();
    uint8_t *accessPixels();
    int getPitch() const;
    
//Methods to play sound and draw
    void inputBuffer(uint8_t *inputBuffer, SDL_Event keyEvent);
    void updateScreen(uint8_t registers[16], uint16_t stack[16], uint16_t pc);
    void updatePixels();
    void generateAudio(bool on); 
    void playSound(bool on); 

//Debugging methods 
    void updateHardware(uint8_t registers[16], uint16_t stack[16], uint16_t pc); 
};  

