#pragma once

#include "lib.h"
typedef struct {
    GLuint VAO;  
    GLuint  VBO_vertices; 
    GLuint  VBO_colors;   
    int nTriangles;  
    vector<vec3> vertices;  
    vector<vec4> colors; 
    int nv;  
    int render; 
    unsigned int programId;
    mat4 Model;
    vec2 position;
    vec4 min_BB_obj;
    vec4 max_BB_obj;
    vec4 min_BB;
    vec4 max_BB;
    vec3 scale;
    float timerFig;
    bool isalive;
    float update;
} Figura;


typedef struct {
    // Identificatori OpenGL per la gestione della geometria e degli attributi
    GLuint VAO;                // Vertex Array Object
    GLuint VBO_vertices;       // Vertex Buffer Object per le coordinate dei vertici della curva
    GLuint VBO_colors;         // Vertex Buffer Object per i colori dei vertici della curva
    GLuint VBO_CP;             // Vertex Buffer Object per le coordinate dei control points
    GLuint VBO_CP_colors;      // Vertex Buffer Object per i colori dei control points
    GLuint VBO_TG;             // Vertex Buffer Object per le coordinate delle tangenti
    GLuint VBO_TG_colors;      // Vertex Buffer Object per i colori delle tangenti


    std::vector<glm::vec3> vertices;  // Coordinate dei vertici della curva
    std::vector<glm::vec4> colors;    // Colori dei vertici della curva
    std::vector<glm::vec3> CP;        // Coordinate dei control points (punti di controllo)
    std::vector<glm::vec4> colCP;     // Colori dei control points
    std::vector<glm::vec3> tg;        // Coordinate dei vettori tangenti ai control points
    std::vector<glm::vec4> col_tg;     // Colori dei vettori tangenti
    vec4 colorVertices;
    // Parametri di interpolazione
    std::vector<glm::vec3> Derivata;  // Vettore derivata (a cosa serve esattamente?)
    std::vector<float> Tens;       // Tensore di tensione
    std::vector<float> Bias;       // Bias
    std::vector<float> Cont;       // Controllo (specificare meglio)

    // Informazioni sulla curva
    int nv;                     // Numero di vertici della curva
    int ncp;                    // Numero di control points
    int ntg;                    // Numero di vettori tangenti
    glm::mat4 Model;            // Matrice di modellazione
    vec2 position;
    vec3 scale;
    // Parametri di rendering
    int render;                // Modalità di rendering (linee, triangoli, ...)
    unsigned int programId;    // Identificatore del programma shader
    bool isalive;
    float timerFig;
    vec4 min_BB_obj;
    vec4 max_BB_obj;
    vec4 min_BB;
    vec4 max_BB;
} Curva;

