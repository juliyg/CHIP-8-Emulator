#include <cstdint> 
#include <random> 
#include <stdlib.h>
#include <graphics.hpp>
using namespace std;

/* CHIP-8 class definitions 
 * Variables: Stack, byte-addressable memory, general purpose registers, stack pointer, index register
 * sound timer, delay timer, display, opcodes 
 * 
 * Private methods: instruction functions
 * 
 * Public methods: emulateCylce and initialize 
 */

class chip8 {
    //Emulation components 
    uint8_t registers[16]{};                
    uint8_t memory[4096]{};                
    uint16_t stack[16];                     
    uint8_t sp{};                            
    uint16_t indreg{};                       
    uint16_t pc{};                          
    uint16_t opcode{};                      
    uint8_t delay{};                        
    uint8_t sound{};                         
    uint8_t keypad[16]; 
    int chip8Screen[64 * 32];
    mt19937 gen; 
    uniform_int_distribution<uint8_t> dist;
    Graphics display{};

    //Opcode method declarations, to be defined in CHIP.cpp 

    //0x0--- opcodes
    void op_0NNN();
    void op_00E0(); 
    void op_00EE(); 

    //0x1NNN through 0x7XNN
    void op_1NNN();
    void op_2NNN();
    void op_3XNN();
    void op_4XNN();
    void op_5XY0(); 
    void op_6XNN();
    void op_7XNN(); 

    //0x8XY0 through 0x9XY0
    void op_8XY0();
    void op_8XY1(); 
    void op_8XY2();
    void op_8XY3();
    void op_8XY4(); 
    void op_8XY5(); 
    void op_8XY6();
    void op_8XY7(); 
    void op_8XYE(); 
    void op_9XY0(); 

    //0xANNN through 0xCXNN
    void op_ANNN(); 
    void op_BNNN(); 
    void op_CXNN(); 

    //0xDXYN
    void op_DXYN(); 

    //0xEX9E through 0xEXF2
    void op_EX9E(); 
    void op_EXA1();

    //0xFX07 through 0xFXF8
    void op_FX07();
    void op_FX0A(); 
    void op_FX15(); 
    void op_FX18(); 
    void op_FX1E(); 
    void op_FX29();
    void op_FX33();
    void op_FX55(); 
    void op_FX65(); 
    void decodeExe(uint16_t opcode); 
           
//public methods 
public: 
    chip8(); 
    void initialize();
    void emulateCycle();
};
