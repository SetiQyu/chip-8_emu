#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "instructions.h"







int loadROM(char* filename, unsigned char* memory){

    FILE *file = fopen(filename, "rb");
    if(!file){
        perror("Error opening ROM");
        return -1;
    }

    fseek(file, 0, SEEK_END);
    size_t fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    unsigned char* buffer = malloc(fileSize);
    if(!buffer){
        perror("Error allocating memory");
        fclose(file);
        return -1;
    }

    size_t bytesRead = fread(buffer, 1, fileSize, file);
    if (bytesRead != fileSize) {
        perror("Error reading file");
        free(buffer);
        fclose(file);
        return -1;
    }


    for (size_t i = 0; i < fileSize; ++i) {
        memory[ROM_START_ADDRESS + i] = buffer[i];
    }

    free(buffer);
    fclose(file);
    return 0;

}

void readAndLoadROMFile(unsigned char* memory){

    char* input = malloc(256 * sizeof(char)); 
    if(!input){
        perror("Error allocating memory");
    }

    printf("Please enter file path to ROM: ");

    if(fgets(input, 256, stdin) != NULL){
        input[strcspn(input, "\n")] = '\0';
    } else
        printf("Input error");

    //DEBUG PATH
    strcpy(input, "/home/leo/Desktop/chip8/roms/IBM Logo.ch8");
    loadROM(input, memory);
    printf("Debug: %s\n", input);
    free(input);
    
}

struct stack* stackInit(int size){
    struct stack* stackPtr = malloc(sizeof(struct stack));
    unsigned char* items = malloc(size * sizeof(unsigned char));

    stackPtr->itemArray = items;
    stackPtr->size = size;
    stackPtr->top = 0;

    return stackPtr;
}

void stackPush(struct stack* stackPtr, unsigned char item){

    if(stackPtr->top == stackPtr->size){
        perror("Stack overflow apparent");
    }
    stackPtr->itemArray[stackPtr->top] = item;
    stackPtr->top += 1;
}

void stackPop(struct stack* stackPtr, unsigned char item){
    
    if(stackPtr->top == 0){
        perror("Stack underflow apparent");
    }
    stackPtr->top -= 1;

}

static SDL_Renderer* initSDL(SDL_Window** window, SDL_Texture** texture){
 
    SDL_Renderer* renderer = NULL;

    
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return NULL;
    }

    
    SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, window, &renderer);
        

    *texture = SDL_CreateTexture(renderer,
                                 SDL_PIXELFORMAT_RGBA8888,
                                 SDL_TEXTUREACCESS_TARGET,
                                 VIRTUAL_WIDTH, VIRTUAL_HEIGHT);



    // Set the render target to the texture to draw the background
    SDL_SetRenderTarget(renderer, *texture);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black background
    SDL_RenderClear(renderer);

    
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Retro green color
    SDL_SetRenderTarget(renderer, NULL); // Reset to the window
    
    return renderer; 

}

void readAndExecOpCode(unsigned short opcode, SDL_Renderer* renderer){


    // Decode opcode
    switch(opcode & 0xF000)
    {    
        case 0x0000:
            switch(opcode & 0x000F)
            {
            case 0x0000: // 0x00E0: Clears the screen        
                // Execute opcode
                clear_screen_00E0(renderer);
                
            break;
        
            case 0x000E: // 0x00EE: Returns from subroutine          
                // Execute opcode
            break;
        
            default:
                printf ("Unknown opcode [0x0000]: 0x%X\n", opcode);   
                break;       
            }

        case 0x1000: //0x1NNN
            jump_1NNN(opcode);
            break;
        case 0x6000:
            set_register_VX_6XNN(opcode);
            break;
        case 0x7000:
            add_value_to_register_7XNN(opcode);
            break;
        case 0xA000:
            set_index_register_I_ANNN(opcode);
            break;
        case 0xD000:
            display_or_draw_DXYN(opcode, renderer);
            break;
        
        // more opcodes //
        default:
            printf ("Unknown opcode: 0x%X\n", opcode);
            break;
    }


}
void recordPlayerInput(){
    //TBD

    return;
}

void drawScreen(SDL_Renderer* renderer, unsigned char* gfx){
    
    // Clear the screen (set background to black)
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Set the color for the pixels (retro green)
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);  // Green pixels

    // Loop through each pixel in the gfx array and draw it
    for (int y = 0; y < SCREEN_HEIGHT; ++y) {
        for (int x = 0; x < SCREEN_WIDTH; ++x) {
            int index = x + (y * SCREEN_WIDTH);

            if (gfx[index]) {
                // Draw a point for each 'on' pixel in the gfx array
                // Optionally scale pixels by using PIXEL_SIZE
                for (int dx = 0; dx < PIXEL_SIZE; ++dx) {
                    for (int dy = 0; dy < PIXEL_SIZE; ++dy) {
                        SDL_RenderDrawPoint(renderer, x * PIXEL_SIZE + dx, y * PIXEL_SIZE + dy);
                    }
                }
            }
        }
    }

    // Present the renderer to the window (refresh the display)
    SDL_RenderPresent(renderer);
}

void gameLoop(SDL_Renderer* renderer, struct stack* stackPtr, unsigned char* memory, unsigned char* gfx){

    int pc = 0x200;
    int delay_timer = 0;
    int sound_timer = 0;
    int draw_flag = FALSE;
    unsigned short I;

    for(;;){

        unsigned short opcode = (memory[pc] << 8) | memory[pc + 1];
        pc += 2;
        readAndExecOpCode(opcode, renderer);

        if(delay_timer > 0)
            --delay_timer;
 
        if(sound_timer > 0){
            if(sound_timer == 1)
                printf("BEEP!\n");
                --sound_timer;
        } 


        if (draw_flag){
            drawScreen(renderer, gfx);
            draw_flag = FALSE;
        }

        recordPlayerInput();

        
    }
}

int main(){

    
    unsigned char* memory = malloc(MEMORY_SIZE * sizeof(unsigned char));
    struct stack* stackPtr = stackInit(STACK_SIZE);
    unsigned char* gfx = malloc(64 * 32 * sizeof(unsigned char));

    readAndLoadROMFile(memory);

    SDL_Window* window = NULL;
    SDL_Texture* texture = NULL;

    // Initialize SDL and get the renderer
    SDL_Renderer* renderer = initSDL(&window, &texture);
    gameLoop(renderer, stackPtr, memory, gfx);
    

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    free(stackPtr->itemArray);
    free(stackPtr);
    free(memory);
    free(gfx);
    return 0;
}