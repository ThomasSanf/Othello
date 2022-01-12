#include <stdio.h>
#include <stdlib.h>
#include "Othello.h"
#include <stdbool.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>



int joue(int player, int tab[8][8])
{
    SDL_Event e;
    int x= 0;
    int y= 0;
    int a,b;
    Uint32 buttons;
    int yo = 0;
    char choix[3];
    int coef;
    int compare = player * -1;
    if (player == 1)
    {
        printf("\nVous jouez les pions blancs");
    }
    else
    {
        printf("\nVous jouez les pions noirs");
    }
    printf("\nENTREZ UNE CASE DE VOTRE CHOIX :");
    while (SDL_PollEvent(&e))
    {
        if(e.type == SDL_MOUSEBUTTONDOWN){
            buttons = SDL_GetMouseState(&x, &y);
            SDL_Log("\nLa case choisie est: %d, %d", x/80, y/80);
            a = (x-40)/8;
            b = (y-40)/8;
            //SDL_RenderCopy(renderer, texture, NULL, NULL);
        }
    }
    int nb1 = a;
    int nb2 = b;
    printf("nb1 est égal a : %d et nb2 est egal a : %d\n", nb1, nb2);
    if ((nb1 < 8 && nb1 >= 0) && (nb2 < 8 && nb2 >= 0))
    {
        if ((tab[nb1][nb2] == 0))
        {
            for (int i=-1;i<2;i++)
            {
                for (int y = -1; y < 2; y++)
                {
                    if (tab[nb1 + i][nb2 + y] == compare)
                    {
                        coef = 2;
                        while (nb1+coef*i<8 && nb1 + coef * i >= 0 && nb2 + coef * y < 8 && nb2 + coef * y >=0 && tab[nb1+coef*i][nb2+coef*y]!=0 && coef !=0)
                        {
                            if (tab[nb1 + coef * i][nb2 + coef * y] == compare)
                            {
                                coef++;
                            }
                            if (tab[nb1 + coef * i][nb2 + coef * y] == player)
                            {
                                coef--;
                                tab[nb1 + coef * i][nb2 + coef * y] = player;
                                yo = 1;

                            }
                        }
                        if (yo == 1)
                        {
                            tab[nb1][nb2] = player;

                        }
                    }
                }
            }
        }
        if (yo == 1)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
}

void IA(int player,int tab[8][8])
{

}

int Endgame(int tab[8][8])
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (tab[i][j] == 0)
            {
                return 0;
            }
        }
    }
    return 1;
}

int comptage(int tab[8][8])
{
    int a = 0;int b=0;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (tab[i][j] == 1)
            {
                a++;
            }
            if (tab[i][j] == -1)
            {
                b++;
            }
        }
    }
    if (a > b)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}