#pragma once

#include "States/State.hxx"

class MultiplayerState : public State
{
  public:
    MultiplayerState(StateData &data);

    ~MultiplayerState();
};