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
	SDL_Rect tile_colidida = {0,0,0,0};






	ultima_pos.x = sprite.alvo.x;
	sprite.alvo.x += velocidade_x;
	
	dash();
 

	tile_colidida = p_map.colisao_cenario(sprite.alvo);
	if (tile_colidida.w != 0) 
	{
		sprite.alvo.x = ultima_pos.x;
		velocidade_x = 0;
	}

	ultima_pos.y = sprite.alvo.y;

	if(no_chao == false)
		velocidade_y += gravidade;

	sprite.alvo.y += velocidade_y;


	tile_colidida = p_map.colisao_cenario(sprite.alvo);

	if (tile_colidida.w != 0 )
	{
		if (velocidade_y > 0)
		{
			sprite.alvo.y = tile_colidida.y - sprite.alvo.h;
			no_chao = true;
		}else
			sprite.alvo.y = ultima_pos.y;


		velocidade_y = 0;

	}
	else
		no_chao = false;
}

void Entidade::desenhar()
{
	static bool flip = false;
	static  int frames = 0;

	if (velocidade_x > 0)
		flip = false;
	else if (velocidade_x < 0)
		flip = true;
	
	sprite.alvo.w = 170.f;
	sprite.alvo.h = 190.f;

	if (no_chao == true && velocidade_x == 0)
		crop = { 0,0,381,500 };		//em pé
	else if (no_chao == true && velocidade_x != 0)
	{
		crop = { 447,0,469,500 };
	}
	else if (no_chao == false && velocidade_y < 0)
		crop = { 1607,0,340,500 };		//pulando
	else if (no_chao == false && velocidade_y > 0)
		crop = {2111,0,406,500};		//caindo

	if (planando == true && dashing == false)
	{
		sprite.alvo.w = 170.f;
		sprite.alvo.h = 190.f;
	
		crop = { 3197 , 0 ,650 ,500 };
	}
	
	if(dashing == true)
	{
		crop = {2688,0,378,500};
	}
		

	sprite.desenhar(&crop,NULL, flip);
	

}