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

	camadas[0].carregar_textura("art/Fundo/PatinhoFundoFlorestaNegra(1).png");
	camadas[1].carregar_textura("art/Fundo/PatinhoFundoFlorestaNegra(2).png");
	camadas[2].carregar_textura("art/Fundo/PatinhoFundoFlorestaNegra(3).png");
	camadas[3].carregar_textura("art/Fundo/PatinhoFundoFlorestaNegra(4).png");
	camadas[4].carregar_textura("art/Fundo/PatinhoFundoFlorestaNegra(5).png");
	camadas[5].carregar_textura("art/Fundo/PatinhoFundoFlorestaNegra(6).png");
	
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

void Cenario::desenhar_fundo(SDL_FRect& camera)
{
	SDL_FRect alvo_fundo = { 0, 0,1600,900 };
	SDL_FRect zero = { 0,0,0,0 };

	float distancia;

	static float pos_inicial[6] = { 0 };
	

	//alvo_fundo.x = pos_inicial;
	float efeito_paralax ;

	for (int i = 0; i < 6; i++)
	{
		switch (i)
		{
		case 0:
			efeito_paralax = 1.0f;
			break;
		case 1:
			efeito_paralax = 0.9f;
			break;
		case 2:
			efeito_paralax = 0.85f;
			break;
		case 3:
			efeito_paralax = 0.75f;
			break;
		case 4:
			efeito_paralax = 0.65f;
			break;
		case 5:
			efeito_paralax = 0.45f;
			break;
		}
		distancia  = camera.x*(1-efeito_paralax);

		if ( -distancia + pos_inicial[i] > alvo_fundo.w)
			pos_inicial[i] -= alvo_fundo.w;
		else if ( -distancia + pos_inicial[i] <  - alvo_fundo.w)
			pos_inicial[i] += alvo_fundo.w;


		alvo_fundo.x = - distancia + pos_inicial[i];
		
		//std::cout << "ponto inicial de print " << alvo_fundo.x << "\n";


		camadas[5 - i].desenhar(&alvo_fundo, &zero, NULL);
		alvo_fundo.x -= 1600;
		camadas[5 - i].desenhar(&alvo_fundo, &zero, NULL);
		alvo_fundo.x += alvo_fundo.w;
		camadas[5 - i].desenhar(&alvo_fundo, &zero, NULL);
		alvo_fundo.x += alvo_fundo.w;
		camadas[5 - i].desenhar(&alvo_fundo, &zero, NULL);

	}
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