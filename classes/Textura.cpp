#include "Textura.h"

float const_conversao_x = 0;
float const_conversao_y = 0;

SDL_Renderer* sistema_render = NULL;
SDL_Window* sistema_janela = NULL;

SDL_FRect sistema_camera = { 0,0,1600,900 };

void update_const_conversao()
{
	int largura_tela = 0;
	int altura_tela = 0;

	SDL_GetWindowSize(sistema_janela, &largura_tela, &altura_tela);			//800
	const_conversao_x = (static_cast<float>(largura_tela) / 1600.00f);	//0.5
	const_conversao_y = (static_cast<float>(altura_tela) / 900.00f);
}



void desenhar_alvo(SDL_FRect hitbox , SDL_FRect p_camera , bool preechido)	//fazer zoom out da camera
{
	SDL_FRect resolucao_convert = { (hitbox.x - p_camera.x) * const_conversao_x, (hitbox.y - p_camera.y) * const_conversao_y,(hitbox.w) * const_conversao_x, (hitbox.h) * const_conversao_y };
	SDL_SetRenderDrawColor(sistema_render, 0xFF, 0x00, 0x00, 0xFF);
	if(preechido == false)
		SDL_RenderDrawRectF(sistema_render, &resolucao_convert);
	else
		SDL_RenderFillRectF(sistema_render, &resolucao_convert);
}



void Textura::desenhar( SDL_FRect* p_destino , SDL_Rect* crop, bool flip) //fazer zoom out da camera
{
	if (p_destino == NULL)
		SDL_RenderCopyExF(sistema_render, imagem, crop, NULL, 0, NULL, SDL_FLIP_NONE);
	else
	{
		SDL_FRect resolucao_convert = { (p_destino->x - sistema_camera.x )* const_conversao_x, (p_destino->y - sistema_camera.y) * const_conversao_y,p_destino->w * const_conversao_x, p_destino->h * const_conversao_y };
		if ( SDL_RenderCopyExF(sistema_render, imagem, crop, &resolucao_convert,0,NULL, (flip) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE ) )
		{
			printf("falhou ao desenhar textura, erro: %s\n", TTF_GetError());
		}
	}
}

void Textura::desenhar_estatico(SDL_FRect* p_destino, SDL_Rect* crop, bool flip) //fazer zoom out da camera
{
	if (p_destino == NULL)
		SDL_RenderCopyExF(sistema_render, imagem, crop, NULL, 0, NULL, SDL_FLIP_NONE);
	else
	{
		SDL_FRect resolucao_convert = { (p_destino->x ) * const_conversao_x, (p_destino->y ) * const_conversao_y,p_destino->w * const_conversao_x, p_destino->h * const_conversao_y };
		if (SDL_RenderCopyExF(sistema_render, imagem, crop, &resolucao_convert, 0, NULL, (flip) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE))
		{
			printf("falhou ao desenhar textura, erro: %s\n", TTF_GetError());
		}
	}
}









//SOLIDO
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
int iniciar_SDL(SDL_Window*& p_janela, SDL_Renderer*& p_render)
{



	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		printf("falhou ao iniciar SDL, erro : %s \n", SDL_GetError());
		return EXIT_FAILURE;
	}
	else
	{	//1600 900		resolucao inicial antiga
		p_janela = SDL_CreateWindow("pato", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 450, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
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

			update_const_conversao();
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


Textura::Textura()
{

}

Textura::~Textura()
{
	free();
}

bool Textura::carregar_textura(std::string path)
{

	imagem = IMG_LoadTexture(sistema_render, path.c_str());
	if (imagem == NULL)
	{
		printf("falhou ao carregar textura %s , %s", path.c_str(), SDL_GetError());
		return EXIT_FAILURE;
	}
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
bool Textura::carregar_texto(std::string texto , int wrap_leght)
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
		imagem = SDL_CreateTextureFromSurface(sistema_render, holder);

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

