/**
 * @file Languages.hxx
 * @brief Header file for internationalization (i18n) support in the application.
 * This file defines macros and functions for setting up and using translations.
 */

#pragma once

#include "Engine/Configuration.hxx"
#include "libintl.h"

#if WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

/**
 * @brief The text domain used to search for internationalization (i18n) locales.
 * This domain is used to identify the translation catalog for the application.
 */
#define DOMAIN "pixelminer"

/**
 * @brief Macro for looking up translations in the current locale's text domain catalog.
 * This macro simplifies the process of retrieving translated strings by wrapping the `gettext` function
 * and converting the result to a `std::string`.
 *
 * @param STRING The string (`msgid`) to resolve to a translated string (`msgstr`).
 * @return The translated string if found in the catalog; otherwise, the original `STRING` is returned.
 */
#define _(STRING) std::move(std::string(gettext(STRING)))

/**
 * @brief Sets up internationalization (i18n) for the application by configuring the locale and text domain.
 * This function initializes the locale settings and binds the text domain for translation lookups.
 * On Windows, it configures the thread locale using the provided locale name. On other platforms,
 * it uses the standard `setlocale` function.
 *
 * @param locale The locale to set, specified as a string (e.g., "en_US.UTF-8").
 *               This determines the language and regional settings for translations.
 *
 * @note On Windows, the locale name is converted to a locale ID using `LocaleNameToLCID`,
 *       and the thread locale is set accordingly. On non-Windows platforms, `setlocale` is used.
 * @note The text domain is bound to the `DOMAIN` ("pixelminer"), and the catalog is expected to be
 *       located in the directory specified by `LOCALES_FOLDER`.
 * @note The text domain's encoding is set to UTF-8 to ensure proper handling of Unicode characters.
 */
void setup_i18n(const std::string_view locale);