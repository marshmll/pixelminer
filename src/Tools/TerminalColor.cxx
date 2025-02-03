#include "Tools/TerminalColor.hxx"
#include "stdafx.hxx"

void TerminalColor::set(int color)
{
    std::cout << "\033[" << color << "m";
}

void TerminalColor::reset()
{
    std::cout << "\033[0m";
}
