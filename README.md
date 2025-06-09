# About 
This is a CHIP-8 emulator written in C++, built from scratch to better understand computer architecture and for fun!

It supports all 35 official CHIP-8 instructions and includes a basic visual debugger that displays stack and register states. The emulator is capable of running most standard test ROMs and public domain CHIP-8 games.

### Features:
- 64×32 monochrome display
- Hex-based keypad input (via SDL3)
- ROM loading via command line
- Stack and register visualization
- Sound output (via SDL3)

---

# How to Build
The emulator uses **SDL3** and **SDL3_ttf** for audio output, keyboard input, and video display. You’ll need to install these libraries before building.

### 📦 Dependencies:
- [SDL3](https://github.com/libsdl-org/SDL)  
- [SDL3_ttf](https://github.com/libsdl-org/SDL_ttf)
- Note: On Windows, you'll need to place `SDL3.dll` and `SDL3_ttf.dll` in the same directory as the `chip8.exe` binary to run the emulator.

### Building and Running 
To build the emulator, you'll need a C++ compiler, along with the SDL3 and SDL3_ttf development libraries.

- Compile all `.cpp` files in `src/`
- Link against `SDL3` and `SDL3_ttf`
- Place `SDL3.dll` and `SDL3_ttf.dll` alongside your executable on Windows

# Test ROMs 
I used [Timendus' chip8-test-suite](https://github.com/Timendus/chip8-test-suite) to verify opcodes, especially for flag-related instructions. It was incredibly helpful during debugging, and I’m grateful for such a thorough resource. Big thanks to Timendus!

You can also try public domain ROMs from:

- [dmatlack/chip8 ROMs](https://github.com/dmatlack/chip8/tree/master/roms)

### Recommended ROMs:
- `IBM Logo` — simple display verification
- `BC_test.ch8` — instruction and logic testing
- `Keypad Test` — verify input mapping
- `Corax+` — thorough opcode check



