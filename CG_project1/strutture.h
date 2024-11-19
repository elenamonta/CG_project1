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
    GLuint VAO;                
    GLuint VBO_vertices;       
    GLuint VBO_colors;         
    GLuint VBO_CP;             
    GLuint VBO_CP_colors;      
    GLuint VBO_TG;             
    GLuint VBO_TG_colors;      


    std::vector<glm::vec3> vertices;  
    std::vector<glm::vec4> colors;    
    std::vector<glm::vec3> CP;        
    std::vector<glm::vec4> colCP;     
    std::vector<glm::vec3> tg;        
    std::vector<glm::vec4> col_tg;    
    vec4 colorVertices;
    std::vector<glm::vec3> Derivata;  
    std::vector<float> Tens;       
    std::vector<float> Bias;       
    std::vector<float> Cont;       
    int nv;                     
    int ncp;                    
    int ntg;                    
    glm::mat4 Model;            
    vec2 position;
    vec3 scale;
    int render;                
    unsigned int programId;    
    bool isalive;
    float timerFig;
    vec4 min_BB_obj;
    vec4 max_BB_obj;
    vec4 min_BB;
    vec4 max_BB;
} Curva;

typedef struct {
    unsigned int textureId;
    glm::ivec2 size; //Dimensioni della bitmap del glifo
    glm::ivec2 bearing; //Offset del glifo rispetto alla linea di base del testo
    glm::vec2 scale; //fattore di scalatura
    glm::vec3 color; //colore
    glm::vec2 position; //posizione
    unsigned int advance; //posizione del cursore
}Glyph;