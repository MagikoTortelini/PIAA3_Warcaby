#include <SDL3/SDL.h>
#include <stdio.h>

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 640
#define BOARD_SIZE 8
#define TILE_SIZE (WINDOW_WIDTH / BOARD_SIZE)

typedef struct {
    SDL_Rect rect;
    int value;
} Tile;

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow("Szachownica 8x8", WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);

    Tile board[BOARD_SIZE][BOARD_SIZE];

    // Inicjalizacja szachownicy
    for (int y = 0; y < BOARD_SIZE; ++y) {
        for (int x = 0; x < BOARD_SIZE; ++x) {
            board[y][x].rect = (SDL_Rect){ x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE };
            board[y][x].value = y * BOARD_SIZE + x; // np. indeks 0-63
        }
    }

    SDL_Event e;
    int running = 1;

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) {
                running = 0;
            } else if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
                int mx = e.button.x;
                int my = e.button.y;
                int tx = mx / TILE_SIZE;
                int ty = my / TILE_SIZE;

                if (tx >= 0 && tx < BOARD_SIZE && ty >= 0 && ty < BOARD_SIZE) {
                    printf("Kliknieto pole (%d, %d) z wartoscia: %d\n", tx, ty, board[ty][tx].value);
                }
            }
        }

        // Rysowanie szachownicy
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        for (int y = 0; y < BOARD_SIZE; ++y) {
            for (int x = 0; x < BOARD_SIZE; ++x) {
                SDL_FRect r = {
                    static_cast<float>(board[y][x].rect.x),
                    static_cast<float>(board[y][x].rect.y),
                    static_cast<float>(board[y][x].rect.w),
                    static_cast<float>(board[y][x].rect.h)

                };


                if ((x + y) % 2 == 0) {
                    SDL_SetRenderDrawColor(renderer, 220, 220, 220, 255); // jasne
                } else {
                    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255); // ciemne
                }
                SDL_RenderFillRect(renderer, &r);

                float inset = TILE_SIZE * 0.25f;

                SDL_FRect innerSquare = {
                    board[y][x].rect.x + inset,
                    board[y][x].rect.y + inset,
                    board[y][x].rect.w - 2 * inset,
                    board[y][x].rect.h - 2 * inset
                };

                // Przykładowy warunek (np. tylko na 4 środkowych polach)
                if ((x >= 0 && x <= 7) && (y >= 1 && y <= 2)) {
                    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);  // niebieski
                    SDL_RenderFillRect(renderer, &innerSquare);
                }
            }

        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
