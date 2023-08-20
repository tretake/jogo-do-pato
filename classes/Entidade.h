#pragma once

#include "Textura.h"
#include "Cenario.h"

extern SDL_FRect sistema_camera;


enum estados_enum
{
	EM_PE, CORRENDO, AGACHADO, PULANDO, CAINDO, PLANANDO, DASH  , POGO ,POGO_ATAQUE , ATACANDO, ATAQUE ,ATACANDO2 ,ATAQUE2 ,SLIDE ,DANO, BALA , END
};

extern Textura sprite_pato[END];
extern Textura sprite_megaman[END];


class Entidade
{
public:
	SDL_Event e;
	bool rodando = true;
	Cenario *E_mapa;

	const Uint8* teclado;
	Uint8 teclado_ultimo_frame[SDL_NUM_SCANCODES];

	Textura* sprite_sheet[END];
	

	Entidade()
	{
		//teclado_ultimo_frame = SDL_GetKeyboardState(NULL);
	}
	

	
	SDL_FRect hitbox;

	SDL_FRect ultima_pos;

	vector2df dimesao_em_pe = { 130.f,140.f };
	vector2df dimesao_agachado = { 130.f,70.f };

	int estado = CAINDO;
	bool no_chao = true;

	int frames_dash = 0;
	int dash_cooldown = 0;

	int frames_pogo = 0;
	bool pogo_hit = false;
	int pogo_cooldown = 0;

	int frames_ataque = 0;
	bool ataque_combo = false;
	int ataque_cooldown = 0;

	bool usou_dash_no_ar = false;
	bool planou_duranto_pulo = false;
	bool agachado_ultimo_frame = false;
	bool olhando_direita = true;

	int frames_invenc = 0;


	float velocidade_x = 0;
	float velocidade_y = 0;

	float modulo_x = 9.0f;
	float modulo_y = 5.5f;

	float gravidade = +1.f;

	void reset_estado();

	void mover_x();
	void mover_y();
	void mover();

	void desenhar(SDL_FRect* p_camera, Textura* sprites);

	void get_teclado_ultimo_frame();
	void imput_sistema(SDL_FRect camera);
	bool butao_precionado(int scancode);
	bool butao_solto(int scancode);
	void imput();
	
	void pulo(bool pulo_stop = false);
	void planar();

	void ataque(int total_frames, int modulo_cooldown, bool ativar = false);
	void pogo_ataque(int total_frames, float multiplicador_velocidade, int modulo_cooldown, bool ativar = false);
	void dash(int total_frames, int multiplicador_velocidade, int modulo_cooldown, bool ativa = false, bool slide = false);

	void tomou_dano();

	void atirar();
	void inteligencia(Entidade alvo);

	void set_sprite_sheet(Textura* sprites);
	
	
	
	static std::vector<Entidade> Seres;
};

