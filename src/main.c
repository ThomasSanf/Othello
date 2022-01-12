#include <printf.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "Othello.h"

enum ePosition
{
    DOWN
};

/**
* Dessine un cercle rempli
*/
void draw_circle(SDL_Renderer *renderer, int x, int y, int radius, SDL_Color color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    for (int w = 0; w < radius * 2; w++)
    {
        for (int h = 0; h < radius * 2; h++)
        {
            int dx = radius - w;
            int dy = radius - h;
            if ((dx*dx + dy*dy) <= (radius * radius))
            {
                SDL_RenderDrawPoint(renderer, x + dx, y + dy);
            }
        }
    }
}

/**
* affiche pions
*/
void affich(int tab[8][8], SDL_Renderer* renderer)
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (tab[i][j] == -1)
            {
                SDL_Color noir = {0,0,0};
                draw_circle(renderer, (i*80)+40, (j*80)+40, 30, noir);
            }
            if (tab[i][j] == 1)
            {
                SDL_Color blanc = {255,255,255};
                draw_circle(renderer, (i*80)+40, (j*80)+40, 30, blanc);
            }
        }
    }
}

int main(int argc, char** argv)
{
    int tab[8][8];
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            tab[i][j] = 0;
        }
    }
    tab[3][3] = 1;
    tab[4][4] = 1;
    tab[4][3] = -1;
    tab[3][4] = -1;

    int game;

    SDL_Window* window;
    SDL_Renderer* renderer;

    SDL_Texture* texture;
    SDL_Surface* image;
    SDL_Rect imagesize;
    SDL_Rect placementpion;

    SDL_Event e;
    int quit = 0;

    int pos = DOWN;

    int x, y, a, b;

    SDL_Color color = {255, 255, 255};

    Uint32 buttons;

    if (!SDL_WasInit(SDL_INIT_VIDEO))
    {
        if (SDL_Init(SDL_INIT_VIDEO) != 0)
        {
            printf("[-] ERROR - Failed to initialise SDL (%s)\n", SDL_GetError());
            return EXIT_FAILURE;
        }
    }

    if (!IMG_Init(IMG_INIT_PNG))
    {
        printf("[-] ERROR - Failed to initialise SDL_Image (%s)\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    window = SDL_CreateWindow("Ma fenetre SDL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 640, SDL_WINDOW_SHOWN);
    if (!window)
    {
        printf("[-] ERROR - Failed to create SDL window (%s)\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    image = IMG_Load("data/plateau.png");
    texture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_FreeSurface(image);


    SDL_SetRenderDrawColor(renderer, 10, 200, 100, 255);

    while (!quit)
    {
        game = 0;
        while(game==0){
            affich(tab, renderer);
            game= joue(1,tab);
        }
        if(Endgame(tab == 1)){
            if(comptage(tab)){
                printf("Bravo c'est le joueur blanc qui a gagné");
                break;
            }
            else{
                printf("Bravo c'est le joueur noir qui a gagné");
                break;
            }
        }
        game = 0;
        while(game==0){
            affich(tab, renderer);
            game= joue(-1,tab);
        }
        if(Endgame(tab == 1)){
            if(comptage(tab)){
                printf("Bravo c'est le joueur blanc qui a gagné");
                break;
            }
            else{
                printf("Bravo c'est le joueur noir qui a gagné");
                break;
            }
        }
        SDL_RenderClear(renderer);
        //afficher le tableau
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        draw_circle(renderer,40,40,30, color);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}