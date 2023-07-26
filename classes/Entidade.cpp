#include "Entidade.h"


void Entidade::imput_sistema()
{
	while (SDL_PollEvent(&e))
	{
		switch (e.type)
		{
		case SDL_WINDOWEVENT:
			if (e.window.event == SDL_WINDOWEVENT_RESIZED)
				Textura::update_const_conversao();

			break;
		case SDL_MOUSEBUTTONDOWN:

			break;

		case SDL_KEYDOWN:
				switch (e.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					rodando = false;
					break;
				}
			break;
		case SDL_QUIT:
			rodando = false;
			break;
		}
	}
}

void Entidade::mover(Cenario& p_map)
{
	colisao_detalhe colisao_status = {FORA,0,0};


	ultima_pos.x = hitbox.x;
	hitbox.x += velocidade_x;
	
	dash();
 

	colisao_status = p_map.colisao_cenario(hitbox);

	if (colisao_status.caso == DENTRO )
	{
		hitbox.x = ultima_pos.x;
		velocidade_x = 0;
	}



	ultima_pos.y = hitbox.y;

	if(no_chao == false)
		velocidade_y += gravidade;

	hitbox.y += velocidade_y;


	colisao_status = p_map.colisao_cenario(hitbox); 


	if (colisao_status.caso == DENTRO || colisao_status.caso == ENCOSTANDO)
	{

 
		if (velocidade_y > 0 )
		{
			hitbox.y = colisao_status.y - hitbox.h;
			no_chao = true;
		}else
			hitbox.y = ultima_pos.y;


		velocidade_y = 0;

	}
	else
		no_chao = false;



}

void Entidade::desenhar()
{
	static bool flip = false;
	SDL_FRect alvo = hitbox;


	if (velocidade_x > 0)
		flip = false;
	else if (velocidade_x < 0)
		flip = true;
	

	
	if (no_chao == true)	
	{
		if(velocidade_x == 0)
			crop = { 0,0,381,500 };	//em pé
		else
			crop = { 447,0,469,500 };//correndo
	}
	else
	{
		if(velocidade_y < 0)
			crop = { 1607,0,340,500 };//pulando
		else
			crop = { 2111,0,406,500 };//caindo
		if (planando == true)
		{
			alvo.x = hitbox.x - 30;
			alvo.w = 250.f;
			alvo.h = 200.f;
			crop = { 3197 , 0 ,650 ,500 };
		}
	}

	if(dashing == true)
	{
		crop = {2688,0,378,500};
	}

	sprite.desenhar(&alvo,&crop, flip);
	

}

void Entidade::dash()
{
	int total_frames = 11;
	int multiplicador_velocidade = 4;
	int modulo_cooldown = 30;

	static bool usou_dash_no_ar = false;
	static int frames = total_frames;
	static int cooldown = 0;

	if (cooldown != 0)
	{
		cooldown--;
		if( no_chao == false && cooldown == 0 && usou_dash_no_ar == true)
			cooldown = 1;

		dashing = false;
	}


	if (dashing == true && cooldown == 0)
	{
		if (frames == total_frames)
		{
			modulo_x *= multiplicador_velocidade;
			if (no_chao == false)
				usou_dash_no_ar = true;
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
	if (no_chao == true)
		usou_dash_no_ar = false;


}

void Entidade::imput()
{
	const Uint8* teclado = NULL;
	teclado = SDL_GetKeyboardState(NULL);

	velocidade_x = 0;
	if (teclado[SDL_SCANCODE_D])
	{
		velocidade_x = modulo_x;
	}
	if (teclado[SDL_SCANCODE_A])
	{
		velocidade_x = -modulo_x;
	}

	if (teclado[SDL_SCANCODE_M])
	{
		if(velocidade_x != 0)
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
			velocidade_y -= gravidade * 0.95f;
		}
	}
	else { planando = false; }


}