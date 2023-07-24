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
int iniciar_SDL();	//todo: passar janela como referencia, nao usar variavel global plss!
void fechar_SDL();



int main(int argc, char* argv[])
{
	iniciar_SDL();
	Textura::setup(render, janela);

	Entidade jogador;

	Textura background;
	background.carregar_textura("art/Fundo/fundo.png");

	jogador.sprite.carregar_textura("art/Protagonista/PatinhoFrames.png");
	jogador.sprite.alvo = { 100.f,100.f,180.f,180.f };					//posicao inicial jogador


	Cenario a("tile_map.txt");											


	proximo_tick = SDL_GetTicks() + tick_intervalo;

	while (jogador.rodando == true)
	{
		SDL_SetRenderDrawColor(render, 0x00, 0x00, 0x00, 0x00);
		SDL_RenderClear(render);

		background.desenhar();
		a.desenhar_mapa();

		jogador.imput_sistema();
		jogador.imput();
		


		jogador.mover(a);



		jogador.desenhar();



		limit_frames();

		SDL_RenderPresent(render);
	}
	


	fechar_SDL();


	return EXIT_SUCCESS;
}



int iniciar_SDL()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		printf("falhou ao iniciar SDL, erro : %s \n", SDL_GetError());
		return EXIT_FAILURE;
	}
	else
	{
		janela = SDL_CreateWindow("pato", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1600, 900, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
		if (janela == NULL)
		{
			printf("falhou ao criar a janela, erro : %s \n", SDL_GetError());
			return EXIT_FAILURE;
		}
		else
		{

			render = SDL_CreateRenderer(janela, -1, SDL_RENDERER_ACCELERATED);

			if (render == NULL)
			{
				printf("falou ao criar o render, erro %s \n", SDL_GetError());
				return EXIT_FAILURE;
			}

			if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
			{
				printf("falhou ao iniciar SDL_image, error %s \n", IMG_GetError());
			}
			if (TTF_Init() == -1)
			{
				printf("falhou ao inicializar SDL_ttf , error: %s \n", TTF_GetError());
				return EXIT_FAILURE;
			}

			return EXIT_SUCCESS;

		}
	}
}
void fechar_SDL()
{
	SDL_DestroyRenderer(render);
	SDL_DestroyWindow(janela);
	SDL_Quit();

}