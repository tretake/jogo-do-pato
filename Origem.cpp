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

	Textura background;
	background.carregar_textura("art/Fundo/fundo.png");

	jogador.sprite.carregar_textura("art/Protagonista/PatinhoFrames.png");
	jogador.hitbox = { 100.f,100.f,170.f,190.f };

	Cenario a("tile_map.txt");											


	proximo_tick = SDL_GetTicks() + tick_intervalo;

	while (jogador.rodando == true)
	{
		SDL_SetRenderDrawColor(render, 0x00, 0x00, 0x00, 0x00);
		SDL_RenderClear(render);

		background.desenhar(NULL);
		a.desenhar_mapa();

		jogador.imput_sistema();
		jogador.imput();
		


		jogador.mover(a);



		jogador.desenhar();



		limit_frames();

		SDL_RenderPresent(render);
	}
	


	fechar_SDL(janela,render);


	return EXIT_SUCCESS;
}



