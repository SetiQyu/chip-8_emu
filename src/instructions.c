#include "instructions.h"
#include "main.h"
#include <stdio.h>
#include <stdlib.h>



void clear_screen_00E0(SDL_Renderer* renderer, unsigned char* gfx) {
    /*
    // Set the draw color to black (R:0, G:0, B:0, A:255)
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    // Clear the screen
    SDL_RenderClear(renderer);
    // Present the renderer
    SDL_RenderPresent(renderer);
    */
   for (size_t i = 0; i < 64*32; i++)
   {
        gfx[i] = 0;
   }
   

    draw_flag = TRUE;
}

void jump_1NNN(unsigned short opcode, int* pc){
    *pc = (int)(opcode & 0x0FFF);
}

void set_register_VX_6XNN(unsigned short opcode){
    reg[(opcode >> 4) & 0x0F] = opcode & 0x00FF;
}

void add_value_to_register_7XNN(unsigned short opcode){
    reg[(opcode >> 4) & 0x0F] += (opcode & 0x00FF);
}
void set_index_register_I_ANNN(unsigned short opcode){
    I = opcode & 0x0FFF;

}

void display_or_draw_DXYN(unsigned short opcode, SDL_Renderer* renderer, unsigned char* gfx, unsigned char* memory){
    /*Draw N pixel tall sprite from memory location that I
    is holding to screen, at horizontal X in VX and Y coordinate in VY*/
    unsigned char X = (opcode >> 4) & 0x0F;
    unsigned char Y= (opcode >> 8) & 0x0F;
    unsigned char height = (opcode >> 12) & 0x0F;

    unsigned char regX = reg[X] % 64;
    unsigned char regY = reg[Y] % 32;
    unsigned char pixel;
    reg[0xF] = '0';

   for(int yline = 0; yline < height; yline++)
   {
    pixel = memory[I + yline];
    for(int xline = 0; xline < 8; xline++)
     {
       if((pixel & (0x80 >> xline)) != 0)
       {
        if(gfx[(X + xline + ((Y + yline) * 64))] == 1)
           reg[0xF] = 1;                                 
         gfx[X + xline + ((Y + yline) * 64)] ^= 1;
       }
     }
   }
   draw_flag = TRUE;
    

    //SDL_RenderDrawPoint(renderer, 300, 200);

}