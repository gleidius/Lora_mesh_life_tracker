#include <Arduino.h>

// #include "variant.h"
#define DEBUG_SERIAL
// ------------------- МАКРО --------------------
#ifdef DEBUG_SERIAL
#define DEBUGLN(x) Serial.println(x)
#define DEBUGF(x, y) Serial.printf(x, y)
#define DEBUG(x) Serial.print(x)
#else
#define DEBUGLN(x)
#define DEBUGF(x, y)
#define DEBUG(x)
#endif

#include "serialHandler.h"

#include "modules/e52.h"