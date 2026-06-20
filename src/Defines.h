#pragma once

#include "nexus-core/Nexus.h"

#if __has_include("../submodules/nexus-mumble/Mumble.h")
#define USE_MUMBLE
#include "nexus-mumble/Mumble.h"
#endif

#if __has_include("../submodules/nexus-rtapi/RTAPI.h")
#define USE_RTAPI
#include "nexus-rtapi/RTAPI.h"
#endif
