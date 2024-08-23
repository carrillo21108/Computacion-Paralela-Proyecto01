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


class Cohete {
public:
    float x, y;
    bool exploded;
    std::vector<Particula> particulas;

    Cohete(float x) {
        this->x = x;
        this->y = HEIGHT;
        this->exploded = false;
    }

    void update() {
        if (exploded) {
            for (auto& p : particulas) {
                p.update();
            }
            particulas.erase(
                std::remove_if(particulas.begin(), particulas.end(), [](Particula& p) { return !p.isAlive(); }),
                particulas.end()
            );
        }
    }

    void explode() {
        exploded = true;
        for (int i = 0; i < 100; i++) {
            particulas.push_back(Particula(x, y));
        }
    }

    void render(SDL_Renderer* renderer) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_Rect rect = { static_cast<int>(x - 2), static_cast<int>(y), 4, 8 };
        SDL_RenderFillRect(renderer, &rect);

        if (exploded) {
            for (auto& p : particulas) {
                SDL_SetRenderDrawColor(renderer, p.r, p.g, p.b, 255);
                SDL_Rect particleRect = { static_cast<int>(p.x), static_cast<int>(p.y), p.size, p.size };
                SDL_RenderFillRect(renderer, &particleRect);
            }
        }
    }

    bool isDone() {
        return exploded && particulas.empty();
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