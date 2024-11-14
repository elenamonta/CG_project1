#pragma once
#include <glad/glad.h>
#include "ShaderMaker.h"
#include "strutture.h"
#include "inizializzazioni.h"

extern unsigned int programId, programIdS, programId_text, VAO_Text, VBO_Text;

void INIT_SHADER(void)
{
    GLenum ErrorCheckValue = glGetError();

    char* vertexShader = (char*)"vertexshaderM.glsl";
    char* fragmentShader = (char*)"fragmentshaderM.glsl";
    programId = ShaderMaker::createProgram(vertexShader, fragmentShader);

    //background
    vertexShader = (char*)"vertexshaderSf.glsl";
    fragmentShader = (char*)"fragmentShaderSf.glsl";
    programIdS = ShaderMaker::createProgram(vertexShader, fragmentShader);

    vertexShader = (char*)"VertexShader_Text.glsl";
    fragmentShader = (char*)"FragmentShader_Text.glsl";

    programId_text = ShaderMaker::createProgram(vertexShader, fragmentShader);
}

void INIT_VAO(Figura* fig)
{
       
    glGenVertexArrays(1, &fig->VAO);
    glBindVertexArray(fig->VAO);

    //vertici
    glGenBuffers(1, &fig->VBO_vertices);
    glBindBuffer(GL_ARRAY_BUFFER, fig->VBO_vertices);
    glBufferData(GL_ARRAY_BUFFER, fig->vertices.size() * sizeof(vec3), fig->vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    //colori
    glGenBuffers(1, &fig->VBO_colors);
    glBindBuffer(GL_ARRAY_BUFFER, fig->VBO_colors);
    glBufferData(GL_ARRAY_BUFFER, fig->colors.size() * sizeof(vec4), fig->colors.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(1);

}

void INIT_VAO_Curva(Curva* fig)
{

    glGenVertexArrays(1, &fig->VAO);
    glBindVertexArray(fig->VAO);

    //Genero , rendo attivo, riempio il VBO della geometria dei vertici di COntrollo
    glGenBuffers(1, &fig->VBO_vertices);
    glBindBuffer(GL_ARRAY_BUFFER, fig->VBO_vertices);
    glBufferData(GL_ARRAY_BUFFER, fig->nv * sizeof(vec3), fig->vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    //Genero , rendo attivo, riempio il VBO dei colori nei vertici di controllo
    glGenBuffers(1, &fig->VBO_colors);
    glBindBuffer(GL_ARRAY_BUFFER, fig->VBO_colors);
    glBufferData(GL_ARRAY_BUFFER, fig->nv * sizeof(vec4), fig->colors.data(), GL_STATIC_DRAW);
    //Adesso carico il VBO dei colori nel layer 2
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(1);


    //Genero , rendo attivo, riempio il VBO della geometria dei vertici di COntrollo
    glGenBuffers(1, &fig->VBO_CP);
    glBindBuffer(GL_ARRAY_BUFFER, fig->VBO_CP);
    glBufferData(GL_ARRAY_BUFFER, fig->ncp * sizeof(vec3), fig->CP.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(2);

    //Genero , rendo attivo, riempio il VBO dei colori nei vertici di controllo
    glGenBuffers(1, &fig->VBO_CP_colors);
    glBindBuffer(GL_ARRAY_BUFFER, fig->VBO_CP_colors);
    glBufferData(GL_ARRAY_BUFFER, fig->ncp * sizeof(vec4), fig->colCP.data(), GL_STATIC_DRAW);
    //Adesso carico il VBO dei colori nel layer 2
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(3);

    //Genero , rendo attivo, riempio il VBO della geometria dei vertici di COntrollo
    glGenBuffers(1, &fig->VBO_TG);
    glBindBuffer(GL_ARRAY_BUFFER, fig->VBO_TG);
    glBufferData(GL_ARRAY_BUFFER, fig->ntg * sizeof(vec3), fig->tg.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(4);

    //Genero , rendo attivo, riempio il VBO dei colori nei vertici di controllo
    glGenBuffers(1, &fig->VBO_TG_colors);
    glBindBuffer(GL_ARRAY_BUFFER, fig->VBO_TG_colors);
    glBufferData(GL_ARRAY_BUFFER, fig->ntg * sizeof(vec4), fig->col_tg.data(), GL_STATIC_DRAW);
    //Adesso carico il VBO dei colori nel layer 2
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(5);


}

void INIT_VAO_Text(void)
{

    // configure VAO/VBO for texture quads
    // -----------------------------------
    glGenVertexArrays(1, &VAO_Text);
    glGenBuffers(1, &VBO_Text);
    glBindVertexArray(VAO_Text);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Text);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glEnableVertexAttribArray(0);
}