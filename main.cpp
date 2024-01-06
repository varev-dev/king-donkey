#define _USE_MATH_DEFINES
#include<math.h>
#include<stdio.h>
#include<string.h>

extern "C" {
#include"./SDL2-2.0.10/include/SDL.h"
#include"./SDL2-2.0.10/include/SDL_main.h"
}

#include "Game.h"
#include "DEFINED_VALUES.h"

// narysowanie napisu txt na powierzchni screen, zaczynajπc od punktu (x, y)
// charset to bitmapa 128x128 zawierajπca znaki
// draw a text txt on surface screen, starting from the point (x, y)
// charset is a 128x128 bitmap containing character images
void DrawString(SDL_Surface *screen, int x, int y, const char *text,
                SDL_Surface *charset) {
	int px, py, c;
	SDL_Rect s, d;
	s.w = 8;
	s.h = 8;
	d.w = 8;
	d.h = 8;
	while(*text) {
		c = *text & 255;
		px = (c % 16) * 8;
		py = (c / 16) * 8;
		s.x = px;
		s.y = py;
		d.x = x;
		d.y = y;
		SDL_BlitSurface(charset, &s, screen, &d);
		x += 8;
		text++;
		};
	};

// narysowanie na ekranie screen powierzchni sprite w punkcie (x, y)
// (x, y) to punkt úrodka obrazka sprite na ekranie
// draw a surface sprite on a surface screen in point (x, y)
// (x, y) is the center of sprite on screen
void DrawSurface(SDL_Surface *screen, SDL_Surface *sprite, int x, int y) {
	SDL_Rect dest;
	dest.x = x - sprite->w / 2;
	dest.y = y - sprite->h / 2;
	dest.w = sprite->w;
	dest.h = sprite->h;
	SDL_BlitSurface(sprite, NULL, screen, &dest);
};


// rysowanie pojedynczego pixela
// draw a single pixel
void DrawPixel(SDL_Surface *surface, int x, int y, Uint32 color) {
	int bpp = surface->format->BytesPerPixel;
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
	*(Uint32 *)p = color;
};


// rysowanie linii o d≥ugoúci l w pionie (gdy dx = 0, dy = 1) 
// bπdü poziomie (gdy dx = 1, dy = 0)
// draw a vertical (when dx = 0, dy = 1) or horizontal (when dx = 1, dy = 0) line
void DrawLine(SDL_Surface *screen, int x, int y, int l, int dx, int dy, Uint32 color) {
	for(int i = 0; i < l; i++) {
		DrawPixel(screen, x, y, color);
		x += dx;
		y += dy;
	};
};


// rysowanie prostokπta o d≥ugoúci bokÛw l i k
// draw a rectangle of size l by k
void DrawRectangle(SDL_Surface *screen, int x, int y, int l, int k,
                   Uint32 outlineColor, Uint32 fillColor) {
	int i;
	DrawLine(screen, x, y, k, 0, 1, outlineColor);
	DrawLine(screen, x + l - 1, y, k, 0, 1, outlineColor);
	DrawLine(screen, x, y, l, 1, 0, outlineColor);
	DrawLine(screen, x, y + k - 1, l, 1, 0, outlineColor);
	for(i = y + 1; i < y + k - 1; i++)
		DrawLine(screen, x + 1, i, l - 2, 1, 0, fillColor);
};


// main
#ifdef __cplusplus
extern "C"
#endif

void clearAll(SDL_Surface *charset, SDL_Surface *screen, SDL_Texture *scrtex,
		SDL_Window *window, SDL_Renderer *renderer) {
	SDL_FreeSurface(charset);
	SDL_FreeSurface(screen);
	SDL_DestroyTexture(scrtex);
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}

int main(int argc, char **argv) {
	int t1, t2, quit, frames, rc;
	double delta, worldTime, fpsTimer, fps, distance, mv_speed;
	SDL_Event event;
	SDL_Surface *screen, *charset;
	SDL_Texture *scrtex;
	SDL_Window *window;
	SDL_Renderer *renderer;

	if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("SDL_Init error: %s\n", SDL_GetError());
		return 1;
	}

	// tryb pe≥noekranowy / fullscreen mode
	//rc = SDL_CreateWindowAndRenderer(0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP, &window, &renderer);
	rc = SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &renderer);

	if(rc != 0) {
		SDL_Quit();
		printf("SDL_CreateWindowAndRenderer error: %s\n", SDL_GetError());
		return 1;
	};
	
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	SDL_SetWindowTitle(window, "King Donkey");


	screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32,
	                              0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

	scrtex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
	                           SDL_TEXTUREACCESS_STREAMING,
	                           SCREEN_WIDTH, SCREEN_HEIGHT);


	// wy≥πczenie widocznoúci kursora myszy
	SDL_ShowCursor(SDL_DISABLE);

	// wczytanie obrazka cs8x8.bmp
	charset = SDL_LoadBMP("./bmp/cs8x8.bmp");
	if(charset == NULL) {
		printf("SDL_LoadBMP(cs8x8.bmp) error: %s\n", SDL_GetError());
		clearAll(screen, charset, scrtex, window, renderer);
		return 1;
	};
	SDL_SetColorKey(charset, true, 0x000000);

	SDL_Surface* platform_bmp = SDL_LoadBMP("./bmp/platform16x16.bmp");
	if (platform_bmp == NULL) {
		printf("SDL_LoadBMP(platform16x16.bmp) error: %s\n", SDL_GetError());
		clearAll(screen, charset, scrtex, window, renderer);
		return 1;
	};

	SDL_Surface* ladder_bmp = SDL_LoadBMP("./bmp/ladder16x16.bmp");
	if (ladder_bmp == NULL) {
		printf("SDL_LoadBMP(ladder16x16.bmp) error: %s\n", SDL_GetError());
		clearAll(screen, charset, scrtex, window, renderer);
		return 1;
	};

	SDL_Surface* player_bmp = SDL_LoadBMP("./bmp/player32x48.bmp");
	if (player_bmp == NULL) {
		printf("SDL_LoadBMP(player16x24.bmp) error: %s\n", SDL_GetError());
		clearAll(screen, charset, scrtex, window, renderer);
		return 1;
	};

	char text[128];
	int czarny = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
	int zielony = SDL_MapRGB(screen->format, 0x00, 0xFF, 0x00);
	int czerwony = SDL_MapRGB(screen->format, 0xFF, 0x00, 0x00);
	int niebieski = SDL_MapRGB(screen->format, 0x11, 0x11, 0xCC);

	t1 = SDL_GetTicks();

	frames = 0;
	fpsTimer = 0;
	fps = 0;
	quit = 0;
	worldTime = 0;
	distance = 0;
	mv_speed = 400;

	GameObject* floor = new GameObject(platform_bmp, "Floor 1", 0, SCREEN_HEIGHT - platform_bmp->h, 
						SCREEN_WIDTH, platform_bmp->h);
	GameObject* ladder = new GameObject(ladder_bmp, "Ladder 1", 48, SCREEN_HEIGHT / 2,
						ladder_bmp->w, SCREEN_HEIGHT / 2 - platform_bmp->h);
	Movable* player = new Movable(player_bmp, "Player", 0, 0, player_bmp->w, player_bmp->h);
	
	while(!quit) {
		t2 = SDL_GetTicks();
		delta = (t2 - t1) * 0.001;
		t1 = t2;

		worldTime += delta;
		distance = mv_speed * delta;

		SDL_FillRect(screen, NULL, czarny);

		fpsTimer += delta;
		if(fpsTimer > 0.5) {
			fps = frames * 2;
			frames = 0;
			fpsTimer -= 0.5;
		};
		
		// tekst informacyjny / info text
		DrawRectangle(screen, 4, 4, SCREEN_WIDTH - 8, 36, czerwony, niebieski);
		//            "template for the second project, elapsed time = %.1lf s  %.0lf frames / s"
		sprintf(text, "In game time = %.1lf s, %.0lf fps", worldTime, fps);
		DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 10, text, charset);
		//	      "Esc - exit, \030 - faster, \031 - slower"
		//sprintf(text, "Esc - wyjscie, \030 - przyspieszenie, \031 - zwolnienie");
		//DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 26, text, charset);
		floor->printOnScreen(screen);
		ladder->printOnScreen(screen);
		player->printOnScreen(screen);

		SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
//		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, scrtex, NULL, NULL);
		SDL_RenderPresent(renderer);

		if (!Collider::CollisionBetweenMovableAndObject(*floor, *player) &&
			!Collider::CollisionBetweenMovableAndObject(*ladder, *player)) {
			player->move(SDLK_DOWN, 2, *ladder);
		}

		while(SDL_PollEvent(&event)) {
			switch(event.type) {
				case SDL_KEYDOWN:
					if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_DOWN) {
						if (Collider::IsMovePossibleOnAxisY(*ladder, *player))
							player->move(event.key.keysym.sym, distance, *ladder);
					} else if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_RIGHT) {
						if (Collider::IsMovePossibleOnAxisX(*floor, *player))
							player->move(event.key.keysym.sym, distance, *floor);
					} else if (event.key.keysym.sym == SDLK_ESCAPE)
						quit = 1;
					break;
				case SDL_KEYUP:
					break;
				case SDL_QUIT:
					quit = 1;
					break;
			};
		};
		frames++;
	};

	// zwolnienie powierzchni / freeing all surfaces
	clearAll(screen, charset, scrtex, window, renderer);
	return 0;
};
