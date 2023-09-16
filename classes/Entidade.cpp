#include "Entidade.h"



std::vector<Entidade> Entidade::Seres;
Textura sprite_pato[END];
Textura sprite_megaman[END];



void Entidade::imput_sistema()
{

	get_teclado_ultimo_frame();

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
					E_mapa->mudar_tile(sistema_camera, hitbox);
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


colisao_detalhe Entidade::mover_x()
{
	colisao_detalhe colisao_status = { FORA,0,0 };

	ataque(20, 10);
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

	return colisao_status;
}
colisao_detalhe Entidade::mover_y()
{
	colisao_detalhe colisao_status = { FORA,0,0 };


	ultima_pos.y = hitbox.y;


	pogo_ataque(10, 4.5f, 25);

	if (estado == PLANANDO)
		velocidade_y -= gravidade * 0.93f;
	if (no_chao == false && estado != DASH && estado != BALA)
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


	if (no_chao == true)
		planou_duranto_pulo = false;

	return colisao_status;
}
colisao_detalhe Entidade::mover()
{
	colisao_detalhe colisao_x;
	colisao_detalhe colisao_y;

	colisao_x = mover_x();
	
	colisao_y = mover_y();

	update_cooldowns();

	if (colisao_x.caso == DENTRO)
		return colisao_x;
	else
		return colisao_y;
	
}




void Entidade::desenhar()
{
		SDL_FRect alvo;
		alvo.w = 180.f;
		alvo.h = 180.f;

		if (estado == BALA)
		{
			alvo.w = hitbox.w + 15;
			alvo.h = hitbox.h + 15;

			desenhar_alvo(hitbox, sistema_camera, true);
		}
		
		alvo.x = hitbox.x + hitbox.w / 2 - alvo.w / 2;
		alvo.y = hitbox.y + hitbox.h - alvo.h + 15;

		

		if (estado == PLANANDO)
		{
			alvo.x = alvo.x - alvo.w / 2;
			alvo.w = 360.f;
			alvo.h = 180.f;
		}else if (estado == POGO)
		{
			SDL_FRect pogo_hitbox = { hitbox.x - 50 ,hitbox.y + dimesao_em_pe.y + 40.f *(1.f -( (float)frames_pogo / 10.f )) , hitbox.w + 50 , hitbox.h + 50};
			sprite_sheet[POGO_ATAQUE]->desenhar(&pogo_hitbox, NULL, !olhando_direita);

		}else if (estado == ATACANDO || estado == ATACANDO2)
		{

			SDL_FRect ataque_hitbox;
			if(olhando_direita)
				ataque_hitbox = { hitbox.x + dimesao_em_pe.x + 60.f * (1.f - ((float)frames_ataque / 10.f)), hitbox.y - 37 , hitbox.w + 50 , hitbox.h + 50 };
			else
				ataque_hitbox = { hitbox.x -(hitbox.w + 50) - 60.f * (1.f - ((float)frames_ataque / 10.f)), hitbox.y - 37 , hitbox.w + 50 , hitbox.h + 50 };

			if( estado == ATACANDO)
				sprite_sheet[ATAQUE]->desenhar(&ataque_hitbox,  NULL, !olhando_direita);
			else
				sprite_sheet[ATAQUE2]->desenhar(&ataque_hitbox, NULL, !olhando_direita);

		}
		

		
		
		sprite_sheet[estado]->desenhar(&alvo, NULL, !olhando_direita);


	
}


void Entidade::pulo( double multiplicador_vertical , bool pulo_stop )
{

	if (no_chao)
		velocidade_y = -multiplicador_vertical * modulo_y;
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

void Entidade::pogo_ataque(int total_frames, float multiplicador_velocidade, int modulo_cooldown, bool ativar)
{
	if (ativar && pogo_cooldown == 0)
	{
		frames_pogo = total_frames;
		pogo_hit = false;
	}

	if (frames_pogo != 0 && E_mapa != NULL)
	{

		SDL_FRect pogo_hitbox = { hitbox.x , hitbox.y + dimesao_em_pe.y + 40.f*( 1.f - ( (float)frames_pogo / (float)total_frames))  , dimesao_em_pe.x , dimesao_em_pe.y };


		if (pogo_hit == false && E_mapa->colisao_cenario(pogo_hitbox).caso == DENTRO)
		{
			velocidade_y = -multiplicador_velocidade * modulo_y;
			usou_dash_no_ar = false;
			planou_duranto_pulo = false;
			pogo_hit = true;
		}
		frames_pogo--;
		estado = POGO;
		if (frames_pogo == 0)
			pogo_cooldown = modulo_cooldown;
	}
	if (pogo_cooldown != 0)
		pogo_cooldown--;

}

void Entidade::ataque(int total_frames , int modulo_cooldown, bool ativar)
{
	if (ativar && ataque_cooldown == 0 && ataque_combo != true)
	{
		if (frames_ataque == 0)
			estado = ATACANDO;
		else
		{
			estado = ATACANDO2;
			ataque_combo = true;
		}

		frames_ataque = total_frames;
	}

	if (frames_ataque != 0)
	{
		SDL_FRect ataque_hitbox;
		if (olhando_direita)
			ataque_hitbox = { hitbox.x + dimesao_em_pe.x + 60.f * (1.f - ((float)frames_ataque / 10.f)), hitbox.y - 37 , hitbox.w + 50 , hitbox.h + 50 };
		else
			ataque_hitbox = { hitbox.x - (hitbox.w + 50) - 60.f * (1.f - ((float)frames_ataque / 10.f)), hitbox.y - 37 , hitbox.w + 50 , hitbox.h + 50 };

		for (auto &ser : Seres)
		{
			if(colisao(ser.hitbox, ataque_hitbox))
				ser.tomou_dano();
		}


		frames_ataque--;

		if (ataque_combo)
			estado = ATACANDO2;
		else
			estado = ATACANDO;


		if (frames_ataque == 0)
		{
			ataque_cooldown = modulo_cooldown;
			ataque_combo = false;
		}
	}

	if (ataque_cooldown != 0)
		ataque_cooldown--;
}





void Entidade::imput()
{
	imput_sistema();

	teclado = SDL_GetKeyboardState(NULL);


	if (estado != DASH && estado != SLIDE)
		reset_estado();

	velocidade_x = 0;


	if (estado != DASH && estado != SLIDE)
	{
		
		if (butao_precionado(SDL_SCANCODE_K))
			atirar(5,10);


		if (teclado[SDL_SCANCODE_S])
		{
			if (no_chao == true)
				estado = AGACHADO;
		}


		if (butao_precionado(SDL_SCANCODE_N))
		{
			if (no_chao == false && teclado[SDL_SCANCODE_S])
				pogo_ataque(10, 4.5f, 25, true);
			else
				ataque(20, 10, true);
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




		if (butao_precionado(SDL_SCANCODE_SPACE) || butao_precionado(SDL_SCANCODE_W))	
			pulo(5);
		else if (butao_solto(SDL_SCANCODE_SPACE) || butao_solto(SDL_SCANCODE_W))	
			pulo(5,true);


		if ((teclado[SDL_SCANCODE_SPACE] || teclado[SDL_SCANCODE_W]))
			planar();

		
	}


	if (butao_precionado(SDL_SCANCODE_M) && dash_cooldown == 0)
	{
		if (estado == AGACHADO)
			dash(11, 4, 30, true, true);
		else
			dash(11, 4, 30, true);
	}

	
} 


void Entidade::tomou_dano()
{
	if (frames_invenc == 0)
	{
		if (olhando_direita)
			velocidade_x -= 10.f;
		velocidade_y -= 10.f;

		frames_invenc = 20;
	}
}

void Entidade::inteligencia(Entidade alvo)
{
	if (alvo.hitbox.x > hitbox.x)
	{
		olhando_direita = true;
	}
	else
		olhando_direita = false;


	if(estado != BALA)
	{
		//std::cout << boss_padrao_cooldown << "\n";
		if (no_chao)
			velocidade_x = 0;
		if (no_chao && boss_padrao_cooldown == 0) {
		

			int moeda = rand() % 3;
			
			tiro_cooldown = 0;
			switch (moeda)
			{
			case 0 :	//pulo atirando pra baixo
				pulo(7);
				if (alvo.hitbox.x > hitbox.x)
				{
					velocidade_x = 23;
				}
				else
					velocidade_x = -23;
				boss_padrao_cooldown = 110;

				break;

			case 1 :	//metralhadora
				for (int i = 2; i < 7; i++)
					atirar(0, i * 4);
				
				boss_padrao_cooldown = 100;
				break;

			case 2:	//TIRO PULANDO
				pulo(6);
				boss_padrao_cooldown = 120;
				break;
			}
		
		}
		if (no_chao == false )
		{
			if (velocidade_x == 0)
				atirar(8, 15);
			else
				atirar(12, 18, BAIXO);
		}
		


	}


}

//quarentena
void Entidade::atirar(int cooldown, double velocidade , int direcao )
{
	if (tiro_cooldown == 0)
	{
		Entidade bala({ 0.0f,0.0f,0.0f,0.0f }, *sprite_sheet, E_mapa );
		bala.estado = BALA;
		bala.olhando_direita = olhando_direita;

		float aresta = 45;

		if (direcao == NEUTRO) {
			if (olhando_direita)
			{
				bala.hitbox = { hitbox.x + hitbox.w , hitbox.y + aresta/2  , aresta , aresta };
				bala.velocidade_x = velocidade;
			}
			else
			{
				bala.hitbox = { hitbox.x - aresta , hitbox.y + aresta/2  , aresta , aresta };
				bala.velocidade_x = -velocidade;
			}
		}
		if (direcao == BAIXO)
		{
			bala.hitbox = { hitbox.x + hitbox.w/2 , hitbox.y  , aresta , aresta };
			bala.velocidade_y = velocidade;

		}




		Seres.push_back(bala);

		tiro_cooldown = cooldown;
	}

}
//quarentena



void Entidade::update_cooldowns()
{
	if(tiro_cooldown != 0)
		tiro_cooldown--;


	if(boss_padrao_cooldown != 0)
	{
		boss_padrao_cooldown--;
	}
}











//SOLIDO



void Entidade::reset_estado()	//fazendo muitas coisas
{
	if (estado != BALA)
	{
		if (no_chao == true)
		{
			bool sob_tile = false;



			if (agachado_ultimo_frame)
			{
				colisao_detalhe colisao;
				SDL_FRect imagem_jogador_em_pe = hitbox;
				imagem_jogador_em_pe.y -= (dimesao_em_pe.y - dimesao_agachado.y);
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
				if (estado == AGACHADO)
					velocidade_y += 70;

			}

			if (velocidade_x != 0)
				estado = CORRENDO;
			else if (sob_tile == false)
				estado = EM_PE;

		}
		else
		{
			if (agachado_ultimo_frame)
			{
				hitbox.h = dimesao_em_pe.y;
				hitbox.y -= (dimesao_em_pe.y - dimesao_agachado.y);
				agachado_ultimo_frame = false;
			}

			if (velocidade_y < 0)
				estado = PULANDO;
			else
				estado = CAINDO;
		}
	}
}



void Entidade::set_sprite_sheet(Textura* sprites)
{
	for (int i = 0; i < END; ++i) {
		sprite_sheet[i] = &sprites[i];
	}
}
void Entidade::get_teclado_ultimo_frame()
{
	static bool frame1 = true;

	if (frame1 == false)
		for (int i = 0; i < SDL_NUM_SCANCODES; ++i) {
			teclado_ultimo_frame[i] = teclado[i];
		}

	if (frame1 == true)
		frame1 = false;
}
bool Entidade::butao_precionado(int scancode)
{
	if (teclado[scancode] && !teclado_ultimo_frame[scancode])
		return true;
	else
		return false;
}
bool Entidade::butao_solto(int scancode)
{
	if (!teclado[scancode] && teclado_ultimo_frame[scancode])
		return true;
	else
		return false;
}
