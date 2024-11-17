#pragma once
#include <fstream>
#include <iostream>
#include "lib.h"
#include "gestione_interazioni.h"
#include "strutture.h"
#include "rendering.h"
#include "inizializzazioni.h"
#include "init_geometrie.h"
#include "gestione_curve.h"
#include "Utilities.h"
#include "init_game.h"

int height = 800, width = 800, speed, nPlatform = 6, points=0;
unsigned int programId, programIdS, programId_text, VAO_Text, VBO_Text;
float r = 0.0f, g = 0.0f, b = 0.0f, deltaTime = 0.0f, lastFrame, alpha, raggio = 1.0f, timeAcc, direction = 0.0f, velocity = 1.0f;
float Tens = 0.0, Bias = 0.0, Cont = 0.0, step_t;
float* t; int counter=0;
bool inGame = true; 

mat4 Projection;
vec2 resolution;
Figura background;
GLuint MatProj, MatModel, MatProjS, MatModelS, vec_resS, loc_time, loc_speed, MatProjText, text_color;
Curva player = {}, highest = {}, cap = {};
Glyph glyph = {}; 
vector<Curva> platforms;
vector<Curva> bouncings;
map<char, Glyph> Characters; 




int main(void)
{
	ISoundEngine* engine = createIrrKlangDevice();

	GLFWwindow* window;

	if (!glfwInit()) return 1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);

	window = glfwCreateWindow(height, width, "Game", NULL, NULL);

	if (!window)
	{
		std::cout << "Failed to create the window !" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failde to load opengl function pointers !" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwSetKeyCallback(window, key_callback);

	glViewport(0, 0, width, height);
	//glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	INIT_SHADER();


	
	background.nTriangles = 2;
	background.programId = programIdS;
	background.scale = vec3(2.0 * float(width), 2.0 * float(height), 1.0);

	INIT_PIANO(&background);
	INIT_VAO(&background);

	initShape();


	 
	//loop game
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		int outOfBoundPlatformNum = 0;
		int outOfBoundBouncNum = 0;

		updateBB_Curva(&player);
		updateBB_Curva(&cap);

		for (int i = 0; i < platforms.size(); i++) {
			updateBB_Curva(&platforms[i]); 
		}
	

		if (velocity < 0) {
			if (checkCollision_platform(player, platforms)) {
				if (!checkCollision(player, platforms[0])) {
					points += 5; 
				}
				velocity = 8;
				if (checkCollision_platform(player, bouncings)) {
					velocity *= 2;
					points += 20;

					engine->play2D("media/spring.mp3", false);
				}

				engine->play2D("media/jump.mp3", false);
			}
		}
		 
		 
		updatePlayer(&player); 
		cap.position.x = player.position.x - 10.0;
		cap.position.y = player.position.y - (cap.min_BB_obj.y * cap.scale.y - player.max_BB_obj.y * player.scale.y) - 20.0;

		

		
		if (player.position.y >= height / 2) {
			player.position.y -= velocity; 
			for (int j = 0; j < bouncings.size(); j++) {
				bouncings[j].position.y -= velocity;
				if (outOfBound(bouncings[j])) {
					outOfBoundBouncNum++;
				}
			}

			while (outOfBoundPlatformNum > 0) {
				bouncings.erase(bouncings.begin());
				outOfBoundBouncNum--;
			}


			for (int i = 0; i < platforms.size(); i++) {
				platforms[i].position.y -= velocity;

				if (outOfBound(platforms[i])) {
					outOfBoundPlatformNum++;
				}
			}

			while (outOfBoundPlatformNum > 0) {

				highest = higher_platform(platforms);
				platforms.erase(platforms.begin());

				Curva platform = {};
				platform.programId = programId;
				platform.scale = vec3(20.0, 20.0, 20.0);
				INIT_FORMA(&platform, "platform_punte.txt", GL_TRIANGLE_FAN);
				platform.position.y = highest.position.y + (height / 5.0f);
				platform.position.x = randomx(platform);
				INIT_VAO_Curva(&platform);
				platforms.push_back(platform);

				counter++;

				if (counter == 3) {
					counter = 0;

					Curva molla = {};
					molla.programId = programId;
					molla.scale = vec3(90.0, 90.0, 90.0);
					INIT_FORMA(&molla, "molla.txt", GL_LINE_STRIP);
					molla.position.x = platforms[platforms.size() - 1].position.x;
					molla.position.y = platforms[platforms.size() - 1].position.y - (molla.min_BB_obj.y * molla.scale.y - platforms[platforms.size() - 1].max_BB_obj.y * platforms[platforms.size() - 1].scale.y);
					INIT_VAO_Curva(&molla);
					bouncings.push_back(molla);
				}

				outOfBoundPlatformNum--;
			}
		}


		render(currentFrame);


		if (player.isalive) {
			std::string text = "points: " + std::to_string(points);
			RenderText(text, glyph);
			renderCurva();
		}
		else {
			platforms.clear();
			bouncings.clear();

			Glyph glyph = {};
			glyph.position.x = width / 2; 
			glyph.position.y = height / 2;
			glyph.color = vec3(1.0f, 1.0f, 1.0f);
			glyph.scale = vec3(1.0f, 1.0f, 1.0f);

			RenderText("GAME OVER", glyph);

			glyph.position.y = height / 2 - 40.0;
			glyph.scale = vec3(0.3f, 0.3f, 0.3f);
			std::string text = "points: " + std::to_string(points);
			RenderText(text, glyph);
		}
			
		 
		glfwSwapBuffers(window);
		glfwPollEvents();

	}



	glDeleteProgram(programId);
	glDeleteProgram(programIdS);
	glDeleteBuffers(1, &background.VBO_vertices);
	glDeleteBuffers(1, &background.VBO_colors);
	glDeleteVertexArrays(1, &background.VAO);
	
	glDeleteBuffers(1, &player.VBO_vertices);
	glDeleteBuffers(1, &player.VBO_colors);
	glDeleteVertexArrays(1, &player.VAO);
	
	for (int i = 0; i < platforms.size(); i++) {
		glDeleteBuffers(1, &platforms[i].VBO_vertices);
		glDeleteBuffers(1, &platforms[i].VBO_colors);
		glDeleteVertexArrays(1, &platforms[i].VAO);
	}

	for (int j = 0; j < bouncings.size(); j++) {
		glDeleteBuffers(1, &bouncings[j].VBO_vertices);
		glDeleteBuffers(1, &bouncings[j].VBO_colors);
		glDeleteVertexArrays(1, &bouncings[j].VAO);
	}
	engine->drop();
	

	glfwTerminate();
	return 0;
}

