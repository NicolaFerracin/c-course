#include <SDL.h>
#include <stdint.h>
#include <math.h>

typedef struct p3 {
    float x, y, z;
} point;

#define NUMPOINTS 10000
point Model[NUMPOINTS];

void clear(SDL_Surface* surface) {
    int height = surface->h;
    int pitch = surface->pitch;
    uint8_t* fb = (uint8_t*)surface->pixels;
    memset(fb, 0, pitch * height);
}

void create_model(void) {
#if 0
    for (int i = 0; i < NUMPOINTS; i++) {
        Model[i].x = -150 + rand() % 300;
        Model[i].y = -150 + rand() % 300;
        Model[i].z = -150 + rand() % 300;
    }
#endif

    int i = 0;
    // We create 10_000 points (50*50). 
    for (float x = -50; x < 50; x++) {
        for (float z = -50; z < 50; z++) {
#if 0
            // We then set the `y` to always the same value so it's a flat 2D plane.
            int y = 10;
#endif

            // Nerdy math to generate some waves
            float y = 10 + (sin(x / 100 * 3.14 * 5) * 5) + (cos(z / 100 * 3.14 * 5) * 5);
            Model[i].x = x * 4;
            Model[i].y = y * 4;
            Model[i].z = z * 4;
            i++;
            if (i == NUMPOINTS) return;
        }
    }
}

void pixel(SDL_Surface* surface, int x, int y, int r, int g, int b) {
    if (x < 0 || x >= surface->w) return;
    if (y < 0 || y >= surface->h) return;

    int pitch = surface->pitch;
    uint8_t* pixels = (uint8_t*)surface->pixels;

    pixels[y * pitch + x * 4 + 0] = b;
    pixels[y * pitch + x * 4 + 1] = g;
    pixels[y * pitch + x * 4 + 2] = r;
    pixels[y * pitch + x * 4 + 3] = 1;
}


void draw(SDL_Surface* surface) {
    int width = surface->w;
    int height = surface->h;
    int cx = width / 2;
    int cy = height / 2;

    clear(surface);
    for (int i = 0; i < NUMPOINTS; i++) {
        // We are drawing a 3D object in a 2D plane. We keep the x and y but we divide by a z factor that simulates depth (aka the side closer to use looks bigger, the side further away looks smaller)
        // The zfactor should increase as the object gets closer.
        // We add 1 so that when it's 0, we keep the original depth.
        float zfactor = 1 + (Model[i].z / 400);
        float x = cx + Model[i].x / zfactor;
        float y = cy + Model[i].y / zfactor;

        pixel(surface, x, y, 255, 255, 255);
    }
}

int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;

    // Initialize SDL video
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return 1;
    }

    // Create window
    SDL_Window* window = SDL_CreateWindow(
        "Pixel Framebuffer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480,
        SDL_WINDOW_SHOWN
    );
    if (!window) {
        SDL_Log("SDL_CreateWindow failed: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Get the window's surface (the framebuffer)
    SDL_Surface* surface = SDL_GetWindowSurface(window);
    if (!surface) {
        SDL_Log("SDL_GetWindowSurface failed: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    create_model();
    int running = 1;
    while (running) {
        draw(surface);
        SDL_UpdateWindowSurface(window);

        // Main loop – wait for quit event
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
        }
        SDL_Delay(16);
    }

    // Cleanup
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
