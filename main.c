#include <stdio.h>
#include <SDL2/SDL.h>

void check_error(int error, const char *message) {
    if(error) {
        printf("ERROR: ");
        printf(message);
        printf("\n");
    }
}


int main(void) {
    printf("Hello SDL!\n");

    check_error(SDL_Init(SDL_INIT_VIDEO) != 0, "Unable to initialize SDL");

    SDL_Window *window = SDL_CreateWindow("Test window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
    check_error(window == NULL, "Unable to create window");

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    check_error(renderer == NULL, "Unable to create a renderer");

    SDL_SetRenderDrawColor(renderer, 100, 149, 237, 255);

    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    SDL_Delay(10000);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit(); 
}
