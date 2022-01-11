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
 * Initialise mixer
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
  /*// See CMakeLists.txt to see how the resources folder is copied from the root to the bin folder
  _font = TTF_OpenFont("resources/OpenSans-Regular.ttf", 18);

  SDL_Surface *textSurface = TTF_RenderText_Blended(_font, "Mouse Click and Drag Rect. Press [Escape] to Exit",
                                                    white);
  _headerText = SDL_CreateTextureFromSurface(_renderer, textSurface);

  _headerTextRect.x = _width / 2 - textSurface->w / 2;
  _headerTextRect.y = 0;
  _headerTextRect.w = textSurface->w;
  _headerTextRect.h = textSurface->h;

  SDL_FreeSurface(textSurface);

  // The value will probably be misreported as never used in CLion, however I think it should always be set
  textSurface = NULL;*/
}

/**
 * Setup a sample texture
 */
void setup_texture()
{
  /*_image = NULL;

  // Load image at specified path
  SDL_Surface *loadedSurface = IMG_Load("resources/floor.png");
  if (loadedSurface == NULL) {
    printf("[Error] Unable to load image : %s\n", SDL_GetError());
    exit(0);
  } else {
    _image = SDL_CreateTextureFromSurface(_renderer, loadedSurface);
    if (_image == NULL) {
      printf("[Error] Unable to create texture : %s\n", SDL_GetError());
    }

    SDL_FreeSurface(loadedSurface);
  }*/
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
 * Handles dragging the sample rectangle around. Demonstrates mouse motion events.
 * @param e
 */
void handle_mouse_drag(SDL_Event e)
{
  /*if (e.type == SDL_MOUSEBUTTONDOWN) {
    // Point where mouse button down occurs
    SDL_Point p = {.x = e.motion.x, .y = e.motion.y};

    if (SDL_PointInRect(&p, &_sampleRect)) {
      _inSampleRect = SDL_TRUE;
    }
  }

  if (e.type == SDL_MOUSEBUTTONUP && _inSampleRect == SDL_TRUE) {
    _inSampleRect = SDL_FALSE;
  }

  if (e.type == SDL_MOUSEMOTION && _inSampleRect == SDL_TRUE) {
    _sampleRect.x += e.motion.xrel;
    _sampleRect.y += e.motion.yrel;
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
      handle_mouse_drag(event);
    }

    // Blank out the renderer with all black
    SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 0);
    SDL_RenderClear(_renderer);

    // Note that all rendercopys are order specific.

    // Render the sample texture. We could use a source and/or destination rect to render to
    // but for now we'll just use it as a background
    SDL_RenderCopy(_renderer, _image, NULL, NULL);

    // Render the sample rectangle
    SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 1);
    SDL_RenderFillRect(_renderer, &_sampleRect);

    // Render sample text
    SDL_RenderCopy(_renderer, _headerText, NULL, &_headerTextRect);

    // Present to renderer
    SDL_RenderPresent(_renderer);
    SDL_Delay(10);
  }
}

/**
 * Main entry point
 * @return exit code
 */
int main()
{
    if (SDL_Init(SDL_INIT_VIDEO) == 0) {
        SDL_Window* window = NULL;
        SDL_Renderer* renderer = NULL;

        if (SDL_CreateWindowAndRenderer(800, 800, 0, &window, &renderer) == 0) {
            SDL_bool done = SDL_FALSE;

            while (!done) {
                SDL_Event event;

                SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
                SDL_RenderClear(renderer);
                for(int i=0; i<6; i++){

                }
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
                SDL_RenderDrawLine(renderer, 50, 50, (550), 50);
                SDL_RenderDrawLine(renderer, 50, 50, 50, 550);
                SDL_RenderDrawLine(renderer, 50, 550, 550, 550);
                SDL_RenderDrawLine(renderer, 50, 550, 50, 550);
                SDL_RenderPresent(renderer);
                /*SDL_RenderDrawLine(renderer, 320, 200, 300, 240);
                SDL_RenderDrawLine(renderer, 300, 240, 340, 240);
                SDL_RenderDrawLine(renderer, 340, 240, 320, 200);
                SDL_RenderPresent(renderer);*/

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
    SDL_Quit();
    return 0;
}