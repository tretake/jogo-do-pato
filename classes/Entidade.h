#pragma once

#include "Textura.h"
#include "Cenario.h"

enum estados_enum
{
	EM_PE, CORRENDO, AGACHADO, PULANDO, CAINDO, PLANANDO, DASH  , POGO 
};

class Entidade
{
public:
	SDL_Event e;
	bool rodando = true;

	

	Textura sprite;
	Textura sprites[8];
	
	
	Entidade()
	{
		sprites[EM_PE].carregar_textura("art/Protagonista/EM_PE.png");
		sprites[CORRENDO].carregar_textura("art/Protagonista/CORRENDO.png");
		sprites[AGACHADO].carregar_textura("art/Protagonista/AGACHADO.png");
		sprites[PULANDO].carregar_textura("art/Protagonista/PULANDO.png");
		sprites[CAINDO].carregar_textura("art/Protagonista/CAINDO.png");
		sprites[PLANANDO].carregar_textura("art/Protagonista/PLANANDO.png");
		sprites[DASH].carregar_textura("art/Protagonista/DASH.png");
		sprites[POGO].carregar_textura("art/Protagonista/POGO.png");

		std::cout << "entidade nasceu\n";

	}
	SDL_Rect crop;
	
	SDL_FRect hitbox;

	SDL_FRect ultima_pos;

	int estado = CAINDO;
	bool no_chao = true;
	int dash_cooldown = 0;
	bool olhando_direita = true;


	float velocidade_x = 0;
	float velocidade_y = 0;

	float modulo_x = 9.0f;
	float modulo_y = 5.0f;

	float gravidade = +1.f;

	void reset_estado();

	void mover(Cenario& p_map);

	void desenhar(SDL_FRect* p_camera);
	void desenhar_hitbox(SDL_Renderer* p_render , SDL_FRect p_camera);

	void imput_sistema();
		
	void imput();
	

	void dash();

	void pogo();
	
};

