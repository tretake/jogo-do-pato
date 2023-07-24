#include "Cenario.h"




Cenario::Cenario(std::string pmap_file)
{
	map_file.open(pmap_file.c_str());

	tile_set.carregar_textura("art/Assets/Tileset.png");

	if (map_file.is_open())
	{
		char c;
		while (map_file.get(c))
		{
			tile_map += c;
		}
		map_file.close();
	}
	else
	{
		printf("falhou ao abrir o arquivo de texto\n");
	}
	tamanho = tile_map.size();
}

char Cenario::tile_em(int x, int y)
{
	int linha = 0;
	int pos = 0;
	for(char a : tile_map)
	{
		if (linha == y)
		{
			break;
		}
		if( a == '\n')
		{
			linha++;
		}
		pos++;
	}
	return tile_map[pos + x];
}

SDL_Rect Cenario::colisao_cenario(SDL_FRect caixa)
{

	SDL_Rect tile_colidida = { 0,0,0,0 };

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
					tile_colidida = { i*(int)(unidade) , j * (int)(unidade) ,(int)unidade ,(int)unidade};
					return tile_colidida;
				}
			}
			
		}
	}

	return tile_colidida;
}

void Cenario::desenhar_mapa()	//melhorar esse codigo
{
	tile_set.alvo = { 0,0,unidade,unidade };

	SDL_Rect crop = { 0,0,0,0 };
	SDL_FRect sprite_destino = { 0,0,0,0 };

	size_t tamanho = tile_map.size();

	int linha = 0;
	int coluna = 0;
	for (char a : tile_map)
	{
		switch (a)
		{
		case 'x':

			sprite_destino = { (coluna*unidade) - unidade * 0.5f ,(linha*unidade) - unidade * 0.5f ,unidade + unidade * 0.5f ,unidade + unidade };
			crop = { 421,74,227,227 };
			tile_set.desenhar(&crop, &sprite_destino);

			sprite_destino = { (coluna*unidade) + unidade , (linha*unidade) - unidade * 0.5f, unidade * 0.5f, 2 * unidade };
			crop = { 1841,74,78,227 };
			tile_set.desenhar(&crop, &sprite_destino);

			break;
		case '\n':
			linha++;
			coluna = -1;
			break;
		default:
			break;
		}
		coluna++;
	}

}