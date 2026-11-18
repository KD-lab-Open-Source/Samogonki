#ifdef GPX
#include <c/gamepix.h>
#endif
// Without SDL_main.h, SDL 3 leaves main() to the game.
#include <SDL3/SDL.h>
#include "filesystem.h"

#ifdef EMSCRIPTEN
#include <emscripten.h>
#endif

#ifdef STEAM_VERSION
#include <algorithm>
#include <array>
#include <string_view>
#include <utility>

#include <steam/steam_api.h>
#endif

#include "xtool.h"

int xtSysQuantDisabled = 0;

namespace {

    struct LoopState {
        int prevId;
        int id;
        bool xObjInit;
        XRuntimeObject *XObj;
        int clockCnt;
    };

    LoopState state;

    void loop() {
        if (state.xObjInit) {
            state.XObj->Init(state.prevId);
            state.prevId = state.id;
            state.id = 0;
            state.xObjInit = false;
            state.clockCnt = clocki();
        } else if (!state.id) {
            if (state.XObj->Timer) {
                int clockNow = clocki();
                int clockDelta = clockNow - state.clockCnt;
                if (clockDelta >= state.XObj->Timer) {
                    state.clockCnt = clockNow - (clockDelta - state.XObj->Timer) % state.XObj->Timer;
                    state.id = state.XObj->Quant();
                }
            } else {
                state.id = state.XObj->Quant();
            }

            if (!xtSysQuantDisabled) {
                XRec.Quant();
            }

            if (xtNeedExit()) {
                ErrH.Exit();
            }
        } else {
            state.XObj->Finit();
            state.XObj = xtGetRuntimeObject(state.id);
            state.xObjInit = true;
#ifdef EMSCRIPTEN
            EM_ASM((
                if (Module.layers) {
                    Module.layers.gameQuantId($0);
                }
            ), state.id);
#endif
        }

#ifdef GPX
        gpx()->async()->runNextTask();
#endif
    };
}

#ifdef GPX

int game_main(int argc, char const *argv[]) {
  set_signal_handler();
  run_crash_test_from_env();
  localization::setLanguage(gpx()->sys()->getLanguage().c_str());
#else
int main(int argc, char const *argv[]) {
  set_signal_handler();
  run_crash_test_from_env();

  #ifdef STEAM_VERSION
  if (!SteamAPI_Init()) {
    return 1;
  }

  using LanguagePair = std::pair<std::string_view, std::string_view>;
  constexpr std::array<LanguagePair, 6> map{
    LanguagePair("czech", "cz"),
    LanguagePair("english", "en"),
    LanguagePair("french", "fr"),
    LanguagePair("italian", "it"),
    LanguagePair("lithuanian", "lt"),
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
#endif
#ifdef _WIN32
  // SDL 2 left the game DPI-unaware, so Windows scales its window on a HiDPI display.
  // SDL 3 is DPI-aware by default, which would make the window smaller there and change
  // the mouse mapping; keep the SDL 2 behaviour until that is done deliberately.
  SDL_SetHint(SDL_HINT_WINDOWS_DPI_AWARENESS, "unaware");
#endif
  if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK)) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_Init(): %s\n", SDL_GetError());
  }
  // The window and its GL context are set up by graphics::Display.

#ifdef EMSCRIPTEN
  SDL_SetHint("SDL_EMSCRIPTEN_ASYNCIFY", "0");
  SDL_SetHint("SDL_HINT_TOUCH_MOUSE_EVENTS", "1");
  SDL_SetHint("SDL_HINT_MOUSE_TOUCH_EVENTS", "0");
#endif

  initclock();

  state.prevId = 0;
  state.id = xtInitApplication();
  state.xObjInit = true;
  state.XObj = xtGetRuntimeObject(state.id);

#ifdef EMSCRIPTEN
  emscripten_set_main_loop(loop, 0, true);
#else
    while (state.XObj) {
        loop();
    }
#endif

  xtDoneApplication();

  SDL_Quit();

  return 0;
}
