#include <SDL.h>
#include <stdint.h>
#include <math.h>


#define NUMPOINTS 10000

void clear(SDL_Surface* surface) {
    int height = surface->h;
    int pitch = surface->pitch;
    uint8_t* fb = (uint8_t*)surface->pixels;
    memset(fb, 0, pitch * height);
}

void draw(SDL_Surface* surface) {
    int width = surface->w;
    int height = surface->h;
    int pitch = surface->pitch;
    uint8_t* pixels = (uint8_t*)surface->pixels;

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            uint8_t r = (x * 255) / width;
            uint8_t g = (y * 255) / height;
            uint8_t b = 128;

            pixels[y * pitch + x * 4 + 0] = g;
            pixels[y * pitch + x * 4 + 1] = b;
            pixels[y * pitch + x * 4 + 2] = r;
            pixels[y * pitch + x * 4 + 3] = 1;
        }
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

    clear(surface);
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
