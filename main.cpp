#include <SDL2/SDL.h>
#include <vector>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <map>
#include <set>

using namespace std;

struct Particle
{
    float x, y;
    float vx, vy;
    int id;
    float ox, oy;
    float phaseX, phaseY;
};

void drawCircle(SDL_Renderer *renderer, int cx, int cy, int r)
{
    for (int w = 0; w < r * 2; w++)
        for (int h = 0; h < r * 2; h++)
            if ((r - w) * (r - w) + (h - r) * (h - r) <= r * r)
                SDL_RenderDrawPoint(renderer, cx + w - r, cy + h - r);
}

int main()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        cout << "SDL error: " << SDL_GetError() << endl;
        return 1;
    }

    const int SCREEN_WIDTH = 1920;
    const int SCREEN_HEIGHT = 1080;

    SDL_Window *window = SDL_CreateWindow("Particles",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          SCREEN_WIDTH,
                                          SCREEN_HEIGHT,
                                          SDL_WINDOW_SHOWN);

    if (!window)
    {
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    bool running = true;
    SDL_Event event;

    vector<Particle> particles;
    map<int, set<int>> connections;
    int nextID = 0;
    int frame = 0;

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                running = false;
        }

        frame++;
        
        if (particles.size() < 80)
        {
            int numNew = 1 + rand() % 2;
            for (int n = 0; n < numNew; n++)
            {    
                Particle p;
                p.x = rand() % SCREEN_WIDTH;
                p.y = rand() % SCREEN_HEIGHT;
                p.ox = p.x;
                p.oy = p.y;
                float angle = ((float)rand() / RAND_MAX) * 2 * M_PI;
                float speed = 1.5f + ((float)rand() / RAND_MAX) * 2.5f;
                p.vx = speed * cos(angle);
                p.vy = speed * sin(angle);
                p.phaseX = ((float)rand() / RAND_MAX) * 2 * M_PI;
                p.phaseY = ((float)rand() / RAND_MAX) * 2 * M_PI;
                p.id = nextID++;
                particles.push_back(p);
            }
        }

        for (auto &p : particles)
        {
            p.x += p.vx;
            p.y += p.vy;

            if (p.x < 0 || p.x > SCREEN_WIDTH)
                p.vx *= -1;
            if (p.y < 0 || p.y > SCREEN_HEIGHT)
                p.vy *= -1;

            p.x += sin(frame * 0.05 + p.phaseX) * 0.5;
            p.y += cos(frame * 0.05 + p.phaseY) * 0.5;
        }

        for (auto &c : connections)
            c.second.clear();

        for (size_t i = 0; i < particles.size(); i++)
        {
            Particle &p1 = particles[i];
            int countP1 = 0;
            for (size_t j = 0; j < particles.size(); j++)
            {
                if (i == j)
                    continue;
                Particle &p2 = particles[j];
                float dist = sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
                if (dist < 120 && countP1 < 2 && connections[p2.id].size() < 2)
                {
                    if (connections[p1.id].count(p2.id) == 0)
                    {
                        connections[p1.id].insert(p2.id);
                        countP1++;
                    }
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        for (auto &c : connections)
        {
            int id1 = c.first;
            for (int id2 : c.second)
            {
                Particle *p1 = nullptr, *p2 = nullptr;
                for (auto &p : particles)
                {
                    if (p.id == id1)
                        p1 = &p;
                    if (p.id == id2)
                        p2 = &p;
                }
                if (p1 && p2)
                {
                    float dist = sqrt(pow(p1->x - p2->x, 2) + pow(p1->y - p2->y, 2));
                    if (dist > 120)
                        continue;
                    float alpha = 50.0f * (1.0f - dist / 120.0f); // fade suave
                    if (alpha < 0)
                        alpha = 0;
                    SDL_SetRenderDrawColor(renderer, 0, 255, 0, (Uint8)alpha);
                    SDL_RenderDrawLine(renderer, (int)p1->x, (int)p1->y, (int)p2->x, (int)p2->y);
                }
            }
        }

        for (auto &p : particles)
        {
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
            drawCircle(renderer, (int)p.x, (int)p.y, 2);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
