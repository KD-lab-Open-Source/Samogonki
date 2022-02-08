#include "app.h"

#define SOKOL_GLCORE33
#define SOKOL_IMPL

#include <sokol_app.h>
#include <sokol_gfx.h>

sapp_desc sokol_main(int argc, char* argv[]) {
  return (sapp_desc){
      .init_cb = onInit,
      .frame_cb = onFrame,
      .cleanup_cb = onCleanup,
      .event_cb = onEvent,
      .width = 1024,
      .height = 768,
  };
}