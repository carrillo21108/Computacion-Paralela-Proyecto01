#include <SDL2/SDL.h>
#include <iostream>

#define WIDTH 1024 
#define HEIGHT 768 

class Particula {
    public:
        float x, y; 
        float vx, vy;
        int life;
        int r, g, b;
        int size;

    Particula(float x, float y) {
        this->x = x;
        this->y = y;
        this->life = rand() % 100 + 50; 

        // Generar velocidad aleatoria
        this->vx = rand() % 5 - 2;
        this->vy = rand() % 5 - 2;

        // Color aleatorio
        this->r = rand() % 256;
        this->g = rand() % 256;
        this->b = rand() % 256;

        this->size = rand() % 3 + 1;
    }

    void update() {
        x += vx;
        y += vy;
        life--;
    }

    bool isAlive() {
        return life > 0;
    }
};

int main(int argc, char* argv[]) {
    // Inicialización de SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "Error initializing SDL: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Fireworks", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
    if (!window) {
        std::cout << "Error creating window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cout << "Error creating renderer: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    bool quit = false;
    SDL_Event e;

    // Bucle principal del programa
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_RenderPresent(renderer);
        SDL_Delay(20);
    }

    // Limpiar recursos y salir
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}