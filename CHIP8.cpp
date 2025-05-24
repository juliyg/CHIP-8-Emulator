#include <CHIP8.hpp>
#include <iostream> 
using namespace std;

//Execute machine lanague subroutine at address NNN (does nothing in emulator)
void chip8::op_0NNN() {}

//Clear the screen
void chip8::op_00E0() {
    
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

chip8::chip8() {
    random_device rd; 
    gen = mt19937(rd());
    dist = uniform_int_distribution<uint8_t> (0,255u); 
}

void chip8::initialize() {


}

// Implements the Fetch -> decode -> execute cycle 
void chip8::emulateCycle() {
    //Fetch 
    //Decode 
    //Execute 
}



