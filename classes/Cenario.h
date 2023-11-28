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

	//criar enumeraçao pra diferentes tiles
	Textura tile_set[6];

	std::string tile_map = "";

	std::ifstream map_file;

	Textura camadas[7];

	float unidade = 50;
	int colunas = 0;
	int linhas = 0;

	Cenario(std::string pmap_file);

	void mudar_tile(SDL_FRect& camera , SDL_FRect jogador);
	void salvar_mapa();
	void desenhar_fundo(SDL_FRect& camera);
	void desenhar_mapa();

	colisao_detalhe colisao_cenario(SDL_FRect caixa);

	char tile_em(int x, int y);

};