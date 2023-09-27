#include "classes/textura.h"
#include "classes/Cenario.h"
#include "classes/Entidade.h"
#include <vector>


Uint32 tick_intervalo = 16; //16 -> 62fps
Uint32 proximo_tick;
float frames = 0;
int segundo = 0;
void  limit_frames()
{
	Uint32 agora;
	frames++;

	agora = SDL_GetTicks();

	/*
	if (agora > segundo + 1000)
	{
		std::cout << frames << "\n\n";

		segundo = agora;

		frames = 0;
	}*/


	if (proximo_tick > agora)
	{
		SDL_Delay(proximo_tick - agora);
	}
	else if (proximo_tick < agora)
		proximo_tick = agora;


	proximo_tick += tick_intervalo;
}


void carregando_assets(){
		sprite_pato[EM_PE].carregar_textura("art/Protagonista/EM_PE.png");
		sprite_pato[CORRENDO].carregar_textura("art/Protagonista/CORRENDO.png");
		sprite_pato[AGACHADO].carregar_textura("art/Protagonista/AGACHADO.png");
		sprite_pato[PULANDO].carregar_textura("art/Protagonista/PULANDO.png");
		sprite_pato[CAINDO].carregar_textura("art/Protagonista/CAINDO.png");
		sprite_pato[PLANANDO].carregar_textura("art/Protagonista/PLANANDO.png");
		sprite_pato[DASH].carregar_textura("art/Protagonista/DASH.png");
		sprite_pato[POGO].carregar_textura("art/Protagonista/POGO.png");
		sprite_pato[POGO_ATAQUE].carregar_textura("art/Protagonista/POGO_ATAQUE.png");
		sprite_pato[SLIDE].carregar_textura("art/Protagonista/SLIDE.png");
		sprite_pato[BALA].carregar_textura("art/Protagonista/BALA.png");
		sprite_pato[ATAQUE].carregar_textura("art/Protagonista/ATAQUE.png");
		sprite_pato[ATACANDO].carregar_textura("art/Protagonista/ATACANDO.png");
		sprite_pato[ATAQUE2].carregar_textura("art/Protagonista/ATAQUE2.png");
		sprite_pato[ATACANDO2].carregar_textura("art/Protagonista/ATACANDO2.png");
		sprite_pato[DANO].carregar_textura("art/Protagonista/megaman/DANO.png");




		sprite_megaman[EM_PE].carregar_textura("art/Protagonista/megaman/EM_PE.png");
		sprite_megaman[CORRENDO].carregar_textura("art/Protagonista/megaman/CORRENDO.png");
		sprite_megaman[AGACHADO].carregar_textura("art/Protagonista/megaman/AGACHADO.png");
		sprite_megaman[PULANDO].carregar_textura("art/Protagonista/megaman/PULANDO.png");
		sprite_megaman[CAINDO].carregar_textura("art/Protagonista/megaman/CAINDO.png");
		//sprite_megaman[PLANANDO].carregar_textura("art/Protagonista/megaman/PLANANDO.png");
		sprite_megaman[DASH].carregar_textura("art/Protagonista/megaman/DASH.png");
		//sprite_megaman[POGO].carregar_textura("art/Protagonista/megaman/POGO.png");
		//sprite_megaman[POGO_ATAQUE].carregar_textura("art/Protagonista/megaman/POGO_ATAQUE.png");
		sprite_megaman[SLIDE].carregar_textura("art/Protagonista/megaman/SLIDE.png");
		sprite_megaman[DANO].carregar_textura("art/Protagonista/megaman/DANO.png");
		sprite_megaman[BALA].carregar_textura("art/Protagonista/megaman/BALA.png");

		assets[POGO_PLANT].carregar_textura("art/Assets/planta1.png");
		assets[CAINDO].carregar_textura("art/Assets/planta1_fechada.png");
}

void desenhar_ui(Entidade jogador);

void ajustar_camera(Entidade jogador)
{
	int x = (int)(jogador.hitbox.x / jogador.E_mapa->unidade);
	int y = (int)(jogador.hitbox.y / jogador.E_mapa->unidade);

	if (jogador.E_mapa->tile_em(x,y ) == '+')
	{
		int lim_x = x - 1;
		int lim_y = y - 1;
		while (jogador.E_mapa->tile_em(lim_x, y) == '+')
		{
			lim_x--;
		}
		while (jogador.E_mapa->tile_em(x, lim_y) == '+')
		{
			lim_y--;
		}
		sistema_camera.x = lim_x + 1*jogador.E_mapa->unidade;
		sistema_camera.y = lim_y + 2*jogador.E_mapa->unidade;
	}
	else {
		sistema_camera.x = jogador.hitbox.x - 1600 * 0.5f + jogador.hitbox.w * 0.5f;
		sistema_camera.y = jogador.hitbox.y - 900 * 0.5f;
	}
}


int main(int argc, char* argv[])
{

	srand(time(0));

	iniciar_SDL(sistema_janela , sistema_render);

	carregando_assets();

	Cenario a("tile_map.txt");
	a.unidade = 90;



	Entidade jogador({ 200.f,200.f,130.f,130.f } , sprite_pato , &a );


	/*
	{	//BOSS CHAPUZINHO
		Entidade megaman( { 300.f,200.f,130.f,140.f } , sprite_megaman , &a );
		Entidade::Seres.push_back(megaman);
	}*/
	

	/*
	{ //assets
			
	}	
	*/
		
		


	proximo_tick = SDL_GetTicks() + tick_intervalo;

	while (jogador.rodando == true)
	{
		SDL_SetRenderDrawColor(sistema_render, 0x00, 0x00, 0x00, 0x00);
		SDL_RenderClear(sistema_render);

		


		jogador.imput();
		
		jogador.mover();


		
		a.desenhar_fundo( sistema_camera);
		a.desenhar_mapa();

		//quarentena
		for (int i = Entidade::Seres.size() -1 ; i >= 0 ; i--)
		{
			colisao_detalhe colisao_senario;
			/*if (Entidade::Seres[i].estado != BALA || Entidade::Seres[i].estado != POGO_PLANT)
			{*/
				Entidade::Seres[i].reset_estado();
				Entidade::Seres[i].inteligencia(jogador);
			
			
			colisao_senario = Entidade::Seres[i].mover();




			Entidade::Seres[i].desenhar();
			
			if (Entidade::Seres[i].estado == BALA)
			{
				if (colisao(jogador.hitbox, Entidade::Seres[i].hitbox))
				{
					colisao_senario.caso = DENTRO;
					//pode ser uma funcao , ou fundir com a tomou_dano()
					SDL_FRect metade_esquerda = {jogador.hitbox.x , jogador.hitbox.y,jogador.hitbox.w/2,jogador.hitbox.y};
					if (colisao(metade_esquerda, Entidade::Seres[i].hitbox))
						jogador.tomou_dano(ESQUERDA, 10);
					else
						jogador.tomou_dano(DIREITA, 10);
				
				}
				if (colisao_senario.caso == DENTRO)
					Entidade::Seres.erase(Entidade::Seres.begin() + i);
			}
			
		}
		//quarentena
		
		

		jogador.desenhar();

		
		ajustar_camera(jogador);


		limit_frames();
		SDL_RenderPresent(sistema_render);
	}
	






	fechar_SDL(sistema_janela, sistema_render);
	return EXIT_SUCCESS;
}



//em quarentenna
void desenhar_ui(Entidade jogador)
{
	SDL_FRect dash_barra = { 20 , 20 , 99.9f , 25 };
	SDL_SetRenderDrawColor(sistema_render, 0x00, 0x50, 0x00, 0x00);
	SDL_RenderFillRectF(sistema_render, &dash_barra);	//back
	float nivel = 99.9f - (3.33f * jogador.dash_cooldown);
	dash_barra = { 20 , 20 , nivel  , 25 };
	if (nivel == 99.9f)
		SDL_SetRenderDrawColor(sistema_render, 0x00, 0xFF, 0x00, 0x00);
	else
		SDL_SetRenderDrawColor(sistema_render, 0x00, 0x90, 0x00, 0x00);
	SDL_RenderFillRectF(sistema_render, &dash_barra);


	dash_barra = { 20 , 50 , 100.f , 25 };
	SDL_SetRenderDrawColor(sistema_render, 0x00, 0x00, 0x50, 0x00);
	SDL_RenderFillRectF(sistema_render, &dash_barra);	//back
	nivel = 100.f - (4 * jogador.pogo_cooldown);
	dash_barra = { 20 , 50 , nivel  , 25 };
	if (nivel == 100.f)
		SDL_SetRenderDrawColor(sistema_render, 0x00, 0x00, 0xFF, 0x00);
	else
		SDL_SetRenderDrawColor(sistema_render, 0x00, 0x00, 0x90, 0x00);
	SDL_RenderFillRectF(sistema_render, &dash_barra);

}



