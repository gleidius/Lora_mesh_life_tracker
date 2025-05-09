#pragma once

// Display
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

// E52 default parameters
#define E52_POWER 15
#define E52_OPTION 1
#define E52_DST_ADDRESS 404
#define E52_RATE 0

// BMP280-
#define BMP_ADDRESS 0x76

#include <Arduino.h>

// Определение пинов для кнопок

#include "debug.h"

#include "variant.h"

#include "button.h"
#include "timerMillis.h"

#include "modules/meshApp.h"
#include "modules/e52.h"
#include "modules/BMP280.h"

#include "menu.h"