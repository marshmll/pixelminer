#include "stdafx.hxx"
#include "States/State.hxx"

State::State(StateData &data) : data(data), dead(false) { }

State::~State() { }

const bool &State::isDead() const
{
	return dead;
};