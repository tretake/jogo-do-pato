#pragma once

#include "Textura.h"
#include "Cenario.h"




enum estados_enum
{
	EM_PE, CORRENDO, AGACHADO, PULANDO, CAINDO, PLANANDO, DASH  
	,POGO ,POGO_ATAQUE , ATACANDO, ATAQUE ,ATACANDO2 ,ATAQUE2 , MINA
	,SLIDE ,DANO , MORTO , BALA,BALA_POGO, TP,
	END
};

enum tipos_enum
{
	JOGADOR,SER,CHAPEUZINHO,COELHO,POGO_PLANTA
};

enum direcao
{
		ESQUERDA,DIREITA,CIMA,BAIXO,NEUTRO,NEUTRO_BAIXO
};

extern Textura assets[END];
extern Textura sprite_pato[END];
extern Textura sprite_chapeuzinho[END];
extern Textura sprite_coelho[END];

class Entidade
{
public:
	SDL_Event e;
	bool rodando = true;
	Cenario *E_mapa;

	const Uint8* teclado;
	Uint8 teclado_ultimo_frame[SDL_NUM_SCANCODES];

	Textura* sprite_sheet[END];
	double rotacao = 0;
	
	SDL_FRect hitbox;

	SDL_FRect ultima_pos;

	vector2df dimesao_em_pe;
	vector2df dimesao_agachado ;

	Entidade(SDL_FRect p_hitbox , Textura* p_sheet , Cenario* p_map , int p_tipo = SER )
	{

		tipo = p_tipo;

		dimesao_em_pe.x = p_hitbox.w;
		dimesao_em_pe.y = p_hitbox.h;

		dimesao_agachado.x = p_hitbox.w;
		dimesao_agachado.y = p_hitbox.h/2;
		hitbox = p_hitbox;
		set_sprite_sheet(p_sheet);
		E_mapa = p_map;
		
		hp = hp_max;
		
	}
	

	int hp_max = 3 ;
	int hp;
	int hp_frame_anterior = hp;
	
	

	int tipo;
	int estado = CAINDO;
	bool no_chao = true;

	int frames_dash = 0;
	int dash_cooldown = 0;

	bool pogoavel = true;
	int frames_pogo = 0;
	bool pogo_hit = false;	//precisa mesmo?
	int pogo_cooldown = 0;

	int frames_ataque = 0;
	bool ataque_combo = false;
	int ataque_cooldown = 0;

	int tiro_cooldown = 0;

	int frames_planar = 0;
	int planar_cooldown = 0;

	bool usou_dash_no_ar = false;
	bool agachado_ultimo_frame = false;
	bool olhando_direita = true;

	int frames_invenc = 0;
	int boss_padrao_cooldown = 0;
	int padrao_npc = 0;
	int ultimo_padrao_npc = 0;

	float velocidade_x = 0;
	float velocidade_y = 0;

	float modulo_x = 9.0f;
	float modulo_y = 5.5f;

	float gravidade = +1.f;

	void reset_estado();

	colisao_detalhe mover_x();
	colisao_detalhe mover_y();
	colisao_detalhe mover();

	void desenhar();

	void get_teclado_ultimo_frame();
	void imput_sistema();
	bool butao_precionado(int scancode);
	bool butao_solto(int scancode);
	void imput();

	void update_cooldowns();
	
	void pulo( double multiplicador_vertical , bool pulo_stop = false);
	void planar(int total_frames , int modulo_cooldown);

	void ataque(int total_frames, int modulo_cooldown);
	void pogo_ataque(int total_frames, float multiplicador_velocidade, int modulo_cooldown);
	void dash(int total_frames, int multiplicador_velocidade, int modulo_cooldown,  bool slide = false);
	void atirar(int cooldown, double velocidade ,float angulo = 0 , bool pogo = false);
	void spaw_mina();
	void spaw_pogo_plant(float p_x, float p_y);

	bool tomou_dano(int direcao , int dano);

	
	void inteligencia(Entidade alvo);

	void set_sprite_sheet(Textura* sprites);
	
	bool interagir();
	
	
	static std::vector<Entidade> Seres;
};

