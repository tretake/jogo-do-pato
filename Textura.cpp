#include "Textura.h"

bool colisao(SDL_FRect a, SDL_FRect b)
{
	float b_right = b.x + b.w;
	float b_bottom = b.y + b.h;

	float a_right = a.x + a.w;
	float a_bottom = a.y + a.h;

	if (((a.x < b_right) && (a_right > b.x)) && (a.y< b_bottom && a_bottom > b.y))
	{
		return true;
	}
	else
	{
		return false;
	}
}


int wrap_leght = 358;

float Textura::const_conversao_x = 0;
float Textura::const_conversao_y = 0;
SDL_Renderer* Textura::trender = NULL;
SDL_Window* Textura::tjanela = NULL;



Textura::Textura()
{

}

Textura::~Textura()
{
	free();
}



void Textura::desenhar_alvo()
{
	SDL_FRect resolucao_convert = { alvo.x * const_conversao_x,alvo.y * const_conversao_y,alvo.w * const_conversao_x, alvo.h * const_conversao_y };
	SDL_SetRenderDrawColor(trender, 0xFF, 0x00, 0x00, 0xFF);
	SDL_RenderDrawRectF(trender, &resolucao_convert);
}

void Textura::desenhar_alvo_cheio()
{
	SDL_FRect resolucao_convert = { alvo.x * const_conversao_x,alvo.y * const_conversao_y,alvo.w * const_conversao_x, alvo.h * const_conversao_y };
	SDL_SetRenderDrawColor(trender, 0xFF, 0x00, 0x00, 0xFF);
	SDL_RenderFillRectF(trender, &resolucao_convert);
}

bool Textura::carregar_textura(std::string path)
{

	imagem = IMG_LoadTexture(trender, path.c_str());
	if (imagem == NULL)
	{
		printf("falhou ao carregar textura %s , %s",path, SDL_GetError());
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

bool Textura::carregar_texto(std::string texto)
{
	SDL_Surface* holder;

	holder = TTF_RenderUTF8_Solid_Wrapped(tfonte, texto.c_str(), { 0,0,0,0 }, wrap_leght);
	if (holder == NULL)
	{
		printf("falhou ao gerar o texto, erro: %s \n", TTF_GetError());
		return EXIT_FAILURE;
	}
	else
	{
		imagem = SDL_CreateTextureFromSurface(trender, holder);

		if (imagem == NULL)
		{
			printf("falhou em criar textura da superficie , erro %s \n", SDL_GetError());
			return EXIT_FAILURE;
		}
	}
	SDL_FreeSurface(holder);
	return EXIT_SUCCESS;
}

bool Textura::free()
{
	if (imagem != NULL)
	{
		SDL_DestroyTexture(imagem);
		return true;
	}
	else
		return false;
}

void Textura::update_const_conversao()
{
	int largura_tela = 0;
	int altura_tela = 0;

	SDL_GetWindowSize(tjanela, &largura_tela, &altura_tela);			//800
	const_conversao_x = (static_cast<float>(largura_tela) / 1600.00f);	//0.5
	const_conversao_y = (static_cast<float>(altura_tela) / 900.00f);
}

void Textura::desenhar(SDL_Rect* crop ,SDL_FRect* p_alvo, bool flip)
{
	if (alvo.w == 0)
		SDL_RenderCopyExF(trender, imagem, crop, NULL, 0, NULL, SDL_FLIP_NONE);
	else
	{
		SDL_FRect* destino = (p_alvo == NULL) ? &alvo : p_alvo;
		SDL_FRect resolucao_convert = { destino->x * const_conversao_x,destino->y * const_conversao_y,destino->w * const_conversao_x, destino->h * const_conversao_y };

		if ( SDL_RenderCopyExF(trender, imagem, crop, &resolucao_convert,0,NULL, (flip) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE ) )
		{
			printf("falhou ao desenhar textura, erro: %s\n", TTF_GetError());
		}
	}
}

bool Textura::setup(SDL_Renderer* prender, SDL_Window* pjanela)
{
	trender = prender;
	if (trender == NULL)
	{
		printf("falhou ao setar um render a classe Textura");
		return EXIT_FAILURE;
	}

	tjanela = pjanela;

	update_const_conversao();

	return EXIT_SUCCESS;
}


bool Textura::carregar_fonte(int size, std::string path)
{
	tfonte = TTF_OpenFont(path.c_str(), size);

	if (tfonte == NULL)
	{
		printf("falhou ao carregar a fonte, erro: %s\n", TTF_GetError());
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
