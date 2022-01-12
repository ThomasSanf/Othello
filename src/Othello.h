#pragma once

#include <SDL_render.h>

void affich(int tab[8][8], SDL_Renderer* renderer);
int joue(int player, int tab[8][8]);
int Endgame(int tab[8][8]);
int comptage(int tab[8][8]);