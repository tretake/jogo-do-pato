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



	dash();

	ultima_pos.x = hitbox.x;
	hitbox.x += velocidade_x;

	colisao_status = p_map.colisao_cenario(hitbox);

	if (colisao_status.caso == DENTRO )
	{
		hitbox.x = ultima_pos.x;
		velocidade_x = 0;
	}



	ultima_pos.y = hitbox.y;


	if (estado == PLANANDO)
		velocidade_y -= gravidade * 0.93f;
	if(no_chao == false && estado != DASH)
		velocidade_y += gravidade;

	hitbox.y += velocidade_y;


	colisao_status = p_map.colisao_cenario(hitbox); 


	if (colisao_status.caso == DENTRO || colisao_status.caso == ENCOSTANDO)
	{

 
		if (velocidade_y > 0 )
		{
			hitbox.y = colisao_status.y - hitbox.h;
			estado = EM_PE;
			no_chao = true;
		}else
			hitbox.y = ultima_pos.y;


		velocidade_y = 0;

	}
	else
		no_chao = false;



}

void Entidade::reset_estado()
{
	if (no_chao == true)
	{
		if (velocidade_x != 0)
			estado = CORRENDO;
		else
			estado = EM_PE;
	}
	else
	{
		if (velocidade_y < 0)
			estado = PULANDO;
		else
			estado = CAINDO;
	}
}

void Entidade::desenhar(SDL_FRect* p_camera)
{
	static bool flip = false;
	SDL_FRect alvo ;
	alvo.w = 170.f;
	alvo.h = 190.f;
	alvo.x = hitbox.x + hitbox.w / 2 - alvo.w / 2;
	alvo.y = hitbox.y + hitbox.h - alvo.h;

	
	if (estado == PLANANDO )
		{
			alvo.x = hitbox.x - 30;
			alvo.w = 250.f;
			alvo.h = 200.f;
		}

	sprites[estado].desenhar(&alvo, p_camera, NULL, !olhando_direita);	

}

void Entidade::desenhar_hitbox(SDL_Renderer* p_render,SDL_FRect p_camera)
{
	Textura t;

	SDL_FRect resolucao_convert = { (hitbox.x - p_camera.x) * t.const_conversao_x, (hitbox.y - p_camera.y) * t.const_conversao_y,(hitbox.w) * t.const_conversao_x, (hitbox.h) * t.const_conversao_y };
	SDL_SetRenderDrawColor(p_render, 0xFF, 0x00, 0x00, 0xFF);
	SDL_RenderDrawRectF(p_render, &resolucao_convert);					//esse garotão todo devia ser uma sistem function
}
void Entidade::dash()
{
	int total_frames = 11;
	int multiplicador_velocidade = 4;
	int modulo_cooldown = 30;

	static bool dash_ativado = false;
	static bool usou_dash_no_ar = false;
	static int frames = total_frames;

	if (estado == DASH && dash_cooldown == 0 && dash_ativado == false && usou_dash_no_ar == false)
	{
		dash_ativado = true;
		if (no_chao == false)
			usou_dash_no_ar = true;	
		velocidade_y = 0;
		frames = total_frames;
	}

	if (dash_ativado)
	{
		if (olhando_direita == true)
			velocidade_x = modulo_x * multiplicador_velocidade;
		else
			velocidade_x = -modulo_x * multiplicador_velocidade;

		estado = DASH;
		frames--;

	}

	if (frames == 0)
	{
		frames = total_frames;
		dash_ativado = false;
		dash_cooldown = modulo_cooldown;
		velocidade_x /= multiplicador_velocidade;
		reset_estado();
	}

	if (dash_cooldown != 0)
	{
		dash_cooldown--;
		if (usou_dash_no_ar == true && dash_cooldown == 0)
			dash_cooldown = 1;
	}

	if (no_chao)
		usou_dash_no_ar = false;
	

}

void Entidade::pogo()
{
	//SDL_FRect hitbox_pogo;

}

void Entidade::imput()
{

	const Uint8* teclado = NULL;
	teclado = SDL_GetKeyboardState(NULL);

	
	if (estado != DASH)
		reset_estado();

	velocidade_x = 0;

	
	if (estado != DASH)
	{
		if (teclado[SDL_SCANCODE_S])
		{
			if (no_chao == true)
				estado = AGACHADO;
			else
				estado = POGO;
		}

		if (teclado[SDL_SCANCODE_D])
		{
			if (estado != AGACHADO)
				velocidade_x = modulo_x;
			olhando_direita = true;
		}
		if (teclado[SDL_SCANCODE_A])
		{
			if (estado != AGACHADO)
				velocidade_x = -modulo_x;
			olhando_direita = false;
		}



		static bool espaco_frame_anterior = false;
		static bool planou_duranto_pulo = false;
		if (no_chao == true)
			planou_duranto_pulo = false;
		if ((teclado[SDL_SCANCODE_SPACE] || teclado[SDL_SCANCODE_W]) && !espaco_frame_anterior)	//evento butao pressionado
		{
			if (no_chao)
				velocidade_y = -5 * modulo_y;
			else if (velocidade_y > 0 && planou_duranto_pulo == false)
			{
				velocidade_y = 0;
				planou_duranto_pulo = true;
			}
		}
		else if (!(teclado[SDL_SCANCODE_SPACE] || teclado[SDL_SCANCODE_W]) && espaco_frame_anterior)	//evento butao solto
		{
			if (velocidade_y < -6.0f)
				velocidade_y = -6.0f;
		}

		espaco_frame_anterior = false;
		if ((teclado[SDL_SCANCODE_SPACE] || teclado[SDL_SCANCODE_W]))
		{
			espaco_frame_anterior = true;
			if (velocidade_y > 0 && !no_chao && planou_duranto_pulo)
				estado = PLANANDO;
		}
	}


	static bool dash_frame_anterior = false;

	if (teclado[SDL_SCANCODE_M] && !dash_frame_anterior && dash_cooldown == 0)
		estado = DASH;

	dash_frame_anterior = false;
	if (teclado[SDL_SCANCODE_M])
		dash_frame_anterior = true;
	
}




