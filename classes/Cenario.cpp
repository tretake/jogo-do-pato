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

	int tile_esquerda = caixa.x / unidade;
	int tile_direita = (caixa.x + caixa.w) / unidade;
	int tile_topo = caixa.y/unidade;
	int tile_baixo = (caixa.y + caixa.h ) / unidade;


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

void Cenario::desenhar_mapa()
{
	float x = 0;
	float y = 0;
	tile_set.alvo = { x,y,unidade,unidade };

	SDL_Rect crop = { 0,0,0,0 };
	SDL_FRect sprite_destino = { 0,0,0,0 };

	int tamanho = tile_map.size();
	for (int i = 0; i < tamanho; i++)
	{
		tile_set.alvo.x = x;
		tile_set.alvo.y = y;
		switch (tile_map[i])
		{
		case '\n':
			y += unidade;
			x = -unidade;
			break;
		case 'x':

			sprite_destino = { x - unidade * 0.5f ,y - unidade * 0.5f ,unidade + unidade*0.5f ,unidade + unidade  };
			crop = { 421,74,227,227 };
			tile_set.desenhar(&crop, &sprite_destino);
			
			sprite_destino = { x + unidade , y - unidade * 0.5f, unidade * 0.5f, 2 * unidade };
			crop = {1841,74,78,227};
			tile_set.desenhar(&crop,&sprite_destino);

			break;
		}
		x += unidade;
	}
}