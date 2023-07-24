#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <fstream>
#include <string>
#include <iostream>

class Textura
{
public:

	static SDL_Renderer* trender;
	static SDL_Window* tjanela ;

	TTF_Font* tfonte = NULL;

	SDL_FRect alvo = { 0.00f , 0.00f ,0.00f ,0.00f };

	SDL_Texture* imagem = NULL;


	Textura();


	~Textura();




	void desenhar(SDL_Rect* crop = NULL , SDL_FRect* p_alvo = NULL , bool flip = false);
	
	void desenhar_alvo();
	void desenhar_alvo_cheio();

	bool carregar_textura(std::string path);

	bool carregar_fonte(int size, std::string path);

	bool carregar_texto(std::string texto);

	bool free();

	static void update_const_conversao();

	static bool setup(SDL_Renderer* prender, SDL_Window* pjanela);

private:
	static float const_conversao_x;
	static float const_conversao_y;

};