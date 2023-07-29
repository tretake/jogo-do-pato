#include "Cenario.h"




Cenario::Cenario(std::string pmap_file)
{
	map_file.open(pmap_file.c_str());

	tile_set.carregar_textura("art/Assets/Tileset.png");

	if (map_file.is_open())
	{
		std::string linha;

		getline(map_file, linha);
		tile_map += linha;				//codigo da preguiça
		colunas = linha.size();

		while (std::getline(map_file, linha))
		{
			tile_map += linha;

		} 

		linhas = tile_map.size() / colunas;

		std::cout << "linhas" << linhas << "  colunas" << colunas << "\n";

		map_file.close();
	}
	else
	{
		printf("falhou ao abrir o arquivo de texto\n");
	}
}

char Cenario::tile_em(int x, int y)
{
	if (x < colunas && x >= 0 && y < linhas && y >= 0)
		return tile_map[colunas * y + x];
	else
		return '.';
}



colisao_detalhe Cenario::colisao_cenario(SDL_FRect caixa)
{
	colisao_detalhe colisao_status = {FORA,0,0};


	SDL_FRect tile_rect = { 0,0,unidade,unidade };

	int tile_esquerda = (int)(caixa.x / unidade);
	int tile_direita = (int)((caixa.x + caixa.w) / unidade);
	int tile_topo = (int)(caixa.y/unidade);
	int tile_baixo = (int)((caixa.y + caixa.h ) / unidade);


	for (int i = tile_esquerda; i <= tile_direita; i++)
	{
		for (int j = tile_topo; j <= tile_baixo; j++)
		{			
			if (tile_em(i, j) == 'x')
			{
				tile_rect.x = i*unidade;
				tile_rect.y = j*unidade;
				if (colisao(caixa, tile_rect))
				{
					colisao_status = { DENTRO, i * (int)(unidade) , j * (int)(unidade) };
					return colisao_status;
				}
				else if (caixa.y == tile_rect.y - caixa.h && (caixa.x + caixa.w > tile_rect.x))
					colisao_status = { ENCOSTANDO , i * (int)(unidade), j * (int)(unidade) };

			}
			
		}
	}

	return colisao_status;
}

void Cenario::desenhar_mapa(SDL_FRect p_camera)
{

	SDL_Rect crop = { 0,0,0,0 };
	SDL_FRect sprite_destino = { 0,0,0,0 };


	int tile_esquerda = (int)(p_camera.x / unidade);
	int tile_direita = (int)((p_camera.x + p_camera.w) / unidade);
	int tile_topo = (int)(p_camera.y / unidade);
	int tile_baixo = (int)((p_camera.y + p_camera.h) / unidade);

	char tile_t = ' ';
	for (int i = tile_esquerda ; i <= tile_direita ; i++)
		for (int j = tile_topo; j <= tile_baixo; j++)
		{

			tile_t = tile_em(i, j);
			switch (tile_t)
			{
			case 'x':
				sprite_destino = { (i * unidade) - unidade * 0.5f , (j * unidade) - unidade * 0.5f ,unidade + unidade * 0.5f ,unidade + unidade };
				crop = { 421,74,227,227 };
				tile_set.desenhar(&sprite_destino, &p_camera, &crop);

				sprite_destino = { (i * unidade) + unidade , (j * unidade) - unidade * 0.5f, unidade * 0.5f, 2 * unidade };
				crop = { 1841,74,78,227 };
				tile_set.desenhar(&sprite_destino, &p_camera, &crop);
				break;
			}
		}

}