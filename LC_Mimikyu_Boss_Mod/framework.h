#pragma once

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>
#include <psapi.h>
#include <winuser.h>
#include <stdio.h>
#include <stdint.h>
#include <array>
#include <algorithm>
#include <time.h>
#include <winternl.h>
#include "GbaState.h"
#include "EXE6_F_functions.h"
#include "EXE6_G_functions.h"