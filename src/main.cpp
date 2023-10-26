#include <SDL2/SDL.h>
#include <blend2d.h>

#include <iostream>

int main()
{
  int screen_w = 800, screen_h = 600;

  if(SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
    return 1;
  }

  int window_flags = 0;
  auto window = SDL_CreateWindow("SDL works!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
    screen_w, screen_h, window_flags);
  if(!window)
  {
    std::cerr << "Failed to open window: " << SDL_GetError() << std::endl;
    return 1;
  }

  auto renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if(!renderer)
  {
    std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
    return 1;
  }

  SDL_Texture* image_tex = nullptr;

  {
    SDL_Surface* image_src = SDL_CreateRGBSurfaceWithFormat(0, screen_w, screen_h, 32, SDL_PIXELFORMAT_ARGB8888);

    BLImage bl_image;
    bl_image.createFromData(image_src->w, image_src->h, BL_FORMAT_PRGB32, (void*)image_src->pixels, image_src->w * 4);

    BLContextCreateInfo bl_cci;
    bl_cci.threadCount = 4;

    BLContext ctx(bl_image, bl_cci);
    ctx.setHint(BL_CONTEXT_HINT_RENDERING_QUALITY, BL_RENDERING_QUALITY_ANTIALIAS);
    ctx.setCompOp(BL_COMP_OP_PLUS);

    ctx.setFillStyle(BLRgba32(0xF0FF0000));
    ctx.fillCircle(100.f, 100.f, 50.f);
    ctx.end();

    image_tex = SDL_CreateTextureFromSurface(renderer, image_src);
    SDL_FreeSurface(image_src);
  }
  if(!image_tex)
  {
    std::cerr << "Failed to create texture: " << SDL_GetError() << std::endl;
    return 1;
  }

  SDL_Event event;
  while(SDL_PollEvent(&event))
  {
    switch(event.type)
    {
    case SDL_QUIT:
      return 0;

    default:
      break;
    }
  }

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  SDL_RenderCopy(renderer, image_tex, nullptr, nullptr);

  SDL_RenderPresent(renderer);

  std::string input;
  std::getline(std::cin, input);

  SDL_DestroyTexture(image_tex);

  return 0;
}
