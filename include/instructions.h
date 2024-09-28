#include <SDL2/SDL.h>

void clear_screen_00E0(SDL_Renderer*, unsigned char*);
void jump_1NNN(unsigned short, int*);
void set_register_VX_6XNN(unsigned short);
void add_value_to_register_7XNN(unsigned short);
void set_index_register_I_ANNN(unsigned short);
void display_or_draw_DXYN(unsigned short, SDL_Renderer*, unsigned char* gfx, unsigned char* memory);