#pragma once
#include "strutture.h"
extern float Tens, Bias, Cont;
int pval = 200;
bool firstInsert = true;
const vec3 NULL_CENTER = vec3(0.0,0.0,0.0);

#define PHI0(t)  (2.0*t*t*t-3.0*t*t+1)
#define PHI1(t)  (t*t*t-2.0*t*t+t)
#define PSI0(t)  (-2.0*t*t*t+3.0*t*t)
#define PSI1(t)  (t*t*t-t*t)


float dx(int i, float* t, float Tens, float Bias, float Cont, Curva* curva)
{
	if (i == 0)
		return  0.5 * (1.0 - Tens) * (1.0 - Bias) * (1.0 - Cont) * (curva->CP[i + 1].x - curva->CP[i].x) / (t[i + 1] - t[i]);
	if (i == curva->CP.size() - 1)
		return  0.5 * (1.0 - Tens) * (1.0 - Bias) * (1.0 - Cont) * (curva->CP[i].x - curva->CP[i - 1].x) / (t[i] - t[i - 1]);

	if (i % 2 == 0)
		return  0.5 * (1.0 - Tens) * (1.0 + Bias) * (1.0 + Cont) * (curva->CP.at(i).x - curva->CP.at(i - 1).x) / (t[i] - t[i - 1]) + 0.5 * (1 - Tens) * (1 - Bias) * (1 - Cont) * (curva->CP.at(i + 1).x - curva->CP.at(i).x) / (t[i + 1] - t[i]);
	else
		return  0.5 * (1.0 - Tens) * (1.0 + Bias) * (1.0 - Cont) * (curva->CP.at(i).x - curva->CP.at(i - 1).x) / (t[i] - t[i - 1]) + 0.5 * (1 - Tens) * (1 - Bias) * (1 + Cont) * (curva->CP.at(i + 1).x - curva->CP.at(i).x) / (t[i + 1] - t[i]);
}
float dy(int i, float* t, float Tens, float Bias, float Cont, Curva* curva)
{
	if (i == 0)
		return 0.5 * (1.0 - Tens) * (1.0 - Bias) * (1.0 - Cont) * (curva->CP.at(i + 1).y - curva->CP.at(i).y) / (t[i + 1] - t[i]);
	if (i == curva->CP.size() - 1)
		return  0.5 * (1.0 - Tens) * (1.0 - Bias) * (1.0 - Cont) * (curva->CP.at(i).y - curva->CP.at(i - 1).y) / (t[i] - t[i - 1]);

	if (i % 2 == 0)
		return  0.5 * (1.0 - Tens) * (1.0 + Bias) * (1.0 + Cont) * (curva->CP.at(i).y - curva->CP.at(i - 1).y) / (t[i] - t[i - 1]) + 0.5 * (1 - Tens) * (1 - Bias) * (1 - Cont) * (curva->CP.at(i + 1).y - curva->CP.at(i).y) / (t[i + 1] - t[i]);
	else
		return  0.5 * (1.0 - Tens) * (1.0 + Bias) * (1.0 - Cont) * (curva->CP.at(i).y - curva->CP.at(i - 1).y) / (t[i] - t[i - 1]) + 0.5 * (1 - Tens) * (1 - Bias) * (1 + Cont) * (curva->CP.at(i + 1).y - curva->CP.at(i).y) / (t[i + 1] - t[i]);
}

float DX(int i, float* t, Curva* curva)
{
	if (curva->Derivata.at(i).x == 0)
		return dx(i, t, 0.0, 0.0, 0.0, curva);

	if (curva->Derivata.at(i).x != 0)
		return curva->Derivata.at(i).x;

}

float DY(int i, float* t, Curva* curva)
{
	if (curva->Derivata.at(i).y == 0)
		return dy(i, t, 0.0, 0.0, 0.0, curva);

	if (curva->Derivata.at(i).y != 0)
		return curva->Derivata.at(i).y;

}

void InterpolazioneHermite(float* t, Curva* curva, vec3 centro, vec4 color_top, vec4 color_bot)
{
	float p_t = 0, p_b = 0, p_c = 0, x, y;
	float passotg = 1.0 / (float)(pval - 1);
	int j;

	curva->vertices.push_back(centro);
	curva->colors.push_back(color_bot);
	
	float tg = 0, tgmapp, ampiezza;
	int i = 0;
	int is = 0; 

	for (tg = 0; tg <= 1; tg += passotg)
	{
		if (tg > t[is + 1]) is++;

		ampiezza = (t[is + 1] - t[is]);
		tgmapp = (tg - t[is]) / ampiezza;

		x = curva->CP[is].x * PHI0(tgmapp) + DX(is, t, curva) * PHI1(tgmapp) * ampiezza + curva->CP[is + 1].x * PSI0(tgmapp) + DX(is + 1, t, curva) * PSI1(tgmapp) * ampiezza;
		y = curva->CP[is].y * PHI0(tgmapp) + DY(is, t, curva) * PHI1(tgmapp) * ampiezza + curva->CP[is + 1].y * PSI0(tgmapp) + DY(is + 1, t, curva) * PSI1(tgmapp) * ampiezza;

		curva->vertices.push_back(vec3(x, y, 0.0));
		curva->colors.push_back(color_top);
		curva->nv = curva->vertices.size();

	}
	
	curva->vertices[curva->vertices.size() - 1] = curva->vertices[1];
	
}

void CostruisciHermite(float* t, Curva* curva, vec3 centro, vec4 color_top)
{
	curva->vertices.clear();
	curva->colors.clear();
	curva->tg.clear();
	
	vec4 color_bot = vec4(color_top.r*0.4, color_top.g*0.4, color_top.b*0.4, 1.0); 

	InterpolazioneHermite(t, curva, centro, color_top, color_bot);
	curva->vertices.push_back(centro);
}

