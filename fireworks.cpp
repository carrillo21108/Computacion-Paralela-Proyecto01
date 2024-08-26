
#include <SDL2/SDL.h>
#include <random>
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

#define WIDTH 1024 
#define HEIGHT 768 

using namespace std;

// Clase que representa cada partícula en la explosión
class Particula {
public:
    float x, y; // Posición de la partícula
    float vx, vy; // Velocidad de la partícula
    int life; // Vida de la partícula (en frames)
    int r, g, b; // Color de la partícula
    int size; // Tamaño de la partícula

    // Constructor que inicializa la posición, velocidad, color, tamaño y vida de la partícula
    Particula(float x, float y, int vidaParticula) {
        this->x = x;
        this->y = y;
        this->life = vidaParticula; // Asignar la vida de la partícula recibida como parámetro

        // Genera una dirección aleatoria en todas direcciones
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dist(0, 2 * M_PI);

        float angle = dist(gen);
        float speed = static_cast<float>(rand() % 3 + 1); // Velocidad más lenta

        this->vx = cos(angle) * speed;
        this->vy = sin(angle) * speed;

        // Color aleatorio
        this->r = rand() % 256;
        this->g = rand() % 256;
        this->b = rand() % 256;

        // Tamaño aleatorio (más grande)
        this->size = rand() % 5;
    }

    // Actualiza la posición de la partícula en cada frame y rebota en los bordes
    void update() {
        x += vx;
        y += vy;

        // Rebote en los bordes
        if (x <= 0 || x >= WIDTH) {
            vx = -vx * 0.2f; // Invertir velocidad en x y reducirla drásticamente
        }
        if (y <= 0 || y >= HEIGHT) {
            vy = -vy * 0.2f; // Invertir velocidad en y y reducirla drásticamente
        }

        life--; // Reduce la vida en cada frame
    }

    // Comprueba si la partícula sigue viva
    bool isAlive() {
        return life > 0;
    }
};

// Clase que representa un cohete que sube y luego explota en partículas
class Cohete {
public:
    float x, y; // Posición del cohete
    bool exploded; // Indica si el cohete ha explotado
    float explosionHeight; // Altura a la que explota el cohete
    vector<Particula> particulas; // Vector que contiene las partículas generadas por la explosión
    int size; // Tamaño del cohete
    int numParticles; // Número de partículas por explosión
    int vidaParticula; // Vida de cada partícula

    // Constructor que inicializa la posición y la altura de explosión del cohete
    Cohete(float x, int numParticles, int vidaParticula) : numParticles(numParticles), vidaParticula(vidaParticula) {
        this->x = x;
        this->y = HEIGHT; // Empieza desde la parte inferior de la pantalla
        this->exploded = false; // Inicialmente no ha explotado
        this->size = 3; // Tamaño del cohete más grande

        // Altura de explosión aleatoria entre el 25% y el 75% de la altura total
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> distHeight(HEIGHT * 0.25, HEIGHT * 0.75);
        this->explosionHeight = distHeight(gen);
    }

    // Actualiza la posición del cohete o de las partículas si ya explotó
    void update() {
        if (!exploded) {
            y -= 2; // Velocidad de ascenso más lenta
            if (y < explosionHeight) { // Altura de explosión aleatoria
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

    // Genera la explosión del cohete en partículas
    void explode() {
        exploded = true;
        for (int i = 0; i < numParticles; i++) {
            particulas.push_back(Particula(x, y, vidaParticula)); // Pasa la vida de las partículas al constructor
        }
    }

    // Renderiza el cohete o las partículas en la pantalla
    void render(SDL_Renderer* renderer) {
        if (!exploded) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_Rect rect = { static_cast<int>(x - size / 2), static_cast<int>(y), size, size * 2 };
            SDL_RenderFillRect(renderer, &rect); // Cohete como un rectángulo más grande
        } else {
            for (auto& p : particulas) {
                SDL_SetRenderDrawColor(renderer, p.r, p.g, p.b, 255); // Partículas de colores aleatorios
                SDL_Rect rect = { static_cast<int>(p.x), static_cast<int>(p.y), p.size, p.size };
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }

    // Comprueba si el cohete ha terminado su explosión
    bool isDone() {
        return exploded && particulas.empty();
    }
};

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cout << "Uso: " << argv[0] << " <cantidad_de_particulas_por_cohete> <vida_de_particulas>" << std::endl;
        return 1;
    }

    int numParticles = atoi(argv[1]); // Número de partículas por explosión
    int vidaParticula = atoi(argv[2]); // Vida de cada partícula

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
    vector<Cohete> cohetes;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(0, WIDTH);

    int frames = 0;
    Uint32 startTime = SDL_GetTicks();

    // Bucle principal del programa
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        // Generar un nuevo cohete con cierta probabilidad
        if (rand() % 100 < 2) {
            cohetes.push_back(Cohete(dist(gen), numParticles, vidaParticula));
        }

        // Limpiar la pantalla
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Actualizar y renderizar cada cohete
        for (auto& cohete : cohetes) {
            cohete.update();
            cohete.render(renderer);
        }

        // Eliminar cohetes que han terminado su ciclo
        cohetes.erase(
            remove_if(cohetes.begin(), cohetes.end(), [](Cohete& c) { return c.isDone(); }),
            cohetes.end()
        );

        // Calcular y mostrar FPS
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

        // Actualizar la pantalla
        SDL_RenderPresent(renderer);
        SDL_Delay(20);
    }

    // Limpiar recursos y salir
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

