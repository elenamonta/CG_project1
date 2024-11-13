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
extern Curva player, molla; 
extern vector<Curva> platforms, bouncings;



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

    for (int i = 0; i < platforms.size(); i++) {
        glUseProgram(platforms[i].programId);
        glUniformMatrix4fv(MatProj, 1, GL_FALSE, value_ptr(Projection));
        platforms[i].Model = mat4(1.0);
        platforms[i].Model = translate(platforms[i].Model, vec3(platforms[i].position.x, platforms[i].position.y, 0.0));
        platforms[i].Model = scale(platforms[i].Model, platforms[i].scale);
        glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(platforms[i].Model));
        glBindVertexArray(platforms[i].VAO);

        updateBB_Curva(&platforms[i]);
        glDrawArrays(platforms[i].render, 0, platforms[i].nv);

    }

    for (int j = 0; j < bouncings.size(); j++) {
        glUseProgram(bouncings[j].programId);
        glUniformMatrix4fv(MatProj, 1, GL_FALSE, value_ptr(Projection));
        bouncings[j].Model = mat4(1.0);
        bouncings[j].Model = translate(bouncings[j].Model, vec3(bouncings[j].position.x, bouncings[j].position.y, 0.0));
        bouncings[j].Model = scale(bouncings[j].Model, bouncings[j].scale);
        glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(bouncings[j].Model));
        glBindVertexArray(bouncings[j].VAO);

        updateBB_Curva(&bouncings[j]);
        glLineWidth(2.0f);
        glDrawArrays(bouncings[j].render, 1, bouncings[j].nv - 5);
    }

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

}


