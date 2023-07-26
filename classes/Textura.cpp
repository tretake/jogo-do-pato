#include "Textura.h"

int iniciar_SDL(SDL_Window*& p_janela, SDL_Renderer*& p_render)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		printf("falhou ao iniciar SDL, erro : %s \n", SDL_GetError());
		return EXIT_FAILURE;
	}
	else
	{
		p_janela = SDL_CreateWindow("pato", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1600, 900, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
		if (p_janela == NULL)
		{
			printf("falhou ao criar a janela, erro : %s \n", SDL_GetError());
			return EXIT_FAILURE;
		}
		else
		{

			p_render = SDL_CreateRenderer(p_janela, -1, SDL_RENDERER_ACCELERATED);

			if (p_render == NULL)
			{
				printf("falou ao criar o render, erro %s \n", SDL_GetError());
				return EXIT_FAILURE;
			}

			if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
			{
				printf("falhou ao iniciar SDL_image, error %s \n", IMG_GetError());
			}
			if (TTF_Init() == -1)
			{
				printf("falhou ao inicializar SDL_ttf , error: %s \n", TTF_GetError());
				return EXIT_FAILURE;
			}

			return EXIT_SUCCESS;

		}
	}
}
void fechar_SDL(SDL_Window*& p_janela, SDL_Renderer*& p_render)
{
	SDL_DestroyRenderer(p_render);
	SDL_DestroyWindow(p_janela);
	SDL_Quit();

}

bool colisao(SDL_FRect a, SDL_FRect b)
{
	float b_right = b.x + b.w;
	float b_bottom = b.y + b.h;

	float a_right = a.x + a.w;
	float a_bottom = a.y + a.h;

	if (((a.x < b_right) && (a_right > b.x)) && (a.y < b_bottom && a_bottom > b.y))
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



// essas fun�oes tem q virar fun�oes do sistema
void Textura::desenhar_alvo(SDL_FRect alvo)
{
	SDL_FRect resolucao_convert = { alvo.x * const_conversao_x,alvo.y * const_conversao_y,alvo.w * const_conversao_x, alvo.h * const_conversao_y };
	SDL_SetRenderDrawColor(trender, 0xFF, 0x00, 0x00, 0xFF);
	SDL_RenderDrawRectF(trender, &resolucao_convert);
}
void Textura::desenhar_alvo_cheio(SDL_FRect alvo)
{
	SDL_FRect resolucao_convert = { alvo.x * const_conversao_x,alvo.y * const_conversao_y,alvo.w * const_conversao_x, alvo.h * const_conversao_y };
	SDL_SetRenderDrawColor(trender, 0xFF, 0x00, 0x00, 0xFF);
	SDL_RenderFillRectF(trender, &resolucao_convert);
}
// essas fun�oes tem q virar fun�oes do sistema


bool Textura::carregar_textura(std::string path)
{

	imagem = IMG_LoadTexture(trender, path.c_str());
	if (imagem == NULL)
	{
		printf("falhou ao carregar textura %s , %s",path.c_str(), SDL_GetError());
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

void Textura::desenhar( SDL_FRect* p_destino, SDL_Rect* crop, bool flip)
{
	if (p_destino == NULL)
		SDL_RenderCopyExF(trender, imagem, crop, NULL, 0, NULL, SDL_FLIP_NONE);
	else
	{
		SDL_FRect resolucao_convert = { p_destino->x * const_conversao_x,p_destino->y * const_conversao_y,p_destino->w * const_conversao_x, p_destino->h * const_conversao_y };
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
