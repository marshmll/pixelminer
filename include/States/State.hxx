#pragma once

class State;

typedef struct 
{
	unsigned int *gridSize;
	std::stack<State *> *states;
} StateData;

class State
{
private:
	StateData &data;
	bool dead;

public:
	State(StateData &data);

	virtual ~State();
	
	virtual void update(const float &dt) = 0;

	virtual void render(sf::RenderTarget &target) = 0;

	const bool &isDead() const;

};