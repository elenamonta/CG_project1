#include "init_game.h"

extern Curva player; 
extern Glyph glyph; 
extern int nPlatform, width, height;
extern vector<Curva> platforms;
extern unsigned int programId, programIdS, programId_text;
extern GLuint MatProj, MatModel, MatProjS, MatModelS, vec_resS, loc_time, loc_speed, MatProjText, text_color;
extern mat4 Projection;
extern vec2 resolution;

extern Curva cap; 

void initShape() {
	player.programId = programId;
	player.position.x = width / 2.0;
	player.position.y = height / 5.0;
	player.isalive = true;
	player.scale = vec3(30.0, 30.0, 30.0);
	INIT_FORMA(&player, "mycurve.txt", GL_TRIANGLE_FAN);
	INIT_VAO_Curva(&player);


	cap.programId = programId;
	cap.scale = vec3(80.0, 80.0, 80.0);
	INIT_FORMA(&cap, "cappellino.txt", GL_TRIANGLE_FAN);
	cap.position.x = player.position.x - 10.0;
	cap.position.y = player.position.y - (cap.min_BB_obj.y * cap.scale.y - player.max_BB_obj.y * player.scale.y) - 20.0;
	INIT_VAO_Curva(&cap);
	


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

	glyph.scale = vec3(0.3f, 0.3f, 0.3f);
	glyph.position.x = 70.0;
	glyph.position.y = 650.0;
	glyph.color = vec3(1.0f, 1.0f, 1.0f);
	LoadFonts("C:/Windows/Fonts/Inkfree.ttf", 80);
	INIT_VAO_Text();



	Projection = ortho(0.0f, float(width), 0.0f, float(height));
	resolution = vec2(float(height), float(width));

	MatProj = glGetUniformLocation(programId, "Projection");
	MatModel = glGetUniformLocation(programId, "Model");

	MatProjS = glGetUniformLocation(programIdS, "Projection");
	MatModelS = glGetUniformLocation(programIdS, "Model");
	vec_resS = glGetUniformLocation(programIdS, "resolution");
	loc_time = glGetUniformLocation(programIdS, "time");
	//loc_speed = glGetUniformLocation(programIdS, "speed");

	MatProjText = glGetUniformLocation(programId_text, "Projection_text");
	text_color = glGetUniformLocation(programId_text, "textColor");
}