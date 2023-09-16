#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <time.h>


struct vector2d
{
	int x;
	int y;
};

struct vector2df
{
	float x;
	float y;

};

extern SDL_FRect sistema_camera;

extern float const_conversao_x ;
extern float const_conversao_y ;

extern SDL_Renderer* sistema_render ;
extern SDL_Window* sistema_janela ;

int iniciar_SDL(SDL_Window*& p_janela, SDL_Renderer*& p_render);
void fechar_SDL(SDL_Window*& p_janela, SDL_Renderer*& p_render);
void update_const_conversao();

void desenhar_alvo(SDL_FRect hitbox, SDL_FRect p_camera, bool preechido = false);

bool colisao(SDL_FRect a, SDL_FRect b);


class Textura
{
public:

	TTF_Font* tfonte = NULL;

	SDL_Texture* imagem = NULL;


	Textura();


	~Textura();




	void desenhar(  SDL_FRect* p_destino, SDL_Rect* crop = NULL, bool flip = false );
	void desenhar_estatico(SDL_FRect* p_destino, SDL_Rect* crop = NULL, bool flip = false);

	bool carregar_textura(std::string path);

	bool carregar_fonte(int size, std::string path);

	bool carregar_texto(std::string texto, int wrap_leght);

	bool free();


};