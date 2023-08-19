#include "Cenario.h"



void Cenario::salvar_mapa()
{
	std::ofstream novo_mapa;
	novo_mapa.open("tile_map.txt");

	for (int j = 0 ; j < linhas ; j ++)
		novo_mapa << tile_map.substr(j*colunas,colunas) << '\n';

	novo_mapa.close();


	std::ofstream tile_size;
	tile_size.open("tile_size.txt");

	tile_size << unidade;

	tile_size.close();
}
Cenario::Cenario(std::string pmap_file)
{
	map_file.open(pmap_file.c_str());

	tile_set[0].carregar_textura("art/Assets/Tileset.png");
	tile_set[1].carregar_textura("art/Assets/floresta_negra1.png");
	tile_set[2].carregar_textura("art/Assets/FlorestaNegraTilesetFrames10.png");
	tile_set[3].carregar_textura("art/Assets/tijolo.png");
	tile_set[4].carregar_textura("art/Assets/Full.png");
	tile_set[5].carregar_textura("art/Assets/floresta_negra2.png");


	if (map_file.is_open())
	{
		std::string linha;

		getline(map_file, linha);
		tile_map += linha;				//codigo da preguiça
		colunas = (int)linha.size();

		while (std::getline(map_file, linha))
		{
			tile_map += linha;

		} 

		linhas = (int)tile_map.size() / colunas;

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





void Cenario::mudar_tile(SDL_FRect camera, SDL_FRect jogador)
{
	int unidade_hold = unidade;

	int hold_coordx = camera.x / unidade;
	int hold_coordy = camera.y / unidade;

	SDL_Event e;
	int mouse_x = 0;
	int mouse_y = 0;
	char nova_tile = '.';


	vector2d coordenadas_tile;
	SDL_FRect tile;



	std::vector<vector2d> tiles_modificadas;
	bool tecla_pressionada = false;
	bool mouse_pressionado = false;
	bool editor_rodando = true;

	bool espaco = false;
	vector2d quadrado_topleft{ 0,0 };


	while (editor_rodando == true)
	{
		tecla_pressionada = false;
		tiles_modificadas.clear();

		while (tecla_pressionada == false)
		{

				SDL_GetMouseState(&mouse_x, &mouse_y);
				coordenadas_tile.x = (int)((mouse_x / const_conversao_x + camera.x) / unidade);
				coordenadas_tile.y = (int)((mouse_y / const_conversao_y + camera.y) / unidade);
			
			while (SDL_PollEvent(&e))
			{
				switch (e.type)
				{
				case SDL_MOUSEBUTTONDOWN:

					mouse_pressionado = true;

					if (e.key.repeat == false && espaco == true) {

						quadrado_topleft.x = (int)((mouse_x / const_conversao_x + camera.x) / unidade);
						quadrado_topleft.y = (int)((mouse_y / const_conversao_y + camera.y) / unidade);
					}


					break;
				case SDL_MOUSEBUTTONUP: 
					mouse_pressionado = false;
				case SDL_KEYDOWN:
					switch (e.key.keysym.sym)
					{
						
						case SDLK_ESCAPE:
							editor_rodando = false;
							tecla_pressionada = true;
							break;

					case SDLK_w:
						camera.y -= unidade;
						break;
					case SDLK_s:
						camera.y += unidade;
						break;
					case SDLK_a:
						camera.x -= unidade;
						break;
					case SDLK_d:
						camera.x += unidade;
						break;

					case SDLK_DOWN:
						hold_coordx = camera.x / unidade;
						hold_coordy = camera.y / unidade;

						unidade -= 10;

						camera.x = hold_coordx * unidade;
						camera.y = hold_coordy * unidade;

						break;
					case SDLK_UP:
						hold_coordx = camera.x / unidade;
						hold_coordy = camera.y / unidade;

						unidade += 10;
						camera.x = hold_coordx * unidade;
						camera.y = hold_coordy * unidade;
						break;

						
					case SDLK_SPACE:
						
						espaco = true;
						
						break;
						

					case SDLK_z:
						nova_tile = 'z'; tecla_pressionada = true;
						break;
					case SDLK_x:
						nova_tile = 'x'; tecla_pressionada = true;
						break;
					case SDLK_c:
						nova_tile = 'c'; tecla_pressionada = true;
						break;
					case SDLK_v:
						nova_tile = 'v'; tecla_pressionada = true;
						break;
					case SDLK_b:
						nova_tile = 'b'; tecla_pressionada = true;
						break;
					case SDLK_PERIOD:
						nova_tile = '.'; tecla_pressionada = true;
						break;
					}
					break;
				case SDL_KEYUP:
					case SDLK_SPACE:
						espaco = false;

						break;
					break;
				}
			}

			if (mouse_pressionado == true && espaco == false)
			{
				tiles_modificadas.push_back(coordenadas_tile);
			}
			else if (mouse_pressionado == true && espaco == true)
			{
				for (int i = quadrado_topleft.x; i <= coordenadas_tile.x; i++)
					for (int j = quadrado_topleft.y; j <= coordenadas_tile.y; j++)
						tiles_modificadas.push_back(vector2d { i,j });

			}



			desenhar_fundo(camera);
			desenhar_mapa(camera);
			desenhar_alvo(jogador, camera, true);
			for (auto t : tiles_modificadas)
			{
				tile = { t.x * unidade,t.y * unidade ,unidade,unidade };
				SDL_SetRenderDrawColor(sistema_render, 0x00, 0x00, 0xFF, 0xFF);
				desenhar_alvo(tile, camera, true);
			}
			SDL_RenderPresent(sistema_render);
		}
		
		for (auto t : tiles_modificadas)
		{
			tile_map[t.x + t.y * colunas] = nova_tile;
		}
	}
	unidade = unidade_hold;
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
			if (tile_em(i, j) != '.')
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

	SDL_Rect crop = { 0,0,0,0 };	//não deve existir 
	SDL_FRect sprite_destino = { 0,0,0,0 };


	int tile_esquerda = (int)(p_camera.x / unidade);
	int tile_direita = (int)((p_camera.x + p_camera.w) / unidade);
	int tile_topo = (int)(p_camera.y / unidade);
	int tile_baixo = (int)((p_camera.y + p_camera.h) / unidade);

	char tile_t = ' ';




	static int animacao_rustica = 0;
	animacao_rustica++;
	if (animacao_rustica == 80)
		animacao_rustica = 0;
	for (int i = tile_esquerda ; i <= tile_direita ; i++)
		for (int j = tile_topo; j <= tile_baixo; j++)
		{

			tile_t = tile_em(i, j);


			/*if (tile_t != '.')
			{
				sprite_destino = { i * unidade , j * unidade , unidade , unidade };
				desenhar_alvo(sprite_destino, p_camera, true);
			}*/

			int resto = i % 2;
			switch (tile_t)
			{
			case 'z': 
				sprite_destino = { (i * unidade) - unidade * 0.25f , (j * unidade) - unidade * 0.25f ,unidade * 1.25f ,unidade *1.5f };
				crop = { 421,74,227,227 };
				tile_set[0].desenhar(&sprite_destino, &p_camera, &crop);

				sprite_destino = { (i * unidade) + unidade , (j * unidade) - unidade * 0.25f, unidade * 0.35f, 1.5f * unidade };
				crop = { 1841,74,78,227 };
				tile_set[0].desenhar(&sprite_destino, &p_camera, &crop);
				break; 

			case 'x':
				if (tile_em(i - 1, j) == '.')
					crop = { 0,0,396,396 };
				else if (tile_em(i + 1, j) == '.')
					crop = { 1246,0,396,396 };
				else
				{	
					switch (resto)
					{
					case  1:
						crop = { 396,0,396,396 };
						break;
					case 0:
						crop = { 792, 0 , 396,396 };
						break;
					}
				}
				sprite_destino = { (i * unidade)- unidade *0.15f, (j * unidade) -unidade*0.15f  , unidade*1.3f ,  unidade*1.3f };
				if (animacao_rustica < 40)
				{
					tile_set[1].desenhar(&sprite_destino, &p_camera, &crop);
				}
				else
					tile_set[5].desenhar(&sprite_destino, &p_camera, &crop);
				break;


			case 'c':
				sprite_destino = { (i * unidade), (j * unidade)  , unidade  ,  unidade };
				tile_set[4].desenhar(&sprite_destino, &p_camera, NULL);
				break;
			case 'v':
				sprite_destino = { (i * unidade) , (j * unidade)  , unidade ,  unidade };
				tile_set[2].desenhar(&sprite_destino, &p_camera, NULL);	
				break;
			case 'b':
			
				sprite_destino = { (i * unidade) , (j * unidade)  , unidade ,  unidade  };
				tile_set[3].desenhar(&sprite_destino, &p_camera, NULL);
				break;
			}
			
		}

}



//SOLIDO
char Cenario::tile_em(int x, int y)
{
	if (x < colunas && x >= 0 && y < linhas && y >= 0)
		return tile_map[colunas * y + x];
	else
		return '.';
}