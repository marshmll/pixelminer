#pragma once

#include "Engine/Languages.hxx"

#define ASSERT_ALWAYS(EXPR, MSG)                                                                                       \
    {                                                                                                                  \
        if (!EXPR)                                                                                                     \
            throw std::runtime_error(_("Assertion Failed: ") + MSG + "\n");                                            \
    }
