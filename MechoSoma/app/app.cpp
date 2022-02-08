//
// Created by caiiiycuk on 07.02.22.
//

#include <sokol_app.h>

#include "KEYS.H"
#include "xgraph.h"
#include "xtool.h"

int _id;
int _previous_id;
XRuntimeObject* _current_rto;

void onInit() {
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

void onEvent(const sapp_event* event) {}
