#include "Entidade.h"

SDL_FRect sistema_camera = { 0,0,1600,900 };	

std::vector<Entidade> Entidade::Seres;
Textura sprite_pato[END];
Textura sprite_megaman[END];



void Entidade::imput_sistema(SDL_FRect camera)
{
	while (SDL_PollEvent(&e))
	{

	switch (e.type)
		{
		case SDL_WINDOWEVENT:
			if (e.window.event == SDL_WINDOWEVENT_RESIZED)
				update_const_conversao();

			break;
		case SDL_MOUSEBUTTONDOWN:
			break;

		case SDL_KEYDOWN:
				switch (e.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					rodando = false;
					break;
				case SDLK_LSHIFT:
					E_mapa->mudar_tile(camera, hitbox);
					break;
				case SDLK_LCTRL:
					E_mapa->salvar_mapa();
					rodando = false;
					break;
				case SDLK_r:
					hitbox = { 200.f,200.f,130.f,140.f };
					break;


				}
			break;
		case SDL_QUIT:
			rodando = false;
			break;
		}
	}
}


void Entidade::mover_x()
{
	colisao_detalhe colisao_status = { FORA,0,0 };

	dash(11, 4, 30);

	ultima_pos.x = hitbox.x;
	hitbox.x += velocidade_x;

	colisao_status = E_mapa->colisao_cenario(hitbox);

	if (colisao_status.caso == DENTRO)
	{
		hitbox.x = ultima_pos.x;
		velocidade_x = 0;
		if (estado == SLIDE)
			estado = AGACHADO;

	}

	if (velocidade_x != 0) {
		if (velocidade_x > 0)
		{
			olhando_direita = true;
		}
		else
			olhando_direita = false;
	}
}
void Entidade::mover_y()
{
	colisao_detalhe colisao_status = { FORA,0,0 };


	ultima_pos.y = hitbox.y;


	pogo_ataque(E_mapa);

	if (estado == PLANANDO)
		velocidade_y -= gravidade * 0.93f;
	if (no_chao == false && estado != DASH)
		velocidade_y += gravidade;

	hitbox.y += velocidade_y;

	colisao_status = E_mapa->colisao_cenario(hitbox);

	if (colisao_status.caso == DENTRO || colisao_status.caso == ENCOSTANDO)
	{
		if (velocidade_y > 0)
		{
			hitbox.y = colisao_status.y - hitbox.h;
			no_chao = true;
		}
		else
			hitbox.y = ultima_pos.y;

		velocidade_y = 0;
	}
	else
		no_chao = false;
}
void Entidade::mover()
{
	mover_x();
	mover_y();

	if (no_chao == true)
		planou_duranto_pulo = false;


}



void Entidade::desenhar(SDL_FRect* p_camera , Textura* sprites)
{

		SDL_FRect alvo;
		alvo.w = 180.f;
		alvo.h = 180.f;
		alvo.x = hitbox.x + hitbox.w / 2 - alvo.w / 2;
		alvo.y = hitbox.y + hitbox.h - alvo.h + 15;


		if (estado == PLANANDO)
		{
			alvo.x = alvo.x - alvo.w / 2;
			alvo.w = 360.f;
			alvo.h = 180.f;
		}



		if (estado == POGO_ATAQUE)
		{
			sprites[POGO].desenhar(&alvo, p_camera, NULL, !olhando_direita);


			SDL_FRect pogo_hitbox = { hitbox.x - 50 ,hitbox.y + hitbox.h + 50 , hitbox.w + 50 , hitbox.h };
			sprites[estado].desenhar(&pogo_hitbox, p_camera, NULL, !olhando_direita);
		}
		else
			sprites[estado].desenhar(&alvo, p_camera, NULL, !olhando_direita);


	
}


void Entidade::pulo(bool pulo_stop)
{

	if (no_chao)
		velocidade_y = -5 * modulo_y;
	else if (pulo_stop)
	{
		if (velocidade_y < -6.0f)
			velocidade_y = -6.0f;
	}
}

void Entidade::planar()
{
	if (velocidade_y > 0 && !no_chao)
	{
		estado = PLANANDO;

		if (planou_duranto_pulo == false)
		{
			if (velocidade_y > 0)
				velocidade_y = 0;
			planou_duranto_pulo = true;
		}

	}
}

void Entidade::dash(int total_frames , int multiplicador_velocidade , int modulo_cooldown , bool ativar , bool slide)	
{

	if (ativar && dash_cooldown == 0 && usou_dash_no_ar == false)	//dash iniciado
	{

		frames_dash = total_frames;
	
		if (no_chao == false)
			usou_dash_no_ar = true;

		if (slide == true)
			estado = SLIDE;
		
		
	}

	if (frames_dash != 0)	//dash ativo
	{

		if (estado == DASH)
			velocidade_y = 0;
		

		if (olhando_direita == true)
			velocidade_x = modulo_x * multiplicador_velocidade;
		else
			velocidade_x = -modulo_x * multiplicador_velocidade;

		if (estado == SLIDE)
			estado = SLIDE;
		else
			estado = DASH;


		frames_dash--;

		if (frames_dash == 0) {		//dash fim
			dash_cooldown = modulo_cooldown;

			if (estado == SLIDE)
			{


				colisao_detalhe colisao;
				SDL_FRect imagem_jogador_em_pe = hitbox;
				imagem_jogador_em_pe.y -= ( dimesao_em_pe.y - dimesao_agachado.y);
				imagem_jogador_em_pe.h = dimesao_em_pe.y;

				colisao = E_mapa->colisao_cenario(imagem_jogador_em_pe);
				if (colisao.caso == DENTRO)
					frames_dash = 1;
				else
				{
					estado = AGACHADO;
					velocidade_x /= multiplicador_velocidade;
				}


			}
			else
			{
				reset_estado();
				velocidade_x /= multiplicador_velocidade;
			}
		}
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


void Entidade::pogo_ataque(Cenario* mapa , bool ativar)
{
	static int frames_ativo = 0;
	static bool acertou = false;

	if (ativar && pogo_cooldown == 0)
	{
		frames_ativo = 5;
		acertou = false;
	}

	if (frames_ativo != 0 && mapa != NULL)
	{
		SDL_FRect pogo_hitbox = { hitbox.x ,hitbox.y + hitbox.h + 50 , hitbox.w , hitbox.h };

		if (acertou == false && mapa->colisao_cenario(pogo_hitbox).caso == DENTRO)
		{
			velocidade_y = -4.5f * modulo_y;
			usou_dash_no_ar = false;
			planou_duranto_pulo = false;
			acertou = true;
		}
		frames_ativo--;
		estado = POGO_ATAQUE;
		if (frames_ativo == 0)
			pogo_cooldown = 25;
	}
	if (pogo_cooldown != 0)
		pogo_cooldown--;

}


//uma bagun�a
void Entidade::imput()
{

	 const Uint8* teclado = SDL_GetKeyboardState(NULL);

	
	if (estado != DASH && estado != SLIDE)
		reset_estado();

	velocidade_x = 0;

	
	if (estado != DASH && estado != SLIDE)
	{

		static bool s_frame_anterior = false;
		
		if (teclado[SDL_SCANCODE_K])
			atirar();


		s_frame_anterior = false;
		if (teclado[SDL_SCANCODE_S])
		{
			if (no_chao == true)
				estado = AGACHADO;
			else
				estado = POGO;
			s_frame_anterior = true;
		}

		static bool pogo_ataque_frame_anterior = false;
		if (estado == POGO && teclado[SDL_SCANCODE_N] && !pogo_ataque_frame_anterior)
			pogo_ataque(NULL, true);
		
		pogo_ataque_frame_anterior = false;
		if (teclado[SDL_SCANCODE_N])
			pogo_ataque_frame_anterior = true;



		if (teclado[SDL_SCANCODE_D])
		{
			if (estado != AGACHADO)
				velocidade_x = modulo_x;
		}
		if (teclado[SDL_SCANCODE_A])
		{
			if (estado != AGACHADO)
				velocidade_x = -modulo_x;
		}




		static bool espaco_frame_anterior = false;

		if ((teclado[SDL_SCANCODE_SPACE] || teclado[SDL_SCANCODE_W]) && !espaco_frame_anterior)	//evento butao pressionado
			pulo();
		else if (!(teclado[SDL_SCANCODE_SPACE] || teclado[SDL_SCANCODE_W]) && espaco_frame_anterior)	//evento butao solto
			pulo(true);


		espaco_frame_anterior = false;
		if ((teclado[SDL_SCANCODE_SPACE] || teclado[SDL_SCANCODE_W]))
		{
			espaco_frame_anterior = true;
			planar();

		}
	}


	static bool dash_frame_anterior = false;
	if (teclado[SDL_SCANCODE_M] && !dash_frame_anterior && dash_cooldown == 0)
	{
		if (estado == AGACHADO)
			dash(11, 4, 30, true, true);
		else
			dash(11, 4, 30, true);
	}
	dash_frame_anterior = false;
	if (teclado[SDL_SCANCODE_M])
		dash_frame_anterior = true;
	
} 


void Entidade::inteligencia(Entidade alvo)
{

	if (alvo.hitbox.x > hitbox.x)
	{
		velocidade_x = 2;
	}
	else
		velocidade_x = -2;


}

//quarentena
void Entidade::atirar()
{
	Entidade bala;
	bala.estado = BALA;
	bala.E_mapa = E_mapa;
	 
	bala.olhando_direita = olhando_direita;


	if (olhando_direita)
	{
		bala.hitbox = {hitbox.x + hitbox.w , hitbox.y + hitbox.h/2 , 70 , 70};
		bala.velocidade_x = 10;
	}
	else
	{
		bala.hitbox = { hitbox.x - 70 , hitbox.y + hitbox.h / 2 , 70 , 70 };
		bala.velocidade_x = -10;
	}

	Seres.push_back(bala);
}
//quarentena





void Entidade::reset_estado()
{

	if (no_chao == true)
	{
		bool sob_tile = false;

	

		if (agachado_ultimo_frame)
		{
			colisao_detalhe colisao;
			SDL_FRect imagem_jogador_em_pe = hitbox;
			imagem_jogador_em_pe.y -= (dimesao_em_pe.y - dimesao_agachado.y) ;
			imagem_jogador_em_pe.h = dimesao_em_pe.y;

			colisao = E_mapa->colisao_cenario(imagem_jogador_em_pe);
			if (colisao.caso == DENTRO)
			{
				estado = AGACHADO;
				sob_tile = true;
			}
			else if (estado != AGACHADO && estado != SLIDE)
			{
				hitbox.h = dimesao_em_pe.y;
				hitbox.y -= (dimesao_em_pe.y - dimesao_agachado.y);
			}
		}
		
		agachado_ultimo_frame = false;
		if (estado == AGACHADO || estado == SLIDE) {

			agachado_ultimo_frame = true;
			hitbox.h = dimesao_agachado.y;
			if(estado == AGACHADO)
				velocidade_y += 70;

		}

		if (velocidade_x != 0)
			estado = CORRENDO;
		else if (sob_tile == false)
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


