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
		sprite_pato[ATAQUE].carregar_textura("art/Protagonista/punch1animation.png");
		sprite_pato[ATACANDO].carregar_textura("art/Protagonista/ATACANDO.png");
		sprite_pato[ATAQUE2].carregar_textura("art/Protagonista/punch2animation.png");
		sprite_pato[ATACANDO2].carregar_textura("art/Protagonista/ATACANDO2.png");
		sprite_pato[DANO].carregar_textura("art/Protagonista/DANO.png");
		sprite_pato[MORTO].carregar_textura("art/Protagonista/MORTO.png");




		sprite_chapeuzinho[EM_PE].carregar_textura("art/Protagonista/megaman/EM_PE.png");
		sprite_chapeuzinho[CORRENDO].carregar_textura("art/Protagonista/megaman/CORRENDO.png");
		sprite_chapeuzinho[AGACHADO].carregar_textura("art/Protagonista/megaman/AGACHADO.png");
		sprite_chapeuzinho[PULANDO].carregar_textura("art/Protagonista/megaman/PULANDO.png");
		sprite_chapeuzinho[CAINDO].carregar_textura("art/Protagonista/megaman/CAINDO.png");
		//sprite_megaman[PLANANDO].carregar_textura("art/Protagonista/megaman/PLANANDO.png");
		sprite_chapeuzinho[DASH].carregar_textura("art/Protagonista/megaman/DASH.png");
		//sprite_megaman[POGO].carregar_textura("art/Protagonista/megaman/POGO.png");
		//sprite_megaman[POGO_ATAQUE].carregar_textura("art/Protagonista/megaman/POGO_ATAQUE.png");
		sprite_chapeuzinho[SLIDE].carregar_textura("art/Protagonista/megaman/SLIDE.png");
		sprite_chapeuzinho[DANO].carregar_textura("art/Protagonista/megaman/DANO.png");
		sprite_chapeuzinho[BALA].carregar_textura("art/Protagonista/megaman/BALA.png");
		sprite_chapeuzinho[TP].carregar_textura("art/Protagonista/megaman/TP.png");

		assets[EM_PE].carregar_textura("art/Assets/planta1.png");
		assets[DANO].carregar_textura("art/Assets/planta1_fechada.png");
		assets[1].carregar_textura("art/HUD/heart_animation.png");
		assets[2].carregar_textura("art/HUD/heart.png");
		assets[3].carregar_textura("art/HUD/DS_death.png");
}


void heart_ui(Entidade jogador);

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



	Entidade jogador({ 200.f,5100.f,130.f,130.f } , sprite_pato , &a );


	
	{	//BOSS CHAPUZINHO
		
		/*Entidade tpchapeu({1000.f,5100.f,300.f,300.f}, sprite_chapeuzinho, &a, CHAPEUZINHO);
		tpchapeu.estado = TP;
		Entidade::Seres.push_back(tpchapeu);*/
		 
		Entidade chapeu({1000.f,5100.f,130.f,140.f}, sprite_chapeuzinho, &a, CHAPEUZINHO);
		chapeu.hp = 100;
		Entidade::Seres.push_back(chapeu);
	}
	
	


	proximo_tick = SDL_GetTicks() + tick_intervalo;

	while (jogador.rodando == true)
	{
		SDL_SetRenderDrawColor(sistema_render, 0x00, 0x00, 0x00, 0x00);
		SDL_RenderClear(sistema_render);

		


		jogador.imput();
		
		jogador.mover();
		jogador.hp_frame_anterior = jogador.hp;
		
		
		a.desenhar_fundo( sistema_camera);
		a.desenhar_mapa();

		//quarentena
		for (int i = Entidade::Seres.size() -1 ; i >= 0 ; i--)
		{
			colisao_detalhe colisao_senario;
			
			if (Entidade::Seres[i].hp <= 0)
				continue;

			if (Entidade::Seres[i].estado == TP)
			{
				Entidade::Seres[i].mover_y();
				Entidade::Seres[i].desenhar();
				continue;
			}
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
						jogador.tomou_dano(ESQUERDA, 1);
					else
						jogador.tomou_dano(DIREITA, 1);
				
				}
				if (colisao_senario.caso == DENTRO)
					Entidade::Seres.erase(Entidade::Seres.begin() + i);
			}
			
		}
		//quarentena
		
		

		jogador.desenhar();

		
		ajustar_camera(jogador);

		

		heart_ui(jogador);
		
		

		limit_frames();
		SDL_RenderPresent(sistema_render);
	}
	



	


	fechar_SDL(sistema_janela, sistema_render);
	return EXIT_SUCCESS;
}



void heart_ui(Entidade jogador)
{

	int hearts_lost = jogador.hp_frame_anterior - jogador.hp;
	if (hearts_lost != 0)
	{
		assets[1].animar(5, 5);	//SÓ FUNCIONA UM CORAÇÃO POR VEZ
		assets[1].alvo_estatico = { 10.0f + (jogador.hp_frame_anterior - 1)* 100.0f , 10.0f , 120.0f , 120.0f };
		/*for (int i = 0; i < hearts_lost; i++)
		{

		}*/
	}
	if (assets[1].frames_total != 0)
		assets[1].desenhar_estatico(&assets[1].alvo_estatico);
	

	int hearts = jogador.hp;
	SDL_Rect heart_crop = { 0,0,500,500 };
	for (int i = 0; i < hearts; i++)
	{
		SDL_FRect ani_alvo = { 10 + i*100 ,10,120,120 };
		assets[2].desenhar_estatico(&ani_alvo, &heart_crop);
	}
	
	if (jogador.hp <= 0)
	{
		SDL_FRect tela = { 0,(sistema_camera.h / 2 - 200) , sistema_camera.w ,200 };
		assets[3].desenhar_estatico(&tela);
	}


}




