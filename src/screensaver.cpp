#include <SDL2/SDL.h>
#include <iostream>
#include <cmath>

int SCREEN_WIDTH;
int SCREEN_HEIGHT;

struct Cube
{
    float x, y;
    int angle;
    float speed;
};
struct Vector2
{
    float vx, vy;
};

void drawRect(SDL_Renderer *renderer, float x, float y, int w, int h)
{
    SDL_Rect rect = {static_cast<int>(x), static_cast<int>(y), w, h};
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
}

float convertDegreesToRadians(int angle)
{
    return ((M_PI * 2) / 360.0f) * angle;
}

int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cerr << "SDL could not be initialized: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_DisplayMode DM;
    if (SDL_GetCurrentDisplayMode(0, &DM) != 0)
    {
        std::cerr << "Could not retrieve screen resolution: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }
    SCREEN_WIDTH = DM.w;
    SCREEN_HEIGHT = DM.h;

    SDL_Window *window = SDL_CreateWindow("SDL2 Cube", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE);
    if (!window)
    {
        std::cerr << "Window could not be created: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    if (SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN) != 0)
    {
        std::cerr << "Fullscreen mode could not be enabled: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        std::cerr << "Renderer could not be created: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    bool running = true;
    SDL_Event event;

    Cube cube = {100.0f, 100.0f, 45, 5.0f};
    Vector2 direction;
    float radian = convertDegreesToRadians(cube.angle);
    direction.vx = std::cos(radian) * cube.speed;
    direction.vy = std::sin(radian) * cube.speed;

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        if (cube.x + 100 >= SCREEN_WIDTH || cube.x <= 0)
        {
            direction.vx = -direction.vx;
        }
        if (cube.y + 100 >= SCREEN_HEIGHT || cube.y <= 0)
        {
            direction.vy = -direction.vy;
        }

        cube.x += direction.vx;
        cube.y += direction.vy;

        drawRect(renderer, cube.x, cube.y, 100, 100);

        SDL_RenderPresent(renderer);

        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
