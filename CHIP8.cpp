#include <CHIP8.hpp>
#include <graphics.hpp>
#include <graphics.cpp>
using namespace std;

//Execute machine lanague subroutine at address NNN (does nothing in emulator)
void chip8::op_0NNN() {}

//Clear the screen
void chip8::op_00E0() {
    uint8_t *pixels = display.accessPixels(); 
    int pitch = display.getPitch(); 

    for(int i = 0; i < 64; i++) {
        for(int j = 0; j < 32; j++) {
            pixels[4*i + j * pitch] = 0;
            pixels[4*i+1 + j * pitch] = 0;
            pixels[4*i+2 + j * pitch] = 0;
            pixels[4*i+3 + j * pitch] = 255;
        }
    }
    display.updatePixels(); 
}

//Return from a subroutine
void chip8::op_00EE() {
    pc = stack[--sp]; 
}

//Jump to address NNN
void chip8::op_1NNN() {
    pc = opcode & 0x0FFFu; 
}

//Execute subroutine starting at NNN
void chip8::op_2NNN() {
    stack[sp] = pc; 
    ++sp; 
    pc = opcode & 0x0FFFu;
}

//Skip the following instruction if VX == NN 
void chip8::op_3XNN() {
    uint8_t X = (opcode & 0x0F00u) >> 8u; 
    uint8_t NN = (opcode & 0x00FFu); 
    uint8_t VX = registers[X]; 

    if(VX == NN) 
        pc += 2; 
}

//Skip the following instruction if VX != NN
void chip8::op_4XNN() {
    uint8_t X = (opcode & 0x0F00u) >> 8u;
    uint8_t NN = (opcode & 0x00FFu); 
    uint8_t VX = registers[X];

    if(VX != NN) 
        pc += 2; 
}

//Skip the following instruction if VX == VY 
void chip8::op_5XY0() {
    uint8_t VX = registers[(opcode & 0x0F00u) >> 8u];
    uint8_t VY = registers[(opcode & 0x00F0u) >> 4u];

    if(VX == VY)
        pc += 2; 
}

//Store number rNN in register VX 
void chip8::op_6XNN() {
    uint8_t NN = opcode & 0x00FFu; 
    uint8_t X = (opcode & 0x0F00u) >> 8u;

    registers[X] = NN; 
}

//Add the value of NN to register VX 
void chip8::op_7XNN() {
    uint8_t NN = opcode & 0x00FFu;
    uint8_t X = (opcode & 0x0F00u) >> 8u;

    registers[X] += NN; 
}

//Store the value of register VY in VX 
void chip8::op_8XY0() {
    uint8_t VY = registers[(opcode & 0x00F0u) >> 4u];
    uint8_t X = (opcode & 0x0F00u) >> 8u;

    registers[X] = VY; 
}

//Set VX to (VX OR VY)
void chip8::op_8XY1() {
    uint8_t VY = registers[(opcode & 0x00F0u) >> 4u]; 
    uint8_t VX = registers[(opcode & 0x0F00u) >> 8u];
    uint8_t X = (opcode & 0x0F00u) >> 8u; 

    registers[X] = (VY | VX); 
}

//Set VX to (VX AND VY) 
void chip8::op_8XY2() {
    uint8_t VY = registers[(opcode & 0x00F0u) >> 4u]; 
    uint8_t VX = registers[(opcode & 0x0F00u) >> 8u];
    uint8_t X = (opcode & 0x0F00u) >> 8u; 

    registers[X] = (VY & VX);
}

//Set VX to (VX XOR VY)
void chip8::op_8XY3() {
    uint8_t VY = registers[(opcode & 0x00F0u) >> 4u]; 
    uint8_t VX = registers[(opcode & 0x0F00u) >> 8u];
    uint8_t X = (opcode & 0x0F00u) >> 8u; 

    registers[X] = (VY ^ VX); 
}

//ADD register VY to register VX 
//Set VF to 01 if there is carryout 
//Set VF to 00 if therer is no carryout
void chip8::op_8XY4() {
    uint8_t Y = (opcode & 0x00F0u) >> 4u;
    uint8_t X = (opcode & 0x0F00u) >> 8u;
    uint16_t sum = registers[Y] + registers[X]; 

    registers[0xF] = (sum > 255) ? 1 : 0; 
    registers[X] = sum & 0x00FFu; 
}

//Subtract VY from VX 
//Set VF to 00 if borrow occurs
//Set VF to 01 if borrow does not occur 
void chip8::op_8XY5() {
    uint8_t Y = (opcode & 0x00F0u) >> 4u;
    uint8_t X = (opcode & 0x0F00u) >> 8u; 

    registers[0xF] = (registers[X] < registers[Y]) ? 0 : 1; 
    registers[X] = registers[X] - registers[Y]; 
}

//Store the value of VY right shifted in VX
//Set VF to the LSB prior to the bitshift 
//VY is unchanged
void chip8::op_8XY6() {
    uint8_t Y = (opcode & 0x00F0u) >> 4u; 
    uint8_t X = (opcode & 0x0F00u) >> 8u; 
    uint8_t rsy = registers[Y] >> 1; 
    unsigned int lsb = registers[Y] & 0x01; 
    
    registers[0xF] = lsb;
    registers[X] = rsy; 
}

//Set VX to the value of VY minus VX 
//Set VF to 00 if there is a borrow
//Set VF to 01 if there is no borrow
void chip8::op_8XY7() {
    uint8_t Y = (opcode & 0x00F0u) >> 4u;
    uint8_t X = (opcode & 0x0F00u) >> 8u; 

    registers[0xF] = (registers[Y] < registers[X]) ? 0 : 1; 
    registers[X] = registers[Y] - registers[X]; 
}

//Store the value of VY shifted left in VX 
//Set VF to the MSB prior to shift 
//VY is unchanged
void chip8::op_8XYE() {
    uint8_t Y = (opcode & 0x00F0u) >> 4u;
    uint8_t X = (opcode & 0x0F00u) >> 8u; 
    uint8_t lsy = registers[Y] << 1; 
    unsigned int msb = registers[Y] & 0x80; 

    registers[0xF] = msb; 
    registers[X] = lsy; 
}

//Skip the next instruction if VX != VY
void chip8::op_9XY0() {
    uint8_t Y = (opcode & 0x00F0u) >> 4u;
    uint8_t X = (opcode & 0x0F00u) >> 8u; 

    if(registers[X] != registers[Y]) 
        pc += 2; 
}

//Store memory address in index register 
void chip8::op_ANNN() {
    indreg = opcode & 0x0FFFu; 
}

//Jump to address NNN + V0 (set PC = NNN + V0)
void chip8::op_BNNN() {
    pc = registers[0x0] + (opcode & 0x0FFFu);
}

//Set VX to a random number with a mask of NN 
void chip8::op_CXNN() {
    uint8_t randByte = dist(gen);
    uint8_t X = (opcode & 0x0F00u) >> 8u; 
    registers[X] = randByte & (opcode & 0x00FFu); 
}

//Draw a sprite at position VX, VY with N bytes of sprite data starting at the address stored in I
//Set VF to 01 if any set pixels are changed to unset, and 00 otherwise
void chip8::op_DXYN() {

}

//Skip the following instruction if the key corresponding to the hex value currently stored in register VX is pressed
//assume that VX is between 0x0 and 0xF, otherwise out of bounds error
void chip8::op_EX9E() {
    uint8_t X = (opcode & 0x0F00u) >> 8u; 
    if(keypad[registers[X]]) 
        pc += 2; 
}

//Skip the following instruction if the key corresponding to the hex value currently stored in register VX is not pressed
void chip8::op_EXA1() {
    uint8_t X = (opcode & 0x0F00u) >> 8u;
    if(!keypad[registers[X]]) 
        pc += 2; 
}

//Store the current value of the delay timer in register VX
void chip8::op_FX07() {

}

//Wait for a keypress and store the result in register VX
void chip8::op_FX0A() {

}

//Set the delay timer to the value of register VX
void chip8::op_FX15() {

}

//Set the sound timer to the value of register VX
void chip8::op_FX18() {

}

//Add the value stored in register VX to register I
void chip8::op_FX1E() {

}

//Set I to the memory address of the sprite data corresponding to the hexadecimal digit stored in register VX
void chip8::op_FX29() {

}

//Store the binary-coded decimal equivalent of the value stored in register VX at addresses I, I + 1, and I + 2
void chip8::op_FX33() {

}

//Store the values of registers V0 to VX inclusive in memory starting at address I
//I is set to I + X + 1 after operation
void chip8::op_FX55() {

}

//Fill registers V0 to VX inclusive with the values stored in memory starting at address I
//I is set to I + X + 1 after operation
void chip8::op_FX65() {

}

chip8::chip8() : display() {
    initialize(); 
}

void chip8::initialize() {
    random_device rd; 
    gen = mt19937(rd());
    dist = uniform_int_distribution<uint8_t> (0,255u); 
    pc = 0x0200;
    uint8_t array[80] = {
        0xF0, 0x90, 0x90, 0x90, 0xF0, 
        0x20, 0x60, 0x20, 0x20, 0x70,
        0xF0, 0x10, 0xF0, 0x80, 0xF0, 
        0xF0, 0x10, 0xF0, 0x10, 0xF0, 
        0x90, 0x90, 0xF0, 0x10, 0x10,
        0xF0, 0x80, 0xF0, 0x10, 0xF0,
        0xF0, 0x80, 0xF0, 0x90, 0xF0, 
        0xF0, 0x10, 0x20, 0x40, 0x40, 
        0xF0, 0x90, 0xF0, 0x90, 0xF0, 
        0xF0, 0x90, 0xF0, 0x10, 0xF0, 
        0xF0, 0x90, 0xF0, 0x90, 0x90, 
        0xE0, 0x90, 0xE0, 0x90, 0xE0, 
        0xF0, 0x80, 0x80, 0x80, 0xF0, 
        0xE0, 0x90, 0x90, 0x90, 0xE0, 
        0xF0, 0x80, 0xF0, 0x80, 0xF0, 
        0xF0, 0x80, 0xF0, 0x80, 0x80  
    };

    for(uint16_t i = 0x0050; i <= 0x009F; i++) 
        memory[i] = array[i - 0x0050];     
}


void chip8::decodeExe(uint16_t opcode) {
    switch(opcode & 0xF000) {
        case 0x0: 
            if(opcode==0x00E0) op_00E0(); 
            else if(opcode==0x00EE) op_00EE();
            break;
        case 0x1000: op_1NNN(); break;
        case 0x2000: op_2NNN(); break;
        case 0x3000: op_3XNN(); break;
        case 0x4000: op_4XNN(); break;
        case 0x5000: op_5XY0(); break;
        case 0x6000: op_6XNN(); break;
        case 0x7000: op_7XNN(); break;
        case 0x8000:
            switch(opcode & 0x000F) {
                case 0x0: op_8XY0(); break;
                case 0x1: op_8XY1(); break;
                case 0x2: op_8XY2(); break;
                case 0x3: op_8XY3(); break;
                case 0x4: op_8XY4(); break;
                case 0x5: op_8XY5(); break;
                case 0x6: op_8XY6(); break;
                case 0x7: op_8XY7(); break;
                case 0xE: op_8XYE(); break;
            }
            break;
        case 0x9000: op_9XY0(); break;
        case 0xA000: op_ANNN(); break;
        case 0xB000: op_BNNN(); break;
        case 0xC000: op_CXNN(); break;
        case 0xD000: op_DXYN(); break;
        case 0xE000: 
            if((opcode & 0x00FF) == 0x9E) op_EX9E();
            else if((opcode & 0x00FF) == 0xA1) op_EXA1();
            break;
        case 0xF000:
            switch(opcode & 0x00FF) {
                case 0x07: op_FX07(); break;
                case 0x0A: op_FX0A(); break;
                case 0x15: op_FX15(); break;
                case 0x18: op_FX18(); break;
                case 0x1E: op_FX1E(); break;
                case 0x29: op_FX29(); break;
                case 0x33: op_FX33(); break;
                case 0x55: op_FX55(); break;
                case 0x65: op_FX65(); break;
            }
    }
}


// Implements the Fetch -> decode -> execute cycle 
void chip8::emulateCycle() {
    opcode = (memory[pc] << 8u) + memory[pc+1]; 
    decodeExe(opcode);
    pc += 2; 
}



