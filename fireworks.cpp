#include <SDL2/SDL.h>
#include <random>
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

#define WIDTH 1024 
#define HEIGHT 768 

using namespace std;

class Particula {
public:
    float x, y; 
    float vx, vy;
    int life;
    int r, g, b; 
    int size; 

    Particula(float x, float y, int vidaParticula) {
        this->x = x;
        this->y = y;
        this->life = vidaParticula; 

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dist(0, 2 * M_PI);

        float angle = dist(gen);
        float speed = static_cast<float>(rand() % 3 + 1); 

        this->vx = cos(angle) * speed;
        this->vy = sin(angle) * speed;

        this->r = rand() % 256;
        this->g = rand() % 256;
        this->b = rand() % 256;

        this->size = rand() % 5;
    }

    void update() {
        x += vx;
        y += vy;

        if (x <= 0 || x >= WIDTH) {
            vx = -vx * 0.2f; 
        }
        if (y <= 0 || y >= HEIGHT) {
            vy = -vy * 0.2f; 
        }

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
    float explosionHeight; 
    vector<Particula> particulas; 
    int size; 
    int numParticles; 
    int vidaParticula; 

    Cohete(float x, int numParticles, int vidaParticula) : numParticles(numParticles), vidaParticula(vidaParticula) {
        this->x = x;
        this->y = HEIGHT; 
        this->exploded = false; 
        this->size = 3; 

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> distHeight(HEIGHT * 0.25, HEIGHT * 0.75);
        this->explosionHeight = distHeight(gen);
    }

    void update() {
        if (!exploded) {
            y -= 2;
            if (y < explosionHeight) {
                explode();
            }
        } else {
            for (auto& p : particulas) {
                p.update();
            }
            particulas.erase(
                remove_if(particulas.begin(), particulas.end(), [](Particula& p) { return !p.isAlive(); }),
                particulas.end()
            );
        }
    }

    void explode() {
        exploded = true;
        for (int i = 0; i < numParticles; i++) {
            particulas.push_back(Particula(x, y, vidaParticula));
        }
    }

    void render(SDL_Renderer* renderer) {
        if (!exploded) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_Rect rect = { static_cast<int>(x - size / 2), static_cast<int>(y), size, size * 2 };
            SDL_RenderFillRect(renderer, &rect); 
        } else {
            for (auto& p : particulas) {
                SDL_SetRenderDrawColor(renderer, p.r, p.g, p.b, 255); 
                SDL_Rect rect = { static_cast<int>(p.x), static_cast<int>(p.y), p.size, p.size };
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }

    bool isDone() {
        return exploded && particulas.empty();
    }
};

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cout << "Uso: " << argv[0] << " <cantidad_de_particulas_por_cohete> <vida_de_particulas>" << std::endl;
        return 1;
    }

    int numParticles = atoi(argv[1]);
    int vidaParticula = atoi(argv[2]);

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
    vector<Cohete> cohetes;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(0, WIDTH);

    int frames = 0;
    Uint32 startTime = SDL_GetTicks();

    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        if (rand() % 100 < 2) {
            cohetes.push_back(Cohete(dist(gen), numParticles, vidaParticula));
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        for (auto& cohete : cohetes) {
            cohete.update();
            cohete.render(renderer);
        }

        cohetes.erase(
            remove_if(cohetes.begin(), cohetes.end(), [](Cohete& c) { return c.isDone(); }),
            cohetes.end()
        );

        frames++;
        Uint32 currentTime = SDL_GetTicks();
        Uint32 deltaTime = currentTime - startTime;

        if (deltaTime >= 1000) {
            float fps = frames / (deltaTime / 1000.0f);
            frames = 0;
            startTime = currentTime;

            std::string fpsText = "FPS: " + std::to_string(static_cast<int>(fps));
            SDL_SetWindowTitle(window, fpsText.c_str());
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(20);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}