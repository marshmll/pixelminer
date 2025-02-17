#include "stdafx.hxx"
#include "Engine/Languages.hxx"

void setup_i18n(const std::string_view locale)
{
#if WIN32
    // LocaleNameToLCID requires a LPCWSTR so we need to convert from char to wchar_t
    const auto wStringSize =
        MultiByteToWideChar(CP_UTF8, 0, locale.data(), static_cast<int>(locale.length()), nullptr, 0);
    std::wstring localeName;
    localeName.reserve(wStringSize);
    MultiByteToWideChar(CP_UTF8, 0, locale.data(), static_cast<int>(locale.length()), localeName.data(), wStringSize);

    _configthreadlocale(_DISABLE_PER_THREAD_LOCALE);
    const auto localeId = LocaleNameToLCID(localeName.c_str(), LOCALE_ALLOW_NEUTRAL_NAMES);
    SetThreadLocale(localeId);
    std::wcout << "Locale ID: " << localeId << std::endl;
#else
    setlocale(LC_MESSAGES, locale.data());
#endif

    // Bind the text domain
    bindtextdomain(DOMAIN, LOCALES_FOLDER.c_str());
    bind_textdomain_codeset(DOMAIN, "UTF-8");
    textdomain(DOMAIN);
}
