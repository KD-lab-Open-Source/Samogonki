//
// Created by caiiiycuk on 07.02.22.
//

#include <sokol_app.h>

#include <memory>

#include "KEYS.H"
#include "renderer.h"
#include "xgraph.h"
#include "xtool.h"

int _id;
int _previous_id;
XRuntimeObject* _current_rto;

std::unique_ptr<graphics::Renderer> graphics::Renderer::shared;

graphics::RendererInterface& graphics::get_renderer() { return *Renderer::shared; }

void onInit() {
  auto context = sg_context_desc {
      .color_format = SG_PIXELFORMAT_RGBA8,
      .depth_format = SG_PIXELFORMAT_DEPTH,
  };
  sg_setup(sg_desc {
      .buffer_pool_size = 8,
      .image_pool_size = 1024,
      .shader_pool_size = 1,
      .pipeline_pool_size = 1,
      .context = context,
  });
  graphics::Renderer::shared = std::make_unique<graphics::Renderer>();

  _previous_id = 0;
  _id = xtInitApplication();
  _current_rto = xtGetRuntimeObject(_id);
}

void onFrame() {
  if (_id) {
    _current_rto->Init(_previous_id);
    _previous_id = _id;
    _id = 0;
  } else {
    _id = _current_rto->Quant();
    if (_id) {
      _current_rto->Finit();
      _current_rto = xtGetRuntimeObject(_id);
    }
  }
}

void onCleanup() {}

void onEvent(const sapp_event* event) {
  switch (event->type) {
    case SAPP_EVENTTYPE_KEY_DOWN: {
      XKey.PressFnc(VK_SPACE, 1);
      KeyBuf->put(VK_SPACE, KBD_CUR_KEY_PRESSED);
    } break;
    case SAPP_EVENTTYPE_KEY_UP: {
      XKey.UnPressFnc(VK_SPACE, 1);
      KeyBuf->put(VK_SPACE, KBD_CUR_KEY_UNPRESSED);
    } break;
    case SAPP_EVENTTYPE_MOUSE_MOVE: {
      auto x = event->mouse_x;
      auto y = event->mouse_y;

      const int x1 = XGR_MouseObj.PosX;
      const int y1 = XGR_MouseObj.PosY;

      XGR_MouseObj.InitPos(x, y);

      XGR_MouseObj.MovementX = x - x1;
      XGR_MouseObj.MovementY = y - y1;

      XGR_MouseObj.Move(0, XGR_MouseObj.PosX, XGR_MouseObj.PosY);
      if (XGR_MouseVisible())
      {
        XGR_MouseRedraw();
      }
    } break;
    case SAPP_EVENTTYPE_MOUSE_DOWN: {
      auto x = event->mouse_x;
      auto y = event->mouse_y;
      auto button = event->mouse_button == SAPP_MOUSEBUTTON_LEFT ? XGM_LEFT_BUTTON : XGM_RIGHT_BUTTON;

      XGR_MouseInitPos(x, y);
      XGR_MousePress(button, 0, XGR_MouseObj.PosX, XGR_MouseObj.PosY);
    } break;
    case SAPP_EVENTTYPE_MOUSE_UP: {
      auto x = event->mouse_x;
      auto y = event->mouse_y;
      auto button = event->mouse_button == SAPP_MOUSEBUTTON_LEFT ? XGM_LEFT_BUTTON : XGM_RIGHT_BUTTON;

      XGR_MouseInitPos(x, y);
      XGR_MouseUnPress(button, 0, XGR_MouseObj.PosX, XGR_MouseObj.PosY);
    }
  }
}
