#pragma once

class UUID
{
  public:
    UUID();
    ~UUID();

    static const std::string generate();
};