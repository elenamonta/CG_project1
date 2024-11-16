#pragma once
#include "strutture.h"
#include "lib.h"

vec2 randomPosition();
float randomx(Curva platform); 
void findBB(Figura* fig);
void findBB_Curva(Curva* fig);
bool checkCollision(Curva obj1, Curva obj2);
bool checkCollision_platform(Curva obj1, vector<Curva> obj2);
void updateBB(Figura* fig);
void updateBB_Curva(Curva* fig);
bool outOfBound(Curva obj);
void updatePlayer(Curva * player);
Curva higher_platform(vector<Curva> platforms);
int generateRand();
void LoadFonts(const std::string& fontPath, unsigned int fontSize);