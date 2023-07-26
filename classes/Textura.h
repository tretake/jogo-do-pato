#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <fstream>
#include <string>
#include <iostream>

int iniciar_SDL(SDL_Window*& p_janela, SDL_Renderer*& p_render);
void fechar_SDL(SDL_Window*& p_janela, SDL_Renderer*& p_render);


bool colisao(SDL_FRect a, SDL_FRect b);

class Textura
{
public:

	static SDL_Renderer* trender;
	static SDL_Window* tjanela ;

	TTF_Font* tfonte = NULL;

	SDL_Texture* imagem = NULL;


	Textura();


	~Textura();




	void desenhar(  SDL_FRect* p_destino, SDL_FRect* p_camera, SDL_Rect* crop = NULL, bool flip = false );
	
	void desenhar_alvo(SDL_FRect alvo);
	void desenhar_alvo_cheio(SDL_FRect alvo);

	bool carregar_textura(std::string path);

	bool carregar_fonte(int size, std::string path);

	bool carregar_texto(std::string texto);

	bool free();

	static void update_const_conversao();

	static bool setup(SDL_Renderer* prender, SDL_Window* pjanela);

	static float const_conversao_x;
	static float const_conversao_y;

};