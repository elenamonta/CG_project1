#pragma once
#include "lib.h"
#include "strutture.h"
#include <random>
extern int width, height; 
extern float direction, velocity;

vec2 randomPosition() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<> disX(0.0f, static_cast<float>(width));
    std::uniform_real_distribution<> disY(0.0f, static_cast<float>(height));
    return glm::vec2(disX(gen), disY(gen));
}

int generateRand() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> disX(0, 9);
    return disX(gen);
}

float randomx(Curva platform) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    float width_plat = ((platform.max_BB_obj.x - platform.min_BB_obj.x) / 2)* platform.scale.x;
    std::uniform_real_distribution<> disX(width_plat, static_cast<float>(width-width_plat));
    return disX(gen); 
}

void findBB(Figura* fig) {
    int n;
    n = fig->vertices.size();
    float minx = fig->vertices[0].x; // Assumiamo che il primo elemento sia il minimo iniziale
    float miny = fig->vertices[0].y; // Assumiamo che il primo elemento sia il minimo iniziale

    float maxx = fig->vertices[0].x; // Assumiamo che il primo elemento sia il minimo iniziale
    float maxy = fig->vertices[0].y; // Assumiamo che il primo elemento sia il minimo iniziale

    for (int i = 1; i < n; i++) {
        if (fig->vertices[i].x < minx) {
            minx = fig->vertices[i].x;
        }
        if (fig->vertices[i].x > maxx) {
            maxx = fig->vertices[i].x;
        }

        if (fig->vertices[i].y < miny) {
            miny = fig->vertices[i].y;
        }

        if (fig->vertices[i].y > maxy) {
            maxy = fig->vertices[i].y;
        }
    }

    fig->min_BB_obj = vec4(minx, miny, 0.0, 1.0);
    fig->max_BB_obj = vec4(maxx, maxy, 0.0, 1.0);

}


void findBB_Curva(Curva* fig) {
    int n;
    n = fig->vertices.size();
    float minx = fig->vertices[0].x; // Assumiamo che il primo elemento sia il minimo iniziale
    float miny = fig->vertices[0].y; // Assumiamo che il primo elemento sia il minimo iniziale

    float maxx = fig->vertices[0].x; // Assumiamo che il primo elemento sia il minimo iniziale
    float maxy = fig->vertices[0].y; // Assumiamo che il primo elemento sia il minimo iniziale

    for (int i = 1; i < n; i++) {
        if (fig->vertices[i].x < minx) {
            minx = fig->vertices[i].x;
        }
        if (fig->vertices[i].x > maxx) {
            maxx = fig->vertices[i].x;
        }

        if (fig->vertices[i].y < miny) {
            miny = fig->vertices[i].y;
        }

        if (fig->vertices[i].y > maxy) {
            maxy = fig->vertices[i].y;
        }
    }

    fig->min_BB_obj = vec4(minx, miny, 0.0, 1.0);
    fig->max_BB_obj = vec4(maxx, maxy, 0.0, 1.0);
}




bool checkCollision(Curva obj1, Curva obj2) {
    // guardo collisioni su asse x

    bool collisionX = obj1.min_BB.x <= obj2.max_BB.x &&
        obj1.max_BB.x >= obj2.min_BB.x;

    // guardo collisioni su asse y

    bool collisionY = obj1.min_BB.y <= obj2.max_BB.y &&
        obj1.max_BB.y >= obj2.min_BB.y;

    return collisionX && collisionY;
}

bool checkCollision_platform(Curva obj1, vector<Curva> vecCurve)
{
    for (const Curva obj2 : vecCurve) {
        bool collisionX = obj1.min_BB.x <= obj2.max_BB.x &&
            obj1.max_BB.x >= obj2.min_BB.x;

        // guardo collisioni su asse y

        bool collisionY = obj1.min_BB.y <= obj2.max_BB.y &&
            obj1.max_BB.y >= obj2.min_BB.y;
        
        if (collisionX && collisionY) {
            return true; 
        }
    }
    return false; 
}

void updateBB(Figura* fig)
{
    //Aggiorno le coordinate del Bounding Box applicandogli la stessa matrice di modellazione della sua shape
    fig->min_BB = fig->min_BB_obj;
    fig->max_BB = fig->max_BB_obj;
    fig->min_BB = fig->Model * fig->min_BB;
    fig->max_BB = fig->Model * fig->max_BB;
}

void updateBB_Curva(Curva* fig) {
    fig->min_BB = fig->min_BB_obj;
    fig->max_BB = fig->max_BB_obj;
    fig->min_BB = fig->Model * fig->min_BB;
    fig->max_BB = fig->Model * fig->max_BB;
}

bool outOfBound(Curva obj) {
    return /*obj.min_BB.y >= height ||*/ obj.max_BB.y <= 0.0; 
}

void updatePlayer(Curva* player) {
    velocity -= 0.1f; 
    player->position.x += direction; 
    player->position.y += velocity; 
    if (player->max_BB.x > width + 1) {
        player->position.x = width - ((player->max_BB.x - player->min_BB.x) / 2);
    }
    if (player->min_BB.x < 0) {
        player->position.x = (player->max_BB.x - player->min_BB.x) / 2;
    }
    if (player->min_BB.y < 0) {
        player->isalive = false; 
    } 
    updateBB_Curva(player); 
}



Curva higher_platform(vector<Curva> platforms) {
    Curva higher = {};
    higher.position.x = 0.0; 
    higher.position.y = 0.0; 
    for (int i = 0; i < platforms.size(); i++) {
        if (platforms[i].position.y > higher.position.y) {
            higher = platforms[i]; 
        }
    }
    return higher; 
}