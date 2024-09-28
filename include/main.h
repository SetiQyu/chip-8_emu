#include <SDL2/SDL.h>

extern unsigned char chip8_fontset[80]; 

struct stack{
    unsigned char* itemArray;
    int top;
    int size;
};

#define MEMORY_SIZE 4096
#define ROM_START_ADDRESS 0x200
#define STACK_SIZE 16
#define FIRST_DIGIT_MASK 0xF000
#define TRUE 1
#define FALSE 0
#define PIXEL_SIZE 10

extern const int SCREEN_WIDTH; // Window size width
extern const int SCREEN_HEIGHT;  // Window size height
extern const int VIRTUAL_WIDTH;   // Emulator screen width
extern const int VIRTUAL_HEIGHT;   // Emulator screen height

extern int delay_timer;
extern int sound_timer;
extern unsigned char* memory;
extern unsigned char* gfx;
extern int draw_flag;

extern unsigned short I;
extern int* pc;

extern unsigned char reg[16];
extern unsigned short opcode;

