#pragma once

#include "Textura.h"
#include "Cenario.h"

extern SDL_FRect sistema_camera;

enum estados_enum
{
	EM_PE, CORRENDO, AGACHADO, PULANDO, CAINDO, PLANANDO, DASH  , POGO ,POGO_ATAQUE
};

class Entidade
{
public:
	SDL_Event e;
	bool rodando = true;

	

	Textura sprite;
	Textura sprites[9];
	
	
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
		sprites[POGO_ATAQUE].carregar_textura("art/Protagonista/POGO_ATAQUE.png");

		std::cout << "entidade nasceu\n";

	}
	SDL_Rect crop;
	
	SDL_FRect hitbox;

	SDL_FRect ultima_pos;

	int estado = CAINDO;
	bool no_chao = true;
	int dash_cooldown = 0;
	int pogo_cooldown = 0;
	bool usou_dash_no_ar = false;
	bool planou_duranto_pulo = false;
	bool olhando_direita = true;


	float velocidade_x = 0;
	float velocidade_y = 0;

	float modulo_x = 9.0f;
	float modulo_y = 5.0f;

	float gravidade = +1.f;

	void reset_estado();

	void mover(Cenario& p_map);

	void desenhar(SDL_FRect* p_camera);

	void imput_sistema(Cenario* mapa , SDL_FRect camera);
		
	void imput();
	
	void pogo_ataque( Cenario* mapa , bool ativar = false);
	void dash(int total_frames, int multiplicador_velocidade, int modulo_cooldown);
	
};

