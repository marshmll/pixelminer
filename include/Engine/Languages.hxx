#pragma once

#include "Engine/Configuration.hxx"
#include "libintl.h"

#if WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

#define DOMAIN "pixelminer"

#define _(STRING) std::string(gettext(STRING))

void setup_i18n(const std::string_view locale);