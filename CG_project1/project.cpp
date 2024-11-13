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
#include "Gui.h"
#include "Utilities.h"
#include <math.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

int height = 800, width = 800, speed, nPlatform = 6;
unsigned int programId, programIdS;
float r = 0.0f, g = 0.0f, b = 0.0f, deltaTime = 0.0f, lastFrame, alpha, raggio = 1.0f, timeAcc, direction = 0.0f, velocity = 1.0f;
float Tens = 0.0, Bias = 0.0, Cont = 0.0, step_t;
float* t; int counter=0;

mat4 Projection;
vec2 resolution;
Figura background;
GLuint MatProj, MatModel, MatProjS, MatModelS, vec_resS, loc_time, loc_speed;
Curva player = {}, highest = {};
vector<Curva> platforms;
vector<Curva> bouncings;

int main(void)
{
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

	INIT_SHADER();

	background.nTriangles = 2;
	background.programId = programIdS;
	background.scale = vec3(2.0 * float(width), 2.0 * float(height), 1.0);

	INIT_PIANO(&background);
	INIT_VAO(&background);

	player.programId = programId;
	player.position.x = width / 2.0;
	player.position.y = height / 5.0;
	player.isalive = true; 
	player.scale = vec3(30.0, 30.0, 30.0); 
	INIT_FORMA(&player, "mycurve.txt", GL_TRIANGLE_FAN);
	INIT_VAO_Curva(&player);

	for (int i = 0; i < nPlatform; i++) {
		Curva platform = {};
		platform.programId = programId;
		platform.scale = vec3(20.0, 20.0, 20.0);
		INIT_FORMA(&platform, "platform_punte.txt", GL_TRIANGLE_FAN);
		if (i == 0) {
			platform.position.y = player.position.y - (((player.max_BB_obj.y - player.min_BB_obj.y) / 2) * player.scale.y) - (((platform.max_BB_obj.y - platform.min_BB_obj.y) / 2) * platform.scale.y);
			platform.position.x = player.position.x;
		}
		else {
			platform.position.y = platforms[platforms.size() - 1].position.y + (height / 5.0f);
			platform.position.x = randomx(platform);
		}
		INIT_VAO_Curva(&platform);
		platforms.push_back(platform);
	}

	Projection = ortho(0.0f, float(width), 0.0f, float(height));
	resolution = vec2(float(height), float(width));

	MatProj = glGetUniformLocation(programId, "Projection");
	MatModel = glGetUniformLocation(programId, "Model");

	MatProjS = glGetUniformLocation(programIdS, "Projection");
	MatModelS = glGetUniformLocation(programIdS, "Model");
	vec_resS = glGetUniformLocation(programIdS, "resolution");
	loc_time = glGetUniformLocation(programIdS, "time");
	loc_speed = glGetUniformLocation(programIdS, "speed");

	Initialize_IMGUI(window);


	//loop game
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		int outOfBoundPlatformNum = 0;
		int outOfBoundBouncNum = 0;

		if (velocity < 0) {
			if (checkCollision_platform(player, platforms)) {
				velocity = 8;
				if (checkCollision_platform(player, bouncings)) {
					velocity *= 2;
				}
			}
		}
		
		updatePlayer(&player); 

		if (!player.isalive) {
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		}
		
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
					molla.scale = vec3(100.0, 100.0, 100.0);
					INIT_FORMA(&molla, "molla.txt", GL_LINE_STRIP);
					molla.position.x = platforms[platforms.size() - 1].position.x;
					molla.position.y = platforms[platforms.size() - 1].position.y - (molla.min_BB_obj.y * molla.scale.y - platforms[platforms.size() - 1].max_BB_obj.y * platforms[platforms.size() - 1].scale.y);
					INIT_VAO_Curva(&molla);
					bouncings.push_back(molla);
				}

				outOfBoundPlatformNum--;
			}
		}

		my_interface();
		render(currentFrame);
		renderCurva();


		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		 
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
	
	

	glfwTerminate();
	return 0;
}
