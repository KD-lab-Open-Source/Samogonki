#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include "filesystem.h"

#ifdef EMSCRIPTEN
#include <emscripten.h>
#define SOKOL_GLES3
#else
#define SOKOL_GLCORE33
#endif

#ifdef STEAM_VERSION
#include <algorithm>
#include <array>
#include <string_view>
#include <utility>

#include <steam/steam_api.h>
#endif

#define SOKOL_GFX_IMPL
#include <sokol_gfx.h>

#include "xtool.h"

int xtSysQuantDisabled = 0;

int main(int argc, char const *argv[]) {
  #ifdef STEAM_VERSION
  if (!SteamAPI_Init()) {
    return 1;
  }

  using LanguagePair = std::pair<std::string_view, std::string_view>;
  constexpr std::array<LanguagePair, 4> map{
    LanguagePair("english", "en"),
    LanguagePair("czech", "cz"),
    LanguagePair("italian", "it"),
    LanguagePair("russian", "ru")
  };

  auto language = SteamApps()->GetCurrentGameLanguage();
  const auto p = std::find_if(map.begin(), map.end(), [&language](const auto &e) {
    return e.first == language;
  });
  if (p == map.end()) {
    return 1;
  }

  localization::setLanguage(p->second.data());
  #else
  for (int i = 1; i < argc - 1; ++i) {
      if (strcmp(argv[i], "-lang") == 0 && strlen(argv[i + 1]) == 2) {
          localization::setLanguage(argv[i + 1]);
          break;
      }
  }
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
