#include "Entidade.h"

#include <math.h>


std::vector<Entidade> Entidade::Seres;
Textura sprite_pato[END];
Textura sprite_chapeuzinho[END];
Textura assets[END];


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
					hp = 3;
					reset_estado();
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
	
	if (frames_ataque == 0)
	{
		if (velocidade_x != 0) {
			if (velocidade_x > 0)
			{
				olhando_direita = true;
			}
			else
				olhando_direita = false;
		}
	}

	return colisao_status;
}
colisao_detalhe Entidade::mover_y()
{
	colisao_detalhe colisao_status = { FORA,0,0 };


	ultima_pos.y = hitbox.y;



	if (no_chao == false && estado != DASH && estado != BALA )
		velocidade_y += gravidade;

	hitbox.y += velocidade_y;

	colisao_status = E_mapa->colisao_cenario(hitbox);

	if (colisao_status.caso == DENTRO || colisao_status.caso == ENCOSTANDO)
	{

		if (estado == MORTO)
			velocidade_x = 0;

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


	

	return colisao_status;
}
colisao_detalhe Entidade::mover()
{
	colisao_detalhe colisao_x;
	colisao_detalhe colisao_y;

	if (tipo != POGO_PLANTA)
	{
		colisao_x = mover_x();
		colisao_y = mover_y();
	}
	update_cooldowns();

	if (colisao_x.caso == DENTRO)
		return colisao_x;
	else
		return colisao_y;
	
}




void Entidade::desenhar()
{
	SDL_FRect alvo;
	SDL_FRect ataque_hitbox;
	SDL_FRect pogo_hitbox;
	alvo.w = dimesao_em_pe.x * 1.40f;
	alvo.h = dimesao_em_pe.y * 1.40f;

	alvo.x = hitbox.x + hitbox.w / 2 - alvo.w / 2;
	alvo.y = hitbox.y + hitbox.h - alvo.h + 15;


	switch (estado)
	{
	case PLANANDO:
		alvo.x = alvo.x - alvo.w / 2;
		alvo.w = alvo.w * 2;
		break;

	case POGO:
		pogo_hitbox = { hitbox.x - 50 ,hitbox.y + dimesao_em_pe.y + 40.f * (1.f - ((float)frames_pogo / 10.f)) , hitbox.w + 50 , hitbox.h + 50 };
		sprite_sheet[POGO_ATAQUE]->desenhar(&pogo_hitbox, NULL, !olhando_direita);
		break;

	case ATACANDO:
	case ATACANDO2:

		if (olhando_direita)
			ataque_hitbox = { hitbox.x + dimesao_em_pe.x + (float)pow(1.2, -(-20 +frames_ataque) + 5)   , hitbox.y - 40 , hitbox.w + 50 , hitbox.h + 50};
		else
			ataque_hitbox = { hitbox.x - (hitbox.w + 50) - (float)pow(1.2, -(-20 + frames_ataque) + 5) , hitbox.y - 40 , hitbox.w + 50 , hitbox.h + 50 };

		if (estado == ATACANDO) {
			if(sprite_sheet[ATAQUE]->frames_total == 0)
			sprite_sheet[ATAQUE]->animar(12, 4);
			sprite_sheet[ATAQUE]->desenhar(&ataque_hitbox, NULL, !olhando_direita);
		}
		else
		{
			if (sprite_sheet[ATAQUE2]->frames_total == 0)
				sprite_sheet[ATAQUE2]->animar(12, 4);

			sprite_sheet[ATAQUE2]->desenhar(&ataque_hitbox, NULL, !olhando_direita);
		}
		break;
	}

	if (tipo == POGO_PLANTA)
	{
		alvo.w = dimesao_em_pe.x * 3.0f;
		alvo.h = dimesao_em_pe.y * 3.0f;
		alvo.x = hitbox.x + hitbox.w / 2 - alvo.w / 2;
		alvo.y = hitbox.y + hitbox.h / 2 - alvo.h / 2;
	}
		
	
	if ((  ((frames_invenc) % 6) < 4) || tipo == POGO_PLANTA)
			sprite_sheet[estado]->desenhar(&alvo, NULL, (frames_invenc == 0  && estado != MORTO) ? !olhando_direita : olhando_direita);
		
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

void Entidade::planar(int total_frames , int modulo_cooldown)
{
	if ( (velocidade_y > 0 || frames_planar != 0 ) && !no_chao)
	{
		
		if ( frames_planar == 0  && planar_cooldown == 0)
		{
			if (velocidade_y > 0)
				velocidade_y = gravidade*8;
			frames_planar = total_frames;
		}

		if (frames_planar == 0)
			return;
		//ATIVO
		frames_ataque = 0;
		ataque_combo = false;

		velocidade_y -= gravidade*1.58 ;
		estado = PLANANDO;
		frames_planar--;

		if (frames_planar == 0 )
		{
			planar_cooldown = modulo_cooldown;
		}
		

	}
}

void Entidade::dash(int total_frames , int multiplicador_velocidade , int modulo_cooldown , bool slide)	
{

	if (frames_dash == 0 && dash_cooldown == 0 && usou_dash_no_ar == false)	//dash iniciado
	{

		frames_dash = total_frames;
	
		if (no_chao == false)
			usou_dash_no_ar = true;

		if (slide == true)
			estado = SLIDE;
		
		
	}

	if (frames_dash == 0)
		return;

	//dash ativo
	frames_ataque = 0;
	ataque_combo = false;

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

	if (frames_dash != 0)
		return;
	//dash fim
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

void Entidade::pogo_ataque(int total_frames, float multiplicador_velocidade, int modulo_cooldown)
{
	if (pogo_cooldown == 0 && frames_pogo == 0)
	{
		frames_pogo = total_frames;
		pogo_hit = false;
	}

	if (frames_pogo != 0 && E_mapa != NULL)
	{

		SDL_FRect pogo_hitbox = { hitbox.x , hitbox.y + dimesao_em_pe.y + 40.f*( 1.f - ( (float)frames_pogo / (float)total_frames))  , dimesao_em_pe.x , dimesao_em_pe.y };

		bool acertou = false;
		for (auto& ser : Seres)
		{
			if (colisao(ser.hitbox, pogo_hitbox) )
			{
				if(ser.tomou_dano(ESQUERDA,10))
					acertou = true;

			}
		}
		if (pogo_hit == false && acertou == true)
		{
			velocidade_y = -multiplicador_velocidade * modulo_y;
			planar_cooldown = 0;
			usou_dash_no_ar = false;
			pogo_hit = true;
		}

		frames_pogo--;
		estado = POGO;
		if (frames_pogo == 0)
			pogo_cooldown = modulo_cooldown;
	}
	

}

void Entidade::ataque(int total_frames , int modulo_cooldown )
{

	
	if (ataque_cooldown == 0)
	{
		
		if (frames_ataque == 0) {
			estado = ATACANDO;
			frames_ataque = total_frames;
			velocidade_y = -10;
		}
		else if (ataque_combo == true )
		{
			estado = ATACANDO2;
			frames_ataque = total_frames;
			ataque_cooldown = total_frames;
			velocidade_y = -10;
		}
		
	}

	
	

	if (frames_ataque == 0)
		return;
	
	SDL_FRect ataque_hitbox;
	if (olhando_direita)
		ataque_hitbox = { hitbox.x + dimesao_em_pe.x + (float)pow(1.22, +20 - frames_ataque) - 5, hitbox.y - 40 , hitbox.w + 50 , hitbox.h + 50 };
	else
		ataque_hitbox = { hitbox.x - (hitbox.w + 50) - (float)pow(1.22, +20 - frames_ataque) - 5, hitbox.y - 40 , hitbox.w + 50 , hitbox.h + 50 };

	for (auto &ser : Seres)
	{
		if (ser.estado == BALA)
			continue;

		if(colisao(ser.hitbox, ataque_hitbox))
			ser.tomou_dano( (olhando_direita) ? ESQUERDA : DIREITA , 10 );
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





void Entidade::imput()
{
	imput_sistema();

	teclado = SDL_GetKeyboardState(NULL);

	if (estado == MORTO)
		return;
	
	if (estado != DASH && estado != SLIDE)
		reset_estado();


	

	if (frames_invenc != 0 )
		return;
	
	velocidade_x = 0;

	if (estado != DASH && estado != SLIDE)
	{
		if (butao_precionado(SDL_SCANCODE_E))
			interagir();


		
		
		if (butao_precionado(SDL_SCANCODE_P))
		{
			int mouse_x = 0;
			int mouse_y = 0;
			SDL_GetMouseState(&mouse_x, &mouse_y);
			spaw_pogo_plant(
				(mouse_x / const_conversao_x + sistema_camera.x),
				(mouse_y / const_conversao_y + sistema_camera.y)
			);
		}


		if (teclado[SDL_SCANCODE_S])
		{
			if (no_chao == true)
				estado = AGACHADO;
		}


		


		if (teclado[SDL_SCANCODE_D])
		{
			if (estado != AGACHADO)
				velocidade_x = modulo_x;
			if(frames_ataque == 0)
			olhando_direita = true;
		}
		if (teclado[SDL_SCANCODE_A])
		{
			if (estado != AGACHADO)
				velocidade_x = -modulo_x;
			if (frames_ataque == 0)
			olhando_direita = false;
		}


		if ((teclado[SDL_SCANCODE_SPACE] || teclado[SDL_SCANCODE_W]))
			planar(40, 67);
		else if (frames_planar != 0)
		{
			frames_planar = 0;
			planar_cooldown = 40;
		}

		if (butao_precionado(SDL_SCANCODE_SPACE) || butao_precionado(SDL_SCANCODE_W))
			pulo(5);
		if (butao_solto(SDL_SCANCODE_SPACE) || butao_solto(SDL_SCANCODE_W))
		{
			pulo(5, true);
		}

		
		if (estado != PLANANDO)
		{
			

			if ((butao_precionado(SDL_SCANCODE_N) && teclado[SDL_SCANCODE_S] && pogo_cooldown == 0 && frames_ataque == 0) || frames_pogo != 0)
			{
				if (no_chao == false)
					pogo_ataque(10, 4.5f, 25);
			}

			if ((butao_precionado(SDL_SCANCODE_N) && ataque_cooldown == 0 && frames_pogo == 0) || frames_ataque != 0)
			{
				if (butao_precionado(SDL_SCANCODE_N) && frames_ataque != 0)
					ataque_combo = true;
				ataque(25, 10);
			}
		}


	}


	if ((butao_precionado(SDL_SCANCODE_M) && dash_cooldown == 0) || frames_dash != 0)
	{
		if (estado == AGACHADO)
			dash(11, 4, 30, true);
		else
			dash(11, 4, 30);
	}

	
	
} 



bool Entidade::tomou_dano(int direcao  , int dano)
{
	if (estado == TP)
		return false;
	estado = DANO;
	if (frames_invenc != 0)
		return false;
	
	reset_estado();
	frames_dash = 0;

	hp -= dano;

	if (hp <= 0)
	{
		estado = MORTO;
	}

	if (tipo != POGO_PLANTA)
	{
		if (direcao == DIREITA)
			velocidade_x = -5.f;
		else
			velocidade_x = 5.f;

		velocidade_y = -15.f;
		frames_invenc = 45;
	}
	else
		frames_invenc = 100;

	
	return true;
	

}

void Entidade::inteligencia(Entidade alvo)
{
	if (frames_invenc != 0 || tipo == POGO_PLANTA)
		return ;

	if (alvo.hitbox.x > hitbox.x)
	{
		olhando_direita = true;
	}
	else
		olhando_direita = false;


	if (estado == BALA ||tipo == POGO_PLANTA)
		return;
	
	//std::cout << boss_padrao_cooldown << "\n";
	if (no_chao)
		velocidade_x = 0;
	if (no_chao && boss_padrao_cooldown == 0) {
	

		int moeda = rand() % 3;
		
		moeda = 1; //ALTERADO

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
			for (int i = 2; i < /*7*/ 3; i++)	//ALTERADO
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
		/*
		if (velocidade_x == 0)
			atirar(8, 15);	//ALTERADO
		else
			atirar(12, 18, BAIXO);*/
	}
		
}


void Entidade::atirar(int cooldown, double velocidade , int direcao )
{
	if (tiro_cooldown != 0)
		return;
	
	
	float aresta = 45;
	Entidade bala({ 0.0f,0.0f,aresta,aresta }, *sprite_sheet, E_mapa );
	bala.estado = BALA;
	bala.olhando_direita = olhando_direita;

	

	if (direcao == NEUTRO) {

		if (olhando_direita)
		{
			bala.hitbox = { hitbox.x + hitbox.w  + 1, hitbox.y + aresta  , aresta , aresta };
			bala.velocidade_x = velocidade;

		}
		else
		{
			bala.hitbox = { hitbox.x - aresta - 1 , hitbox.y + aresta  , aresta , aresta };
			bala.velocidade_x = -velocidade;
		}
	}
	if (direcao == BAIXO)
	{
		bala.hitbox = { hitbox.x + hitbox.w/2 , hitbox.y +hitbox.h +1 , aresta , aresta };
		bala.velocidade_y = velocidade;
	}

	Seres.push_back(bala);

	tiro_cooldown = cooldown;
	

}

void Entidade::spaw_pogo_plant(float p_x , float p_y)
{

	Entidade planta({ p_x , p_y ,70.0f,70.0f }, assets , E_mapa);
	//planta.olhando_direita = true;
	planta.tipo = POGO_PLANTA;
	planta.estado = EM_PE;

	Seres.push_back(planta);

}

bool Entidade::interagir()
{
	
	for (auto &ser : Seres)
	{
		if (ser.estado == TP)
		{
			
			if(colisao(hitbox, ser.hitbox))
			{
				
				switch (ser.tipo)
				{
				case CHAPEUZINHO:
					std::cout << "chapeuzinho luta ativada\n";
					hitbox.x = 200.f;
					hitbox.y = 200.f;

					Entidade chapeu({ 300.f,200.f,130.f,140.f }, sprite_chapeuzinho, E_mapa, CHAPEUZINHO);
					Entidade::Seres.push_back(chapeu);

					E_mapa->camadas[0].carregar_textura("art/Fundo/FundoChapeuVermelho.png");
					break;
				}
				return true;
			}
		}


	}

	return false;

}

void Entidade::update_cooldowns()
{
	if(tiro_cooldown != 0)
		tiro_cooldown--;

	if (frames_invenc != 0)
		frames_invenc--;

	if(boss_padrao_cooldown != 0)
	{
		boss_padrao_cooldown--;
	}

	if (ataque_cooldown != 0)
		ataque_cooldown--;

	if (pogo_cooldown != 0)
		pogo_cooldown--;

	if (planar_cooldown != 0)
		planar_cooldown--;

	if (dash_cooldown != 0)
	{
		dash_cooldown--;
		if (usou_dash_no_ar == true && dash_cooldown == 0)
			dash_cooldown = 1;
	}
	if (no_chao)
		usou_dash_no_ar = false;

}











//SOLIDO



void Entidade::reset_estado()	//fazendo muitas coisas
{

	
	
	if (frames_invenc != 0)
	{
		estado = DANO;
		return;
	}

	if (estado == BALA)
		return;

	if(tipo == POGO_PLANTA)
	estado = EM_PE;


	if (no_chao == true)
	{
		bool sob_tile = false;
		frames_planar = 0;



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
