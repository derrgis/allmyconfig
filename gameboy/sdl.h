#include <Arduino.h>
#define FASTLED_ESP32_I2S 1
#include "FastLED.h"

#ifndef SDL_H
#define SDL_H

int sdl_update(void);
void sdl_init(void);
void sdl_frame(void);
void sdl_quit(void);
byte* sdl_get_framebuffer(void);
unsigned int sdl_get_buttons(void);
unsigned int sdl_get_directions(void);
CRGB* get_leds();
CRGB* get_colors();
#endif
