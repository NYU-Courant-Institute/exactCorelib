#include <sstream>
#include <string>

#ifdef __CYGWIN32__
#include "glui.h"
#endif
#ifdef _WIN32
#include <gl/glui.h>
#endif
#ifdef __APPLE__
#include "glui.h"
#endif
#ifdef __linux__
#include <GL/glui.h>
#endif

#define WINDOW_WIDTH 1024
#define ABS_EPS 1.0d / (1 << 10)
// #define GEOM_EPS 0.5
#define GEOM_EPS 1.0d / (1 << 10)

// Directions.
// #define E 1
// #define W -1
// #define N 2
// #define S -2
