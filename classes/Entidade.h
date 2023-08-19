#pragma once

#include "Textura.h"
#include "Cenario.h"

extern SDL_FRect sistema_camera;


enum estados_enum
{
	EM_PE, CORRENDO, AGACHADO, PULANDO, CAINDO, PLANANDO, DASH  , POGO ,POGO_ATAQUE ,SLIDE , BALA
};

extern Textura sprites[11];

class Entidade
{
public:
	SDL_Event e;
	bool rodando = true;
	Cenario *E_mapa;

		
	
	Entidade()
	{
		
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
	float modulo_y = 5.5f;

	float gravidade = +1.f;

	void reset_estado();

	void mover(Cenario& p_map);

	void desenhar(SDL_FRect* p_camera);

	void imput_sistema(Cenario* mapa , SDL_FRect camera);
		
	void imput();
	
	void pogo_ataque( Cenario* mapa , bool ativar = false);
	void dash(int total_frames, int multiplicador_velocidade, int modulo_cooldown, bool ativa = false, bool slide = false);

	void atirar();
	
	
	static std::vector<Entidade> Seres;
};

