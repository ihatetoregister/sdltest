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
    //SDL_RenderPresent(renderer);

    // Draw rectangle
    // Rectangle
    SDL_Rect rect;
    rect.x = 10;
    rect.y = 10;
    rect.w = 50;
    rect.h = 25;

    SDL_SetRenderDrawColor(renderer, 50, 100, 25, 255);
    SDL_RenderFillRect(renderer,  &rect);
    //SDL_RenderPresent(renderer);

    //SDL_Delay(10000);

    // Event loop
    SDL_Event event;
    SDL_bool done = SDL_FALSE;
    while(SDL_WaitEvent(&event) && (done == SDL_FALSE)) {
        printf("Polling event\n");
        switch(event.type) {
        case SDL_KEYDOWN:
            printf("Key pressed\n");
            if(event.key.keysym.sym == SDLK_ESCAPE) {
                done = SDL_TRUE;
                printf("Escape received, quitting!\n");
            }
            break;
        case SDL_QUIT:
            done = SDL_TRUE;
            printf("SDL_QUIT\n");
            break;
        //SDL Unsupported: Raspberry pi touch screen
        case 1792:
            printf("Touch pressed: (x = %f, y = %f)\n", event.tfinger.x, event.tfinger.y);
            break;
        case 1793:
            printf("Touch relaseed: (x = %f, y = %f)\n", event.tfinger.x, event.tfinger.y);
            break;
        case 1794:
            printf("Touch drag: : (x = %f, y = %f)\n", event.tfinger.x, event.tfinger.y);
            break;
        default:
            printf("Unknown event type (%d)\n", event.type);
        }
        //SDL_Delay(10);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit(); 
}
