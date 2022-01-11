#include <printf.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

// SDL Window
SDL_Window *_window;

// SDL Renderer to draw to
SDL_Renderer *_renderer;

// Our default font
TTF_Font *_font;

// Easy access to a useful color
SDL_Color white = {255, 255, 255};

// Window size
int _width = 640;
int _height = 480;

// Our sample header texture and where we're drawing our header to
SDL_Rect _headerTextRect;
SDL_Texture *_headerText;

// Our sample texture
SDL_Texture *_image;

// Our sample rectangle that we can drag around the viewport
SDL_Rect _sampleRect = {.x = 10, .y = 10, .w = 100, .h = 100};
SDL_bool _inSampleRect = SDL_FALSE;

// Our sample 'music'
Mix_Music *_music = NULL;

/**
 * Initialise SDL2 and output some useful display info
 */
void init_sdl()
{
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    printf("[Error] SDL Init : %s \n", SDL_GetError());
  } else {
    printf("SDL INITIALISED\n");
    SDL_DisplayMode dm;
    SDL_GetCurrentDisplayMode(0, &dm);

    printf("Display mode is %dx%dpx @ %dhz\n", dm.w, dm.h, dm.refresh_rate);
  }
}

/**
 * Initialise an SDL Window and Renderer
 *
 * This uses SDL_CreateWindowAndRenderer. They can alternatively be created separately. See SDL2 Docs
 */
void init_window_and_renderer()
{
  if (SDL_CreateWindowAndRenderer(_width, _height, SDL_WINDOW_SHOWN, &_window, &_renderer) != 0) {
    printf("[Error] Creating Window and Renderer: %s\n", SDL_GetError());
    exit(0);
  } else {
    printf("Created Window and Renderer %dx%d\n", _width, _height);
  }
}

/**
 * Initialise TTF
 */
void init_ttf()
{
  TTF_Init();
}

/**
 * Demarre audio
 */
void init_audio()
{
  if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) != 0) {
    printf("[Error] Error Initialising Audio : %s\n", SDL_GetError());
  } else {
    printf("Audio Initialised\n");
  }
}

/**
 * Setup a sample header text
 */
void setup_header_text()
{
  // See CMakeLists.txt to see how the resources folder is copied from the root to the bin folder
  _font = TTF_OpenFont("resources/OpenSans-Regular.ttf", 5);

  SDL_Surface *textSurface = TTF_RenderText_Blended(_font, "Jeu de l'Othello",
                                                    white);
  _headerText = SDL_CreateTextureFromSurface(_renderer, textSurface);

  _headerTextRect.x = _width / 2 - textSurface->w / 2;
  _headerTextRect.y = 0;
  _headerTextRect.w = textSurface->w;
  _headerTextRect.h = textSurface->h;

  SDL_FreeSurface(textSurface);

  // The value will probably be misreported as never used in CLion, however I think it should always be set
  textSurface = NULL;
}

/**
 * Setup a window app icon
 */
void setup_window_icon()
{
  SDL_Surface *iconSurface;
  iconSurface = IMG_Load("resources/appicon.jpg");

  // The icon requires the window pointer NOT the renderer
  SDL_SetWindowIcon(_window, iconSurface);

  // ...and can now free the appicon surface
  SDL_FreeSurface(iconSurface);
}

/**
 * Play a sample audio file
 */
void play_audio()
{
  /*_music = Mix_LoadMUS("resources/sound.ogg");
  if (Mix_PlayMusic(_music, -1) != 0) {
    printf("[Error] Could not play music : %s", Mix_GetError());
  }*/
}

/**
 * The main game loop. Continues to loop until Escape or an SDL_Quit event occurs
 */
void main_loop()
{
  SDL_bool loop = SDL_TRUE;
  SDL_Event event;

  while (loop) {
    // Allow quiting with escape key by polling for pending events
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        loop = SDL_FALSE;
      } else if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
          case SDLK_ESCAPE:
            loop = SDL_FALSE;
            break;
          default:
            loop = SDL_TRUE;
        }
      }
    }
    // Blank out the renderer with all black
    SDL_SetRenderDrawColor(_renderer, 0, 255, 0, 255);
    SDL_RenderClear(_renderer);

    // Note that all rendercopys are order specific.

    // Render the sample texture. We could use a source and/or destination rect to render to
    // but for now we'll just use it as a background
    SDL_RenderCopy(_renderer, _image, NULL, NULL);

    // Render the sample rectangle
    SDL_SetRenderDrawColor(_renderer, 0, 255, 0, 1);
    SDL_RenderFillRect(_renderer, &_sampleRect);

    // Render sample text
    SDL_RenderCopy(_renderer, _headerText, NULL, &_headerTextRect);

    // Present to renderer
    SDL_RenderPresent(_renderer);
    SDL_Delay(10);
  }
}

/**
 * Dessine un cercle
 */
void dessiner_pion(SDL_Renderer * renderer, int32_t centreX, int32_t centreY, int32_t radius){

    const int32_t diameter = (radius * 2);

    int32_t x = (radius - 1);
    int32_t y = 0;
    int32_t tx = 1;
    int32_t ty = 1;
    int32_t error = (tx - diameter);

    while (x >= y)
    {
        //  Each of the following renders an octant of the circle
        SDL_RenderDrawPoint(renderer, centreX + x, centreY - y);

        SDL_RenderDrawPoint(renderer, centreX + x, centreY + y);
        SDL_RenderDrawPoint(renderer, centreX - x, centreY - y);
        SDL_RenderDrawPoint(renderer, centreX - x, centreY + y);
        SDL_RenderDrawPoint(renderer, centreX + y, centreY - x);
        SDL_RenderDrawPoint(renderer, centreX + y, centreY + x);
        SDL_RenderDrawPoint(renderer, centreX - y, centreY - x);
        SDL_RenderDrawPoint(renderer, centreX - y, centreY + x);

        if (error <= 0)
        {
            ++y;
            error += ty;
            ty += 2;
        }

        if (error > 0)
        {
            --x;
            tx += 2;
            error += (tx - diameter);
        }
    }
}
typedef struct Case{
    int x;
    int y;
}Case;

/**
 * Creer le plateau de jeu
 */
void dessin_plateau(){
    int x = 50;

    if (SDL_Init(SDL_INIT_VIDEO) == 0) {
        SDL_Window* window = NULL;
        SDL_Renderer* renderer = NULL;

        if (SDL_CreateWindowAndRenderer(740, 740, 0, &window, &renderer) == 0) {
            SDL_bool done = SDL_FALSE;

            while (!done) {
                SDL_Event event;

                SDL_SetRenderDrawColor(renderer, 0, 200, 150, SDL_ALPHA_OPAQUE);
                SDL_RenderClear(renderer);

                SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
                SDL_RenderDrawLine(renderer, 50, 50, 690, 50);
                SDL_RenderDrawLine(renderer, 50, 130, 690, 130);
                SDL_RenderDrawLine(renderer, 50, 210, 690, 210);
                SDL_RenderDrawLine(renderer, 50, 290, 690, 290);
                SDL_RenderDrawLine(renderer, 50, 370, 690, 370);
                SDL_RenderDrawLine(renderer, 50, 450, 690, 450);
                SDL_RenderDrawLine(renderer, 50, 530, 690, 530);
                SDL_RenderDrawLine(renderer, 50, 610, 690, 610);

                SDL_RenderDrawLine(renderer, 50, 50, 50, 690);
                SDL_RenderDrawLine(renderer, 130, 50, 130, 690);
                SDL_RenderDrawLine(renderer, 210, 50, 210, 690);
                SDL_RenderDrawLine(renderer, 290, 50, 290, 690);
                SDL_RenderDrawLine(renderer, 370, 50, 370, 690);
                SDL_RenderDrawLine(renderer, 450, 50, 450, 690);
                SDL_RenderDrawLine(renderer, 530, 50, 530, 690);
                SDL_RenderDrawLine(renderer, 610, 50, 610, 690);


                SDL_RenderDrawLine(renderer, 50, 690, 690, 690);
                SDL_RenderDrawLine(renderer, 690, 50, 690, 690);

                dessiner_pion(renderer, 90, 90, 30);

                SDL_RenderPresent(renderer);

                while (SDL_PollEvent(&event)) {
                    if (event.type == SDL_QUIT) {
                        done = SDL_TRUE;
                    }
                }
            }
        }

        if (renderer) {
            SDL_DestroyRenderer(renderer);
        }
        if (window) {
            SDL_DestroyWindow(window);
        }
    }
}

/**
 * Main entry point
 * @return exit code
 */
int main()
{
    Case Tiles[8][8];
    dessin_plateau();
    SDL_Quit();
    return 0;
}