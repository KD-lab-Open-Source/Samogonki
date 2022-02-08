//
// Created by caiiiycuk on 07.02.22.
//

#ifndef MOONSHINE_RUNNERS_APP_H
#define MOONSHINE_RUNNERS_APP_H

struct sapp_event;

void onInit();
void onFrame();
void onCleanup();
void onEvent(const sapp_event* event);

#endif  // MOONSHINE_RUNNERS_APP_H
