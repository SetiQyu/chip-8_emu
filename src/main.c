#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <main.h>






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

int main(){

    
    unsigned char* memory = malloc(MEMORY_SIZE * sizeof(unsigned char));
    struct stack* stackPtr = stackInit(STACK_SIZE);
    readAndLoadROMFile(memory);

    SDL_Window* window = NULL;
    SDL_Texture* texture = NULL;

    // Initialize SDL and get the renderer
    SDL_Renderer* renderer = initSDL(&window, &texture);
    

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    free(stackPtr->itemArray);
    free(stackPtr);
    free(memory);
    return 0;
}