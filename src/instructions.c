#include <main.h>
#include <instructions.h>


static inline void clear_screen_00E0(SDL_Renderer* renderer){

    // Set the draw color to black (R:0, G:0, B:0, A:255)
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    // Clear the screen
    SDL_RenderClear(renderer);
    // Present the renderer
    SDL_RenderPresent(renderer);
    //Set color back
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Retro green color
}

static inline void jump_1NNN(unsigned short opcode){
    pc = opcode & 0x0FFF;
}

static inline void set_register_VX_6XNN(unsigned short opcode){
    reg[(opcode >> 4) & 0x0F] = opcode & 0x00FF;
}

static inline void add_value_to_register_7XNN(unsigned short opcode){
    reg[(opcode >> 4) & 0x0F] += (opcode & 0x00FF);
}
static inline void set_index_register_I_ANNN(unsigned short opcode){
    I = opcode & 0x0FFF;

}

static inline void display_or_draw_DXYN(unsigned short opcode, SDL_Renderer* renderer){
    /*Draw N pixel tall sprite from memory location that I
    is holding to screen, at horizontal X in VX and Y coordinate in VY*/
    unsigned char second_nibble = (opcode >> 4) & 0x0F;
    unsigned char third_nibble = (opcode >> 8) & 0x0F;
    unsigned char fourth_nibble = (opcode >> 12) & 0x0F;

    unsigned char X = reg[second_nibble];
    unsigned char Y = reg[third_nibble];

}