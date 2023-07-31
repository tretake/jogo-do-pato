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








int main(int argc, char* argv[])
{
	iniciar_SDL(sistema_janela , sistema_render);



	Entidade jogador;
	jogador.sprite.carregar_textura("art/Protagonista/PatinhoFrames.png");
	jogador.hitbox = { 1.f,1.f,140.f,160.f };


	int largura_tela = 0;
	int altura_tela = 0;
	


	Textura background;
	background.carregar_textura("art/Fundo/fundo.png");

	Cenario a("tile_map.txt");											


	proximo_tick = SDL_GetTicks() + tick_intervalo;


	while (jogador.rodando == true)
	{
		SDL_SetRenderDrawColor(sistema_render, 0x00, 0x00, 0x00, 0x00);
		SDL_RenderClear(sistema_render);

		

		jogador.imput_sistema(&a,sistema_camera);
		jogador.imput();
		

		jogador.mover(a);



		a.desenhar_fundo(jogador.hitbox);

		

		a.desenhar_mapa(sistema_camera);
		jogador.desenhar(&sistema_camera);
		

		
		{//prototipo de funcao de ajuste de tela
			SDL_SetRenderDrawColor(sistema_render, 0xFF, 0x00, 0x00, 0x00);
			SDL_FRect hitbox_jogador = jogador.hitbox;

			sistema_camera.x = jogador.hitbox.x - 1600/2 + jogador.hitbox.w/2;

			sistema_camera.y = jogador.hitbox.y - 900 * 0.50f;

			//jogador.desenhar_hitbox(render, camera);	diminuir hitbox quando agachado
		}




		{
			SDL_FRect dash_barra = { 20 , 20 , 99.9f , 25 };
			SDL_SetRenderDrawColor(sistema_render, 0x00, 0x50, 0x00, 0x00);
			SDL_RenderFillRectF(sistema_render, &dash_barra);	//back
			float nivel = 99.9f -(3.33f * jogador.dash_cooldown);
			dash_barra = { 20 , 20 , nivel  , 25 };
			if(nivel == 99.9f)
				SDL_SetRenderDrawColor(sistema_render, 0x00, 0xFF, 0x00, 0x00);
			else
				SDL_SetRenderDrawColor(sistema_render, 0x00, 0x90, 0x00, 0x00);
			SDL_RenderFillRectF(sistema_render, &dash_barra);


			dash_barra = { 20 , 50 , 100.f , 25 };
			SDL_SetRenderDrawColor(sistema_render, 0x00, 0x00, 0x50, 0x00);
			SDL_RenderFillRectF(sistema_render, &dash_barra);	//back
			nivel = 100.f - (4* jogador.pogo_cooldown);
			dash_barra = { 20 , 50 , nivel  , 25 };
			if (nivel == 100.f)
				SDL_SetRenderDrawColor(sistema_render, 0x00, 0x00, 0xFF, 0x00);
			else
				SDL_SetRenderDrawColor(sistema_render, 0x00, 0x00, 0x90, 0x00);
			SDL_RenderFillRectF(sistema_render, &dash_barra);

		}

		limit_frames();
		SDL_RenderPresent(sistema_render);
	}
	



	fechar_SDL(sistema_janela, sistema_render);


	return EXIT_SUCCESS;
}



