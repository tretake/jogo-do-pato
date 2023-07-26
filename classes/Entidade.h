#pragma once

#include "Textura.h"
#include "Cenario.h"

class Entidade
{
public:
	SDL_Event e;
	bool rodando = true;

	

	Textura sprite;	
	SDL_Rect crop;
	
	SDL_FRect hitbox;

	SDL_FRect ultima_pos;

	bool no_chao = true;
	bool planando = false;
	bool dashing = false;

	float velocidade_x = 0;
	float velocidade_y = 0;

	float modulo_x = 7.0f;
	float modulo_y = 5.0f;

	float gravidade = +1.f;

	void mover(Cenario& p_map);

	void desenhar();

	void imput_sistema();

	void imput();
	

	void dash();

	
};

