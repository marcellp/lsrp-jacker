#pragma once

#include <windows.h>
#pragma comment (lib,"User32.lib")

#include <string>
#include <assert.h>
#include <process.h>
#include <stdlib.h>

#define PROGRAM_NAME "lsrp-jacker"
#define PROGRAM_VERSION "1.1"
#define PROGRAM_AUTHORS "Marcell Pek"
#define PROGRAM_WEBSITE "http://marcellp.github.io"
#define PROGRAM_BUGREPORT_EMAIL "marcellp0+lsrp-jacker@gmail.com"

#include "SAMPFUNCS_API.h"
#include "game_api\game_api.h"

#include "main.hpp"
#include "unscrambler.hpp"
#include "unlocker.hpp"