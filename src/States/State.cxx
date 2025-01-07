#include "States/State.hxx"
#include "stdafx.hxx"

State::State(StateData &data) : data(data), dead(false)
{
}

State::~State()
{
}

const bool &State::isDead() const
{
    return dead;
};