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


	void imput()
	{
		const Uint8* teclado = NULL;
		teclado = SDL_GetKeyboardState(NULL);

		velocidade_x = 0;
		if(teclado[SDL_SCANCODE_D])
		{
			velocidade_x = modulo_x;
		}
		if (teclado[SDL_SCANCODE_A])
		{
			velocidade_x = -modulo_x;
		}

		if(teclado[SDL_SCANCODE_M])
		{
			dashing = true;
		}

		if (no_chao)
			planando = false;
		if (teclado[SDL_SCANCODE_SPACE])
		{
			
			if (no_chao)
				velocidade_y = -5 * modulo_y;
	
			else if (velocidade_y > 0)
			{
				if (planando == false)
				{
					velocidade_y = 0;
					planando = true;
				}
				velocidade_y -= gravidade*0.95;
			}
		}
		else { planando = false; }


	}


	void dash()
	{
		int total_frames = 11;
		int multiplicador_velocidade = 4;
		static int frames = total_frames;

		int modulo_cooldown = 30;
		static int cooldown = 0;

		if (cooldown != 0)
		{
			cooldown--;
			dashing = false;
		}

		if (dashing == true && cooldown == 0 )
		{
			if (frames == total_frames)
			{
				modulo_x *= multiplicador_velocidade;
			}
				frames--;

		}
		if (frames == 0)
		{
			modulo_x /= multiplicador_velocidade;
			dashing = false;
			frames = total_frames;
			cooldown = modulo_cooldown;
		}

		if (dashing == true)
			velocidade_y = 0;
		

	}
};

