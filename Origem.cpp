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
		//sprite_megaman[BALA].carregar_textura("art/Protagonista/megaman/BALA.png");
}

void desenhar_ui(Entidade jogador);

void ajustar_camera(Entidade jogador)
{
	sistema_camera.x = jogador.hitbox.x - 1600 * 0.5f + jogador.hitbox.w * 0.5f;
	sistema_camera.y = jogador.hitbox.y - 900 * 0.5f;
}


int main(int argc, char* argv[])
{

	iniciar_SDL(sistema_janela , sistema_render);

	Entidade jogador;
	jogador.hitbox = { 200.f,200.f,130.f,140.f };	//posição inicial e tamanho da hitbox

	
	jogador.set_sprite_sheet(sprite_pato);


	Cenario a("tile_map.txt");				

	carregando_assets();


	jogador.E_mapa = &a;

	{
		Entidade megaman;
		megaman.hitbox = { 300.f,200.f,130.f,140.f };
		megaman.E_mapa = &a;
		megaman.set_sprite_sheet(sprite_megaman);
		Entidade::Seres.push_back(megaman);
	}

	{
		std::ifstream tile_size;
		std::string size_str = "";
		int size_int = 0;
		tile_size.open("tile_size.txt");

		getline(tile_size, size_str);

		size_int = std::stoi(size_str);

		a.unidade = size_int;
		
		tile_size.close();

	}

	proximo_tick = SDL_GetTicks() + tick_intervalo;

	while (jogador.rodando == true)
	{
		SDL_SetRenderDrawColor(sistema_render, 0x00, 0x00, 0x00, 0x00);
		SDL_RenderClear(sistema_render);

		

		jogador.get_teclado_ultimo_frame();
		jogador.imput_sistema(sistema_camera);
		jogador.imput();
		





		
		jogador.mover();


		
		a.desenhar_fundo(jogador.hitbox);



		a.desenhar_mapa(sistema_camera);

		//quarentena
		for (auto &ser : Entidade::Seres)
		{
			ser.reset_estado();

			if(ser.frames_invenc == 0 && ser.estado != BALA)
				ser.inteligencia(jogador);

			ser.mover();
			ser.desenhar(&sistema_camera, sprite_megaman);

			if (ser.frames_invenc != 0)
			{
				ser.frames_invenc--;
			}
		}
		//quarentena


		jogador.desenhar(&sistema_camera , sprite_pato);

		
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



