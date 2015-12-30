#pragma once

#include "fmod.hpp"
#include "fmod.h"
#include <sstream>
#include "fmod_errors.h"

using namespace std;

#pragma comment(lib, "fmodL_vc.lib")
void ERRCHECK_fn(FMOD_RESULT result, const char *file, int line);
#define ERRCHECK(_result) ERRCHECK_fn(_result, __FILE__, __LINE__)

