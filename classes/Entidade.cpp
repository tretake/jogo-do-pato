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



void Entidade::desenhar(SDL_FRect* p_camera)
{
	static bool flip = false;
	SDL_FRect alvo ;
	alvo.w = 170.f;
	alvo.h = 190.f;
	alvo.x = hitbox.x + hitbox.w / 2 - alvo.w / 2;
	alvo.y = hitbox.y + hitbox.h - alvo.h;

	
	if (no_chao == true)	
	{

		

		if (velocidade_x == 0)
		{
			crop = { 0,0,381,500 };	//em pé
			if (agachado == true)
			{
				crop = { 1057 , 0 , 390 ,500 };
			}
		}
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
		alvo.w = 170.f;
		alvo.h = 190.f;
	}


		sprite.desenhar(&alvo,p_camera,&crop, !olhando_direita);
	

}

void Entidade::desenhar_hitbox(SDL_Renderer* p_render,SDL_FRect p_camera)
{
	Textura t;

	SDL_FRect resolucao_convert = { (hitbox.x - p_camera.x) * t.const_conversao_x, (hitbox.y - p_camera.y) * t.const_conversao_y,(hitbox.w) * t.const_conversao_x, (hitbox.h) * t.const_conversao_y };
	SDL_SetRenderDrawColor(p_render, 0xFF, 0x00, 0x00, 0xFF);
	SDL_RenderDrawRectF(p_render, &resolucao_convert);
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
	static bool key_frame_anterior = false;

	const Uint8* teclado = NULL;
	teclado = SDL_GetKeyboardState(NULL);

	velocidade_x = 0;
	if (teclado[SDL_SCANCODE_D])
	{
		if(agachado == false)
		velocidade_x = modulo_x;
		olhando_direita = true;
	}
	if (teclado[SDL_SCANCODE_A])
	{
		if (agachado == false)
		velocidade_x = -modulo_x;
		olhando_direita = false;
	}
	if (teclado[SDL_SCANCODE_S])
	{
		if (no_chao == true)
			agachado = true;
		else
			olhando_baixo = true;
	}	
	else
	{
		agachado = false;
		olhando_baixo = false;
	}


	if (teclado[SDL_SCANCODE_M])
	{
		if(velocidade_x != 0)
			dashing = true;
	}
	


	if ((teclado[SDL_SCANCODE_SPACE] || teclado[SDL_SCANCODE_W]) && !key_frame_anterior)	//butao pressionado
	{
		if (no_chao)
			velocidade_y = -5 * modulo_y;	
	}
	else if (!(teclado[SDL_SCANCODE_SPACE] || teclado[SDL_SCANCODE_W]) && key_frame_anterior)
	{
		if (velocidade_y < -6.0f)
			velocidade_y = -6.0f;

	}


	key_frame_anterior = false;
	if (no_chao)
	{
		planando = false;
	}
	if ((teclado[SDL_SCANCODE_SPACE] || teclado[SDL_SCANCODE_W]))
	{
		key_frame_anterior = true;
		

		if (velocidade_y > 0 && !no_chao)
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



