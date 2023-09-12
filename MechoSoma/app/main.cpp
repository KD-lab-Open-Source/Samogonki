#include <SDL2/SDL.h>
#ifdef EMSCRIPTEN
#include <emscripten.h>
#define SOKOL_GLES3
#else
#define SOKOL_GLCORE33
#endif

#define SOKOL_GFX_IMPL
#include <sokol_gfx.h>

#include "xtool.h"

int xtSysQuantDisabled = 0;

#ifndef _WIN32
int main(int argc, char const *argv[]) {
#else
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
#endif
  SDL_Init(SDL_INIT_VIDEO);

  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
  SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

#ifdef EMSCRIPTEN
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
#else
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
#endif

  initclock();

  int prevID = 0;
  int id = xtInitApplication();
  XRuntimeObject *XObj = xtGetRuntimeObject(id);
  while (XObj) {
    XObj->Init(prevID);
    prevID = id;

    id = 0;

    int clockCnt = clocki();
    while (!id) {
      if (XObj->Timer) {
        int clockNow = clocki();
        int clockDelta = clockNow - clockCnt;
        if (clockDelta >= XObj->Timer) {
          clockCnt = clockNow - (clockDelta - XObj->Timer) % XObj->Timer;
          id = XObj->Quant();
        }
      } else {
        id = XObj->Quant();
      }

      if (!xtSysQuantDisabled) {
        XRec.Quant();
      }

      if (xtNeedExit()) {
        ErrH.Exit();
      }

#ifdef EMSCRIPTEN
       emscripten_sleep(0);
#endif
    }

    XObj->Finit();
    XObj = xtGetRuntimeObject(id);
  }
  xtDoneApplication();

  SDL_Quit();

  return 0;
}
