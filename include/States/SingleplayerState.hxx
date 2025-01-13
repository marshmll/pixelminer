#pragma once

#include "States/State.hxx"

class SingleplayerState : public State
{
  public:
    SingleplayerState(StateData &data);

    ~SingleplayerState();
};