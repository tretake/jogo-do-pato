#include "classes/textura.h"
#include "classes/Cenario.h"
#include "classes/Entidade.h"
#include <vector>


Uint32 tick_intervalo = 16; //16 -> 62fps
Uint32 proximo_tick;
void  limit_frames()
{
	Uint32 agora;
	agora = SDL_GetTicks();
	
	if (proximo_tick > agora)
	{
		SDL_Delay(proximo_tick - agora);
	}
	proximo_tick += tick_intervalo;

	//	resolver casos em q 'agora' adianta em relacoes ao tick, ex: resizing window
}



SDL_Window* janela = NULL;
SDL_Renderer* render = NULL;




int main(int argc, char* argv[])
{
	iniciar_SDL(janela , render);
	Textura::setup(render, janela);

	Entidade jogador;
	jogador.sprite.carregar_textura("art/Protagonista/PatinhoFrames.png");
	jogador.hitbox = { 100.f,100.f,140.f,160.f };


	int largura_tela = 0;
	int altura_tela = 0;
	SDL_GetWindowSize(janela, &largura_tela, &altura_tela);
	SDL_FRect camera = { 0,0,(float)largura_tela,(float)altura_tela };


	Textura background;
	background.carregar_textura("art/Fundo/fundo.png");

	Cenario a("tile_map.txt");											


	proximo_tick = SDL_GetTicks() + tick_intervalo;


	while (jogador.rodando == true)
	{
		SDL_SetRenderDrawColor(render, 0x00, 0x00, 0x00, 0x00);
		SDL_RenderClear(render);

		

		jogador.imput_sistema();
		jogador.imput();
		


		jogador.mover(a);



		background.desenhar(NULL,&camera);
		a.desenhar_mapa(&camera);

		jogador.desenhar(&camera);
		

		
		{//prototipo de funcao de ajuste de tela
			SDL_SetRenderDrawColor(render, 0xFF, 0x00, 0x00, 0x00);
			SDL_FRect hitbox_jogador = jogador.hitbox;

				camera.x = jogador.hitbox.x - 1600/2 + jogador.hitbox.w/2;

				camera.y = jogador.hitbox.y - 900 * 0.50f;

			//jogador.desenhar_hitbox(render, camera);	diminuir hitbox quando agachado
		}
		limit_frames();

		SDL_RenderPresent(render);
	}
	


	fechar_SDL(janela,render);


	return EXIT_SUCCESS;
}



