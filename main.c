#include <stdio.h>
#include <SDL2/SDL.h>


struct System {
    SDL_Window *window;
    SDL_Renderer *renderer;
    int wHeight;
    int wWidth;
};

struct System gSystem;

struct Player {
    int x;
    int y;
    int size;
    SDL_Color c;
    SDL_bool dirty;
} gPlayer = {0, 0, 50, SDL_FALSE};

int gPlayerCountMax = 1000;
struct Player gPlayers[1000];
int gPlayerCount = 1;


SDL_Color randomColor() {
    SDL_Color c = {.r = rand() % 255, .g = rand() % 255, .b = rand() % 255, .a = 255};
    return c;
}

struct Player createRandomPlayer() {
    struct Player p = {
        .x = rand() % (gSystem.wWidth - gPlayer.size),
        .y = rand() % (gSystem.wHeight - gPlayer.size),
        .size = 50,
        .c = randomColor(),
        .dirty = SDL_FALSE};
    return p;
}

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

    gSystem.window = SDL_CreateWindow("Test window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 480, SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_OPENGL);
    check_error(gSystem.window == NULL, "Unable to create window");
    SDL_GetWindowSize(gSystem.window, &gSystem.wWidth, &gSystem.wHeight);

    printf("Window: w = %d, h = %d\n", gSystem.wWidth, gSystem.wHeight);

    gSystem.renderer = SDL_CreateRenderer(gSystem.window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    check_error(gSystem.renderer == NULL, "Unable to create a renderer");
}

void update() {
    for (int i=0; i < gPlayerCount; i++) {
        if (gPlayers[i].dirty) {
            gPlayers[i] = createRandomPlayer();
        }
    }
    
    if (gPlayerCount < gPlayerCountMax) {
        gPlayers[gPlayerCount] = createRandomPlayer();
        gPlayerCount += 1;
    }
}

void redraw() {
    
    // Clear screen
    SDL_SetRenderDrawColor(gSystem.renderer, 10, 10, 100, 255);
    SDL_RenderClear(gSystem.renderer);
    
    // Render players
    for (int i=0; i<gPlayerCount; i++) {
        SDL_Rect rect;
        rect.x = gPlayers[i].x;
        rect.y = gPlayers[i].y;
        rect.w = gPlayers[i].size;
        rect.h = gPlayers[i].size;
        
        SDL_SetRenderDrawColor(gSystem.renderer, gPlayers[i].c.r, gPlayers[i].c.g, gPlayers[i].c.b, gPlayers[i].c.a);
        SDL_RenderFillRect(gSystem.renderer,  &rect);
    }
    
    SDL_RenderPresent(gSystem.renderer);
}

SDL_bool testCollision(struct Player *p, int x, int y) {
    printf("Test collision\n");
    
    // Check player
    if ((x >= p->x) && (x <= (p->x + p->size))) {
        if ((y >= p->y) && (y <= (p->y + p->size))) {
            p->dirty = SDL_TRUE;
            return SDL_TRUE;
        }
    }
    return SDL_FALSE;
}

SDL_bool testCollisions(int x, int y) {
    for (int i=0; i<gPlayerCount; i++) {
        if (testCollision(&gPlayers[i], x, y))
            return SDL_TRUE;
    }
    return SDL_FALSE;
}

int main(void) {

    setup();
    
    gPlayers[0] = gPlayer;
    
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
                if (testCollisions(event.button.x, event.button.y)) {
                    update();
                }
                break;
            //SDL Unsupported: Raspberry pi touch screen
            case 1792:
                printf("Touch pressed: (x = %f, y = %f)\n", event.tfinger.x, event.tfinger.y);
                if (testCollisions(gSystem.wWidth * event.tfinger.x, gSystem.wHeight * event.tfinger.y)) {
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

        SDL_Delay(10);
    }

    SDL_DestroyRenderer(gSystem.renderer);
    SDL_DestroyWindow(gSystem.window);
    SDL_Quit();
}
