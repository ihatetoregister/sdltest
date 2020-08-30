#include <stdio.h>
#include <SDL2/SDL.h>

// New file

struct system {
    SDL_Window *window;
    SDL_Renderer *renderer;
    int wHeight;
    int wWidth;
};

struct system gSystem;

struct player {
    int x;
    int y;
    int size;
} gPlayer = {0, 0, 50};

void check_error(int error, const char *message) {
    if(error) {
        printf("ERROR: ");
        printf(message);
        printf("\n");
    }
}

void setup() {
    printf("SDL init\n");
    check_error(SDL_Init(SDL_INIT_VIDEO) != 0, "Unable to initialize SDL");

    gSystem.window = SDL_CreateWindow("Test window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 480, SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL);
    check_error(gSystem.window == NULL, "Unable to create window");
    SDL_GetWindowSize(gSystem.window, &gSystem.wWidth, &gSystem.wHeight);

    printf("Window: w = %d, h = %d\n", gSystem.wWidth, gSystem.wHeight);

    gSystem.renderer = SDL_CreateRenderer(gSystem.window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    check_error(gSystem.renderer == NULL, "Unable to create a renderer");
}

void update() {
    gPlayer.x = rand() % gSystem.wWidth;
    gPlayer.y = rand() % gSystem.wWidth;
}

void redraw() {
    // Draw rectangle
    SDL_Rect rect;
    rect.x = gPlayer.x;
    rect.y = gPlayer.y;
    rect.w = gPlayer.size;
    rect.h = gPlayer.size;

    // Clear screen
    SDL_SetRenderDrawColor(gSystem.renderer, 10, 10, 100, 255);
    SDL_RenderClear(gSystem.renderer);
    
    // Render player
    SDL_SetRenderDrawColor(gSystem.renderer, 10, 100, 10, 255);
    SDL_RenderFillRect(gSystem.renderer,  &rect);
    SDL_RenderPresent(gSystem.renderer);
}

SDL_bool testCollision(int x, int y) {
    printf("Test collision\n");
    
    // Check player
    if ((x >= gPlayer.x) && (x <= (gPlayer.x + gPlayer.size))) {
        if ((y >= gPlayer.y) && (y <= (gPlayer.y + gPlayer.size))) {
            return SDL_TRUE;
        }
    }
    return SDL_FALSE;
}

int main(void) {

    setup();

    //SDL_Delay(5000);

    // Event loop
    SDL_Event event;
    SDL_bool done = SDL_FALSE;

    while(done == SDL_FALSE) {
        //printf("Polling event\n");

        if(SDL_PollEvent(&event)) {

            switch(event.type) {
                case SDL_QUIT:
                    done = SDL_TRUE;
                    printf("SDL_QUIT\n");
                    break;
                case SDL_KEYDOWN:
                    printf("Key pressed\n");
                    if(event.key.keysym.sym == SDLK_ESCAPE) {
                        done = SDL_TRUE;
                        printf("Escape received, quitting!\n");
                    }
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    printf("Mouse button pressed\n");
                    if (testCollision(event.button.x, event.button.y)) {
                        update();
                    }
                    break;
                //SDL Unsupported: Raspberry pi touch screen
                case 1792:
                    printf("Touch pressed: (x = %f, y = %f)\n", event.tfinger.x, event.tfinger.y);
                    if (testCollision(event.button.x, event.button.y)) {
                        update();
                    }
                    break;
                case 1793:
                    printf("Touch relaseed: (x = %f, y = %f)\n", event.tfinger.x, event.tfinger.y);
                    //gPlayer.x = gSystem.wWidth * event.tfinger.x;
                    //gPlayer.y = gSystem.wHeight * event.tfinger.y;
                    break;
                case 1794:
                    printf("Touch drag: : (x = %f, y = %f)\n", event.tfinger.x, event.tfinger.y);
                    //gPlayer.x = gSystem.wWidth * event.tfinger.x;
                    //gPlayer.y = gSystem.wHeight * event.tfinger.y;
                    break;
                default:
                    printf("Unknown event type (%d)\n", event.type);
            }
        }

        //update();
        redraw();

        SDL_Delay(100);
    }

    SDL_DestroyRenderer(gSystem.renderer);
    SDL_DestroyWindow(gSystem.window);
    SDL_Quit();
}
