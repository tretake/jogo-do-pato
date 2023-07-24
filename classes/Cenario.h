#pragma once

#include "Textura.h"

bool colisao(SDL_FRect a, SDL_FRect b);

class Cenario
{
public:
	Textura tile_set;

	std::string tile_map = "";

	std::ifstream map_file;

	

	float unidade = 50;
	size_t tamanho = 0;

	Cenario(std::string pmap_file);

	void desenhar_mapa();

	SDL_Rect colisao_cenario(SDL_FRect caixa);

	char tile_em(int x, int y);

};