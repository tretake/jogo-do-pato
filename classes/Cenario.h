#pragma once

#include "Textura.h"


enum colisao_casos
{
	DENTRO, FORA, ENCOSTANDO 
};
struct colisao_detalhe
{
	int caso;
	int x;
	int y;
};



class Cenario
{
public:
	Textura tile_set;

	std::string tile_map = "";

	std::ifstream map_file;

	

	float unidade = 50;
	int colunas = 0;
	int linhas = 0;

	Cenario(std::string pmap_file);

	void desenhar_mapa(SDL_FRect p_camera);

	colisao_detalhe colisao_cenario(SDL_FRect caixa);

	char tile_em(int x, int y);

};