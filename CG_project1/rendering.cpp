#pragma once
#include "lib.h"
#include "strutture.h"
#include "Utilities.h"
#include "gestione_curve.h"

extern float r, g, b;
extern unsigned int MatProjS, MatModelS, MatProj, MatModel, vec_resS, loc_time, loc_speed, loc_platform;
extern int  height, width, speed, nPlatform;
extern float deltaTime;
extern mat4 Projection;
extern vec2 resolution;
extern vec3 centro; 
extern Figura background;
extern Curva player; 
extern vector<Curva> platforms;



void render(float currentFrame)
{
	glClearColor(r, g, b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);


    glUseProgram(background.programId);
    background.Model = mat4(1.0);
    background.Model = scale(background.Model, background.scale);

    glUniformMatrix4fv(MatProjS, 1, GL_FALSE, value_ptr(Projection));
    glUniformMatrix4fv(MatModelS, 1, GL_FALSE, value_ptr(background.Model));
    glUniform2fv(vec_resS, 1, value_ptr(resolution));
    glUniform1f(loc_time, currentFrame);
    glUniform1f(loc_speed, speed);

    glBindVertexArray(background.VAO);
    glDrawArrays(background.render, 0, background.nv);

}


void renderCurva() {
    if (player.isalive) {
        glUseProgram(player.programId);
        glUniformMatrix4fv(MatProj, 1, GL_FALSE, value_ptr(Projection));
        player.Model = mat4(1.0);
        player.Model = translate(player.Model, vec3(player.position.x, player.position.y, 0.0));
        player.Model = scale(player.Model, player.scale);
        glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(player.Model));
        glBindVertexArray(player.VAO);

        updateBB_Curva(&player);
        glDrawArrays(player.render, 0, player.nv);
    }
    

    for (int i = 0; i < platforms.size(); i++) {
        platforms[i].Model = mat4(1.0);
        platforms[i].Model = translate(platforms[i].Model, vec3(platforms[i].position.x, platforms[i].position.y, 0.0));
        platforms[i].Model = scale(platforms[i].Model, platforms[i].scale);
        glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(platforms[i].Model));
        glBindVertexArray(platforms[i].VAO);

        updateBB_Curva(&platforms[i]);
        glDrawArrays(platforms[i].render, 0, platforms[i].nv);

    }

    
    

}


