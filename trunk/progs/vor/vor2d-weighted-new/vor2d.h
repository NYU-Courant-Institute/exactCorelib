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

using std::string;

// Stubs.
void Mouse(int button, int state, int x, int y);
void parse(string input);
void run();
void save_png();
