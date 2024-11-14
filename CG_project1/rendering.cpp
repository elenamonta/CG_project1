#pragma once
#include "lib.h"
#include "strutture.h"
#include "Utilities.h"
#include "gestione_curve.h"

extern float r, g, b;
extern unsigned int MatProjS, MatModelS, MatProj, MatModel, vec_resS, loc_time, loc_speed, loc_platform, programId_text, VAO_Text, VBO_Text, MatProjText, text_color;
extern int  height, width, speed, nPlatform;
extern float deltaTime;
extern mat4 Projection;
extern vec2 resolution;
extern vec3 centro; 
extern Figura background;
extern Curva player, molla; 
extern vector<Curva> platforms, bouncings;
extern map<char, Glyph> Characters;


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

void RenderText(const std::string& text, Glyph glyph) {
    glUseProgram(programId_text);
    glUniformMatrix4fv(MatProjText, 1, GL_FALSE, glm::value_ptr(Projection));

    glUniform3f(text_color, glyph.color.x, glyph.color.y, glyph.color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO_Text);

    for (auto c = text.begin(); c != text.end(); c++) {
        Glyph ch = Characters[*c];

        float xpos = glyph.position.x + ch.bearing.x * glyph.scale.x;
        float ypos = glyph.position.y - (ch.size.y - ch.bearing.y) * glyph.scale.y;

        float w = ch.size.x * glyph.scale.x;
        float h = ch.size.y * glyph.scale.y;

        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }
        };

        glBindTexture(GL_TEXTURE_2D, ch.textureId);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_Text);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        glyph.position.x += (ch.advance >> 6) * glyph.scale.x; // Avanza alla prossima posizione
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}