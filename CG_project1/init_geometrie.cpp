#pragma once
#include <iostream>
#include "strutture.h"
#include "Utilities.h"
#include "gestione_curve.h"

#define _CRT_SECURE_NO_WARNINGS
#define PI 3.14159265358979323
extern float alpha;
const vec3 NULL_CENTER = vec3(0.0,0.0,0.0);

void INIT_PIANO(Figura* fig)
{
    fig->vertices.clear();

    fig->vertices.push_back(vec3(-0.5, -0.5, 0.0));
    fig->colors.push_back(vec4(1.0, 1.0, 1.0, 1.0));
    fig->vertices.push_back(vec3(0.5,-0.5, 0.0));
    fig->colors.push_back(vec4(1.0, 1.0, 1.0, 1.0));
    fig->vertices.push_back(vec3(0.5, 0.5, 0.0));
    fig->colors.push_back(vec4(1.0, 1.0, 1.0, 1.0));
    fig->vertices.push_back(vec3(-0.5, 0.5, 0.0));
    fig->colors.push_back(vec4(1.0, 1.0, 1.0, 1.0));

    findBB(fig);

    fig->vertices.push_back(vec3(fig->min_BB_obj.x, fig->min_BB_obj.y, 0.0));
    fig->colors.push_back(vec4(1.0, 1.0, 1.0, 1.0));
    fig->vertices.push_back(vec3(fig->max_BB_obj.x, fig->min_BB_obj.y, 0.0));
    fig->colors.push_back(vec4(1.0, 1.0, 1.0, 1.0));
    fig->vertices.push_back(vec3(fig->max_BB_obj.x, fig->max_BB_obj.y, 0.0));
    fig->colors.push_back(vec4(1.0, 1.0, 1.0, 1.0));
    fig->vertices.push_back(vec3(fig->min_BB_obj.x, fig->max_BB_obj.y, 0.0));
    fig->colors.push_back(vec4(1.0, 1.0, 1.0, 1.0));

    fig->nv = fig->vertices.size();
    fig->render = GL_TRIANGLE_FAN;


     
 }

void INIT_FORMA(Curva* curva, const char* filename, int render)
{
    float* t;
    float step_t;
    int i;
    struct Dati {
        float x;
        float y;
        float z;
        float r;
        float g;
        float b;
    };

    FILE* file;
    errno_t err = fopen_s(&file, filename, "r");
    if (err != 0) {
        std::cout << "impossibile aprire il file" << std::endl; 
    }


    // Vettore per memorizzare i dati
    struct Dati dati[1000]; // Supponiamo che ci siano al massimo 100 righe nel file

    int riga = 0;
    while (fscanf_s(file, "%f %f %f %f %f %f", &dati[riga].x, &dati[riga].y, &dati[riga].z, &dati[riga].r, &dati[riga].g, &dati[riga].b) == 6) {
        // Incrementa l'indice della riga
        riga++;

        // Puoi aggiungere un controllo qui per evitare il superamento dell'array dati
        if (riga >= 1000) {
            printf("Troppe righe nel file. L'array dati   stato completamente riempito.\n");
            break;
        }
    }



    // Chiudi il file
    fclose(file);


    for (int i = 0; i < riga; i++) {
        curva->CP.push_back(vec3(dati[i].x, dati[i].y, dati[i].z));
        curva->colorVertices = vec4(dati[i].r, dati[i].g, dati[i].b, 1.0f);
        curva->colCP.push_back(vec4(0.0, 0.0, 0.0, 1.0));
        curva->Derivata.push_back(vec3(0.0, 0.0, 0.0));
    }
    curva->ncp = curva->CP.size();


    t = new float[curva->CP.size()];
    step_t = 1.0 / (curva->CP.size() - 1);
    for (i = 0; i < curva->CP.size(); i++)
        t[i] = (float)i * step_t;

    t[curva->CP.size()] = 1.0;
    float sumx = 0;
    float sumy = 0;
    float sumz = 0;
    for (int i = 0; i < curva->CP.size(); i++) {
        sumx += curva->CP[i].x; 
        sumy += curva->CP[i].y;
        sumz += curva->CP[i].z;
    }
    vec3 centro = vec3(sumx/curva->CP.size(), sumy / curva->CP.size(), 0.0);
    /*if (!with_center) {
        centro = NULL_CENTER;
    }*/
    
    CostruisciHermite(t, curva, centro, curva->colorVertices);
    findBB_Curva(curva);


    /*if (centro != NULL_CENTER) {
        curva->nv = curva->vertices.size();
        curva->render = GL_TRIANGLE_FAN;
    }
    else {
        curva->nv = curva->vertices.size();
        curva->render = GL_LINE_STRIP;
    }*/

    curva->nv = curva->vertices.size();
    curva->render = render;
}

